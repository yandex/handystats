// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <limits>
#include <algorithm>
#include <stdexcept>

#include <handystats/common.h>
#include <handystats/math_utils.hpp>

#include <handystats/detail/statistics.hpp>

namespace handystats {

// statistics::data

statistics::data::data()
	: m_moving_interval(chrono::nanoseconds(0))
	, m_histogram_bins(0)
	, m_tags(tag::empty)
{
	m_tags |= tag::timestamp;
	reset();
}

statistics::data::data(const config::statistics& config)
	: m_moving_interval(config.moving_interval)
	, m_histogram_bins(config.histogram_bins)
{
	m_tags = tag::empty;
	m_tags |= tag::timestamp;

	if (config.tags & tag::value) {
		m_tags |= tag::value;
	}

	if (config.tags & tag::min) {
		m_tags |= tag::min;
	}

	if (config.tags & tag::max) {
		m_tags |= tag::max;
	}

	if (config.tags & tag::count) {
		m_tags |= tag::count;
	}

	if (config.tags & tag::sum) {
		m_tags |= tag::sum;
	}

	if (config.tags & tag::avg) {
		m_tags |= tag::avg | tag::count | tag::sum;
	}

	if (config.tags & tag::moving_count) {
		m_tags |= tag::moving_count;
	}

	if (config.tags & tag::moving_sum) {
		m_tags |= tag::moving_sum;
	}

	if (config.tags & tag::moving_avg) {
		m_tags |= tag::moving_avg | tag::moving_count | tag::moving_sum;
	}

	if (config.tags & tag::histogram) {
		m_tags |= tag::histogram;
	}

	if (config.tags & tag::quantile) {
		m_tags |= tag::quantile | tag::histogram;
	}

	if (config.tags & tag::entropy) {
		m_tags |= tag::entropy | tag::histogram;
	}

	if (config.tags & tag::throughput) {
		m_tags |= tag::throughput | tag::moving_sum;
	}

	if (config.tags & tag::frequency) {
		m_tags |= tag::frequency | tag::moving_count;
	}

	reset();
}

void statistics::data::reset() {
	m_value = value_type(0);
	m_min = std::numeric_limits<value_type>::max();
	m_max = -std::numeric_limits<value_type>::max();
	m_sum = value_type(0);
	m_count = 0;
	m_moving_count = 0.0;
	m_moving_sum = 0.0;
	m_histogram.clear();
	if (m_histogram_bins > 0) {
		m_histogram.reserve(m_histogram_bins + 1);
	}
	m_data_timestamp = time_point();

	m_timestamp = time_point();
}

static
double shift_data(
		const statistics::data& ctx,
		const double& data, const statistics::time_point& data_timestamp,
		const statistics::time_point& timestamp
	)
{
	// data is uninitialized
	if (data_timestamp == statistics::time_point()) return 0;

	if (timestamp <= ctx.m_timestamp) return data;

	const auto& stale_interval = data_timestamp - (timestamp - ctx.m_moving_interval);

	if (stale_interval.count() <= 0) return 0;

	return data * stale_interval.count() /
		(ctx.m_moving_interval - (ctx.m_timestamp - data_timestamp)).count();
}

static
double update_data(
		const statistics::data& ctx,
		const double& data, const statistics::time_point& data_timestamp,
		const statistics::value_type& value, const statistics::time_point& timestamp
	)
{
	// data is uninitialized
	if (data_timestamp == statistics::time_point()) return value;

	if (timestamp <= ctx.m_timestamp) {
		if (timestamp < ctx.m_timestamp - ctx.m_moving_interval) {
			return data;
		}
		else {
			return data + value;
		}
	}
	else {
		return value + shift_data(ctx, data, data_timestamp, timestamp);
	}

	return data;
}

static
double truncate_data_before(
		const statistics::data& ctx,
		const double& data, const statistics::time_point& data_timestamp,
		const statistics::time_point& timestamp
	)
{
	// data is uninitialized
	if (data_timestamp == statistics::time_point()) return 0;

	if (math_utils::cmp<double>(data, 0) == 0) return 0;

	if (timestamp >= data_timestamp) return 0;

	if (timestamp <= ctx.m_timestamp - ctx.m_moving_interval) return data;

	const auto& stale_interval = timestamp - (ctx.m_timestamp - ctx.m_moving_interval);
	const auto& data_interval = data_timestamp - (ctx.m_timestamp - ctx.m_moving_interval);

	return data * (data_interval - stale_interval).count() / data_interval.count();
}

static
double truncate_data_after(
		const statistics::data& ctx,
		const double& data, const statistics::time_point& data_timestamp,
		const statistics::time_point& timestamp
	)
{
	// data is uninitialized
	if (data_timestamp == statistics::time_point()) return 0;

	if (math_utils::cmp<double>(data, 0) == 0) return 0;

	if (timestamp >= data_timestamp) return data;

	if (timestamp <= ctx.m_timestamp - ctx.m_moving_interval) return 0;

	const auto& stale_interval = timestamp - (ctx.m_timestamp - ctx.m_moving_interval);
	const auto& data_interval = data_timestamp - (ctx.m_timestamp - ctx.m_moving_interval);

	return data * stale_interval.count() / data_interval.count();
}

static
void shift_histogram(
		statistics::data& ctx,
		const statistics::time_point& timestamp
	)
{
	if (ctx.m_histogram_bins == 0) return;

	for (auto bin = ctx.m_histogram.begin(); bin != ctx.m_histogram.end(); ++bin) {
		auto& bin_count = std::get<statistics::BIN_COUNT>(*bin);
		auto& bin_timestamp = std::get<statistics::BIN_TIMESTAMP>(*bin);

		bin_count = shift_data(ctx, bin_count, bin_timestamp, timestamp);

		if (math_utils::cmp<double>(bin_count, 0) == 0) {
			bin_count = 0;
		}
	}
}

static
void truncate_histogram_before(
		statistics::data& ctx,
		const statistics::time_point& timestamp
	)
{
	if (ctx.m_histogram_bins == 0) return;

	for (auto bin = ctx.m_histogram.begin(); bin != ctx.m_histogram.end(); ++bin) {
		auto& bin_count = std::get<statistics::BIN_COUNT>(*bin);
		auto& bin_timestamp = std::get<statistics::BIN_TIMESTAMP>(*bin);

		bin_count = truncate_data_before(ctx, bin_count, bin_timestamp, timestamp);

		if (math_utils::cmp<double>(bin_count, 0) == 0) {
			bin_count = 0;
		}
	}
}

static
void truncate_histogram_after(
		statistics::data& ctx,
		const statistics::time_point& timestamp
	)
{
	if (ctx.m_histogram_bins == 0) return;

	for (auto bin = ctx.m_histogram.begin(); bin != ctx.m_histogram.end(); ++bin) {
		auto& bin_count = std::get<statistics::BIN_COUNT>(*bin);
		auto& bin_timestamp = std::get<statistics::BIN_TIMESTAMP>(*bin);

		bin_count = truncate_data_after(ctx, bin_count, bin_timestamp, timestamp);

		if (math_utils::cmp<double>(bin_count, 0) == 0) {
			bin_count = 0;
		}
	}
}

static
double bin_merge_criteria(
		const statistics::bin_type& left_bin,
		const statistics::bin_type& right_bin
	)
{
	// possible variants:
	// * distance between bins' centers
	// * sum of bins' weights (number of elements)
	// * other heuristics

	// distance between bins' centers
	return std::get<statistics::BIN_CENTER>(right_bin) - std::get<statistics::BIN_CENTER>(left_bin);

	// sum of bins' weights
	//return std::get<statistics::BIN_COUNT>(left_bin) + std::get<statistics::BIN_COUNT>(right_bin);

	// heuristic -- minimum resulted bin square
	//return (std::get<statistics::BIN_COUNT>(left_bin) + std::get<statistics::BIN_COUNT>(right_bin)) *
	//	(std::get<statistics::BIN_CENTER>(right_bin) - std::get<statistics::BIN_CENTER>(left_bin));
}

static
void compress_histogram(statistics::histogram_type& histogram, const size_t& histogram_bins) {
	if (histogram.size() > histogram_bins) {
		// look for empty bins
		auto bin_iter = histogram.rbegin();
		while (bin_iter != histogram.rend() && histogram.size() > histogram_bins) {
			if (math_utils::cmp<double>(std::get<statistics::BIN_COUNT>(*bin_iter), 0) == 0) {
				auto empty_bin_iter = std::next(bin_iter).base();
				++bin_iter;

				histogram.erase(empty_bin_iter);
			}
			else {
				++bin_iter;
			}
		}
	}

	while (histogram.size() > histogram_bins) {
		size_t best_merge_index = -1;
		double best_merge_criteria = 0;
		for (size_t index = 0; index < histogram.size() - 1; ++index) {
			double merge_criteria = bin_merge_criteria(histogram[index], histogram[index + 1]);
			if (best_merge_index == -1 || math_utils::cmp(merge_criteria, best_merge_criteria) < 0) {
				best_merge_index = index;
				best_merge_criteria = merge_criteria;
			}
		}

		auto& left_bin = histogram[best_merge_index];
		auto& right_bin = histogram[best_merge_index + 1];

		if (math_utils::cmp(std::get<statistics::BIN_COUNT>(left_bin), 0.0) <= 0 &&
				math_utils::cmp(std::get<statistics::BIN_COUNT>(right_bin), 0.0) <= 0)
		{
			std::get<statistics::BIN_CENTER>(left_bin) =
				math_utils::weighted_average(
						std::get<statistics::BIN_CENTER>(left_bin), 1,
						std::get<statistics::BIN_CENTER>(right_bin), 1
						);

			std::get<statistics::BIN_COUNT>(left_bin) = 0;

			std::get<statistics::BIN_TIMESTAMP>(left_bin) =
				std::max(
						std::get<statistics::BIN_TIMESTAMP>(left_bin),
						std::get<statistics::BIN_TIMESTAMP>(right_bin)
					);
		}
		else {
			std::get<statistics::BIN_CENTER>(left_bin) =
				math_utils::weighted_average(
						std::get<statistics::BIN_CENTER>(left_bin), std::get<statistics::BIN_COUNT>(left_bin),
						std::get<statistics::BIN_CENTER>(right_bin), std::get<statistics::BIN_COUNT>(right_bin)
						);

			std::get<statistics::BIN_COUNT>(left_bin) += std::get<statistics::BIN_COUNT>(right_bin);

			std::get<statistics::BIN_TIMESTAMP>(left_bin) =
				std::max(
						std::get<statistics::BIN_TIMESTAMP>(left_bin),
						std::get<statistics::BIN_TIMESTAMP>(right_bin)
					);
		}

		histogram.erase(histogram.begin() + best_merge_index + 1);
	}
}

static
void update_histogram(
		statistics::data& ctx,
		const statistics::value_type& value, const statistics::time_point& timestamp
	)
{
	if (ctx.m_histogram_bins == 0) return;

	statistics::bin_type new_bin(value, 1.0, timestamp);

	auto insert_iter = std::lower_bound(ctx.m_histogram.begin(), ctx.m_histogram.end(), new_bin);
	ctx.m_histogram.insert(insert_iter, std::move(new_bin));

	shift_histogram(ctx, timestamp);

	compress_histogram(ctx.m_histogram, ctx.m_histogram_bins);
}

void statistics::data::update(const value_type& value, const time_point& timestamp) {
	if (m_tags & tag::value) {
		m_value = value;
	}

	if (m_tags & tag::min) {
		m_min = std::min(m_min, value);
	}

	if (m_tags & tag::max) {
		m_max = std::max(m_max, value);
	}

	if (m_tags & tag::sum) {
		m_sum += value;
	}

	if (m_tags & tag::count) {
		++m_count;
	}

	if (m_tags & tag::moving_count) {
		m_moving_count = update_data(*this, m_moving_count, m_data_timestamp, 1, timestamp);
	}

	if (m_tags & tag::moving_sum) {
		m_moving_sum = update_data(*this, m_moving_sum, m_data_timestamp, value, timestamp);
	}

	if (m_tags & tag::histogram) {
		update_histogram(*this, value, timestamp);
	}

	m_timestamp = std::max(m_timestamp, timestamp);
	m_data_timestamp = std::max(m_data_timestamp, timestamp);
}

void statistics::data::update_time(const time_point& timestamp) {
	if (timestamp <= m_timestamp) return;

	if (m_tags & tag::moving_count) {
		m_moving_count = shift_data(*this, m_moving_count, m_data_timestamp, timestamp);
		if (math_utils::cmp<double>(m_moving_count, 0) == 0) {
			m_moving_count = 0;
		}
	}

	if (m_tags & tag::moving_sum) {
		m_moving_sum = shift_data(*this, m_moving_sum, m_data_timestamp, timestamp);
		if (math_utils::cmp<double>(m_moving_sum, 0) == 0) {
			m_moving_sum = 0;
		}
	}

	if (m_tags & tag::histogram) {
		shift_histogram(*this, timestamp);
	}

	m_timestamp = std::max(m_timestamp, timestamp);
}

void statistics::data::append(data d) {
	if (m_timestamp == time_point()) {
		// this is uninitialized
		*this = d;
		return;
	}

	if (d.m_timestamp == time_point()) {
		// data is uninitialized
		return;
	}

	if (d.m_timestamp <= m_timestamp) {
		// data to append is too old
		return;
	}
	if (d.m_data_timestamp <= m_data_timestamp) {
		// data to append is too old
		// but timestamp should be updated along with moving_interval
		m_moving_interval += d.m_timestamp - m_timestamp;
		m_timestamp = d.m_timestamp;
		return;
	}

	if (d.m_tags & tag::value) {
		m_value = d.m_value;
		m_tags |= tag::value;
	}
	if (d.m_tags & tag::min) {
		m_min = d.m_min;
		m_tags |= tag::min;
	}
	if (d.m_tags & tag::max) {
		m_max = d.m_max;
		m_tags |= tag::max;
	}
	if (d.m_tags & tag::sum) {
		m_sum = d.m_sum;
		m_tags |= tag::sum;
	}
	if (d.m_tags & tag::count) {
		m_count = d.m_count;
		m_tags |= tag::count;
	}

	if (d.m_tags & tag::moving_count) {
		m_moving_count += truncate_data_before(d, d.m_moving_count, d.m_data_timestamp, m_timestamp);
		m_tags |= tag::moving_count;
	}
	if (d.m_tags & tag::moving_sum) {
		m_moving_sum += truncate_data_before(d, d.m_moving_sum, d.m_data_timestamp, m_timestamp);
		m_tags |= tag::moving_sum;
	}

	if (d.m_tags & tag::histogram) {
		truncate_histogram_before(d, m_timestamp);
		m_histogram.reserve(m_histogram.size() + d.m_histogram.size());
		m_histogram.insert(m_histogram.end(), d.m_histogram.begin(), d.m_histogram.end());
		std::sort(m_histogram.begin(), m_histogram.end());
		compress_histogram(m_histogram, m_histogram_bins);

		m_tags |= tag::histogram;
	}

	m_moving_interval += d.m_timestamp - m_timestamp;

	m_timestamp = d.m_timestamp;
	m_data_timestamp = d.m_data_timestamp;
}

void statistics::data::merge(const data& d) {
	if (m_timestamp == time_point()) {
		// this is uninitialized
		*this = d;
		return;
	}

	if (d.m_timestamp == time_point()) {
		// data is uninitialized
		return;
	}

	if (d.m_tags & tag::value) {
		if (
			((m_tags & tag::value) && d.m_data_timestamp > m_data_timestamp) ||
			!(m_tags & tag::value)
		)
		{
			m_value = d.m_value;
			m_tags |= tag::value;
		}
	}
	if (d.m_tags & tag::min) {
		m_min = std::min(m_min, d.m_min);
		m_tags |= tag::min;
	}
	if (d.m_tags & tag::max) {
		m_max = std::max(m_max, d.m_max);
		m_tags |= tag::max;
	}
	if (d.m_tags & tag::sum) {
		m_sum += d.m_sum;
		m_tags |= tag::sum;
	}
	if (d.m_tags & tag::count) {
		m_count += d.m_count;
		m_tags |= tag::count;
	}

	if (d.m_tags & tag::moving_count) {
		m_moving_count += d.m_moving_count;
		m_tags |= tag::moving_count;
	}
	if (d.m_tags & tag::moving_sum) {
		m_moving_sum += d.m_moving_sum;
		m_tags |= tag::moving_sum;
	}

	if (d.m_tags & tag::histogram) {
		m_histogram.reserve(m_histogram.size() + d.m_histogram.size());
		m_histogram.insert(m_histogram.end(), d.m_histogram.begin(), d.m_histogram.end());
		std::sort(m_histogram.begin(), m_histogram.end());
		compress_histogram(m_histogram, m_histogram_bins);

		m_tags |= tag::histogram;
	}

	m_data_timestamp = std::max(m_data_timestamp, d.m_data_timestamp);

	const auto& last_timestamp = std::min(m_timestamp - m_moving_interval, d.m_timestamp - d.m_moving_interval);
	m_timestamp = std::max(m_timestamp, d.m_timestamp);
	m_moving_interval = m_timestamp - last_timestamp;
}

void statistics::data::truncate_before(const time_point& timestamp) {
	if (m_tags & tag::moving_count) {
		m_moving_count = truncate_data_before(*this, m_moving_count, m_data_timestamp, timestamp);
		if (math_utils::cmp<double>(m_moving_count, 0) == 0) {
			m_moving_count = 0;
		}
	}

	if (m_tags & tag::moving_sum) {
		m_moving_sum = truncate_data_before(*this, m_moving_sum, m_data_timestamp, timestamp);
		if (math_utils::cmp<double>(m_moving_sum, 0) == 0) {
			m_moving_sum = 0;
		}
	}

	if (m_tags & tag::histogram) {
		truncate_histogram_before(*this, timestamp);
	}

	if (timestamp >= m_data_timestamp) {
		m_data_timestamp = time_point();
	}

	if (timestamp >= m_timestamp) {
		m_timestamp = time_point();
		m_moving_interval = chrono::duration(0, m_moving_interval.unit());
	}
	else {
		m_moving_interval = std::min(m_moving_interval, m_timestamp - timestamp);
	}
}

void statistics::data::truncate_after(const time_point& timestamp) {
	if (m_tags & tag::moving_count) {
		m_moving_count = truncate_data_after(*this, m_moving_count, m_data_timestamp, timestamp);
		if (math_utils::cmp<double>(m_moving_count, 0) == 0) {
			m_moving_count = 0;
		}
	}

	if (m_tags & tag::moving_sum) {
		m_moving_sum = truncate_data_after(*this, m_moving_sum, m_data_timestamp, timestamp);
		if (math_utils::cmp<double>(m_moving_sum, 0) == 0) {
			m_moving_sum = 0;
		}
	}

	if (m_tags & tag::histogram) {
		truncate_histogram_after(*this, timestamp);
	}

	if (timestamp <= m_timestamp - m_moving_interval) {
		m_data_timestamp = time_point();
		m_timestamp = time_point();
		m_moving_interval = chrono::duration(0, m_moving_interval.unit());
	}
	else {
		const auto& last_timestamp = m_timestamp - m_moving_interval;

		m_data_timestamp = std::min(m_data_timestamp, timestamp);
		m_timestamp = std::min(m_timestamp, timestamp);
		m_moving_interval = m_timestamp - last_timestamp;
	}
}


void statistics::data::fulfill_dependencies() {
	// timestamp - no dependency
	// value - no dependency
	// min - no dependency
	// max - no dependency
	// count - no dependency
	// sum - no dependecy

	// avg - depends on sum & count
	if ((m_tags & tag::count) && (m_tags & tag::sum)) {
		m_tags |= tag::avg;
	}
	else {
		m_tags &= ~tag::avg;
	}

	// moving_count - depends on moving_interval
	if ((m_tags & tag::moving_count) &&
			(m_moving_interval.count() > 0)
		)
	{
		m_tags |= tag::moving_count;
	}
	else {
		m_tags &= ~tag::moving_count;
	}

	// moving_sum - depends on moving_interval
	if ((m_tags & tag::moving_sum) &&
			(m_moving_interval.count() > 0)
		)
	{
		m_tags |= tag::moving_sum;
	}
	else {
		m_tags &= ~tag::moving_sum;
	}

	// moving_avg - depends on moving_count and moving_sum
	if ((m_tags & tag::moving_count) && (m_tags & tag::moving_sum)) {
		m_tags |= tag::moving_avg;
	}
	else {
		m_tags &= ~tag::moving_avg;
	}

	// histogram - depends on moving_interval
	if ((m_tags & tag::histogram) &&
			(m_moving_interval.count() > 0)
		)
	{
		m_tags |= tag::histogram;
	}
	else {
		m_tags &= ~tag::histogram;
	}

	// quantile - depends on histogram
	if (m_tags & tag::histogram) {
		m_tags |= tag::quantile;
	}
	else {
		m_tags &= ~tag::quantile;
	}

	// entropy - depends on histogram
	if (m_tags & tag::histogram) {
		m_tags |= tag::entropy;
	}
	else {
		m_tags &= ~tag::entropy;
	}

	// throughput - depends on moving_sum
	if (m_tags & tag::moving_sum) {
		m_tags |= tag::throughput;
	}
	else {
		m_tags &= ~tag::throughput;
	}

	// frequency - depends on moving_count
	if (m_tags & tag::moving_count) {
		m_tags |= tag::frequency;
	}
	else {
		m_tags &= ~tag::frequency;
	}
}

bool statistics::data::computed(const statistics::tag::type& t) const HANDYSTATS_NOEXCEPT {
	return m_tags & t;
}


statistics::quantile_extractor::quantile_extractor(const statistics::data* const data)
	: m_data(data)
{}


// a x^2 + b x + c == 0
// z -- root in [0, 1]
static
long double find_z(const long double& a, const long double& b, const long double& c) {
	const auto& roots = handystats::math_utils::solve_quadratic(a, b, c);
	for (size_t index = 0; index < roots.size(); ++index) {
		if (handystats::math_utils::cmp<double>(roots[index], 0.0) >= 0 &&
				handystats::math_utils::cmp<double>(roots[index], 1.0) <= 0
			)
		{
			return roots[index];
		}
	}
	throw std::logic_error("find_z: not found (" + std::to_string(a) + ", " + std::to_string(b) + ", " + std::to_string(c) + ")");
}

double statistics::quantile_extractor::at(const double& probability) const {
	if (math_utils::cmp<double>(probability, 0) <= 0 || math_utils::cmp<double>(probability, 1) >= 0) {
		throw std::logic_error("probability should be in (0, 1)");
	}

	if (m_data == nullptr) {
		return 0;
	}

	const auto& histogram = m_data->m_histogram;

	if (histogram.size() == 0) {
		return 0;
	}

	double moving_count = 0;
	for (auto bin = histogram.begin(); bin != histogram.end(); ++bin) {
		moving_count += std::get<BIN_COUNT>(*bin);
	}

	if (math_utils::cmp<double>(moving_count, 0) <= 0) {
		return 0;
	}

	if (histogram.size() == 1) {
		return std::get<BIN_CENTER>(histogram[0]);
	}

	double required_count = moving_count * probability;

	int bin_index = -1;
	for (; bin_index < (int)histogram.size() - 1; ++bin_index) {
		double volume =
			(
				(bin_index == -1 ? 0 : std::get<BIN_COUNT>(histogram[bin_index]))
				+ (bin_index + 1 == histogram.size() ? 0 : std::get<BIN_COUNT>(histogram[bin_index + 1]))
			) / 2.0;

		if (math_utils::cmp(volume, required_count) > 0) break;

		required_count -= volume;
	}

	bin_type left_bin;
	bin_type right_bin;

	if (bin_index == -1) {
		left_bin = bin_type{
			2 * std::get<BIN_CENTER>(histogram[0]) -
				math_utils::weighted_average(
						std::get<BIN_CENTER>(histogram[0]), std::get<BIN_COUNT>(histogram[0]),
						std::get<BIN_CENTER>(histogram[1]), std::get<BIN_COUNT>(histogram[1])
					),
			0,
			std::get<BIN_TIMESTAMP>(histogram[0]) // don't care
		};
		right_bin = histogram[0];
	}
	else if (bin_index + 1 < histogram.size()) {
		left_bin = histogram[bin_index];
		right_bin = histogram[bin_index + 1];
	}
	else {
		left_bin = histogram[bin_index];
		right_bin = bin_type{
			2 * std::get<BIN_CENTER>(histogram[bin_index]) -
				math_utils::weighted_average(
						std::get<BIN_CENTER>(histogram[bin_index - 1]), std::get<BIN_COUNT>(histogram[bin_index - 1]),
						std::get<BIN_CENTER>(histogram[bin_index]), std::get<BIN_COUNT>(histogram[bin_index])
					),
			0,
			std::get<BIN_TIMESTAMP>(histogram[bin_index]) // don't care
		};
	}

	const double& a = std::get<BIN_COUNT>(right_bin) - std::get<BIN_COUNT>(left_bin);
	const double& b = 2 * std::get<BIN_COUNT>(left_bin);
	const double& c = -2 * required_count;

	try {
		const double& z = find_z(a, b, c);

		return std::get<BIN_CENTER>(left_bin) + (std::get<BIN_CENTER>(right_bin) - std::get<BIN_CENTER>(left_bin)) * z;
	}
	catch (const std::logic_error&) {
		// z is not found
		return 0;
	}
}

} // namespace handystats
