// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <cstring>
#include <limits>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>
#include <utility>

#include <handystats/common.h>
#include <handystats/math_utils.hpp>

#include <handystats/statistics.hpp>

#include <handystats/detail/statistics.hpp>

// a x^2 + b x + c == 0
// z -- root in [0, 1]
static long double
find_z(const long double& a, const long double& b, const long double& c) {
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
	m_max = std::numeric_limits<value_type>::min();
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
	if ((m_tags & tag::throughput) && (m_tags & tag::moving_sum)) {
		m_tags |= tag::throughput;
	}
	else {
		m_tags &= ~tag::throughput;
	}

	// frequency - depends on moving_count
	if ((m_tags & tag::frequency) && (m_tags & tag::moving_count)) {
		m_tags |= tag::frequency;
	}
	else {
		m_tags &= ~tag::frequency;
	}
}

statistics::quantile_extractor::quantile_extractor(const statistics* const statistics)
	: m_statistics(statistics)
{}

double statistics::quantile_extractor::at(const double& probability) const {
	if (math_utils::cmp<double>(probability, 0) <= 0 || math_utils::cmp<double>(probability, 1) >= 0) {
		throw std::logic_error("probability should be in (0, 1)");
	}

	if (m_statistics == nullptr) {
		return 0;
	}

	const auto& histogram = m_statistics->m_data->m_histogram;

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

const statistics::tag::type statistics::tag::empty;
const statistics::tag::type statistics::tag::value;
const statistics::tag::type statistics::tag::min;
const statistics::tag::type statistics::tag::max;
const statistics::tag::type statistics::tag::count;
const statistics::tag::type statistics::tag::sum;
const statistics::tag::type statistics::tag::avg;

const statistics::tag::type statistics::tag::moving_count;
const statistics::tag::type statistics::tag::moving_sum;
const statistics::tag::type statistics::tag::moving_avg;
const statistics::tag::type statistics::tag::histogram;
const statistics::tag::type statistics::tag::quantile;
const statistics::tag::type statistics::tag::entropy;
const statistics::tag::type statistics::tag::throughput;
const statistics::tag::type statistics::tag::frequency;

const statistics::tag::type statistics::tag::timestamp;

statistics::tag::type statistics::tag::from_string(const std::string& tag_name) {
	if (strcmp("value", tag_name.c_str()) == 0) {
		return value;
	}
	if (strcmp("min", tag_name.c_str()) == 0) {
		return min;
	}
	if (strcmp("max", tag_name.c_str()) == 0) {
		return max;
	}
	if (strcmp("count", tag_name.c_str()) == 0) {
		return count;
	}
	if (strcmp("sum", tag_name.c_str()) == 0) {
		return sum;
	}
	if (strcmp("avg", tag_name.c_str()) == 0) {
		return avg;
	}

	if (strcmp("moving-count", tag_name.c_str()) == 0) {
		return moving_count;
	}
	if (strcmp("moving-sum", tag_name.c_str()) == 0) {
		return moving_sum;
	}
	if (strcmp("moving-avg", tag_name.c_str()) == 0) {
		return moving_avg;
	}
	if (strcmp("histogram", tag_name.c_str()) == 0) {
		return histogram;
	}
	if (strcmp("quantile", tag_name.c_str()) == 0) {
		return quantile;
	}
	if (strcmp("entropy", tag_name.c_str()) == 0) {
		return entropy;
	}
	if (strcmp("throughput", tag_name.c_str()) == 0) {
		return throughput;
	}
	if (strcmp("frequency", tag_name.c_str()) == 0) {
		return frequency;
	}

	if (strcmp("timestamp", tag_name.c_str()) == 0) {
		return timestamp;
	}

	throw invalid_tag_error("tag::from_string: invalid tag");
}

bool statistics::enabled(const statistics::tag::type& t) const HANDYSTATS_NOEXCEPT {
	return (m_config.tags & t) && computed(t);
}

bool statistics::computed(const statistics::tag::type& t) const HANDYSTATS_NOEXCEPT {
	return m_data->m_tags & t;
}

statistics::tag::type statistics::tags() const HANDYSTATS_NOEXCEPT {
	return m_config.tags;
}

statistics::statistics(
			const config::statistics& opts
		)
	: m_config(opts)
	, m_data(new data(opts))
{
	reset();
}

statistics::statistics(statistics&& stats)
	: m_config(std::move(stats.m_config))
	, m_data(std::move(stats.m_data))
{
}

statistics::statistics(const statistics& stats)
	: m_config(stats.m_config)
	, m_data(new data(*stats.m_data)) // deep copy
{
}

statistics::statistics(const data& d)
	: m_config()
	, m_data(new data(d))
{
	m_config.moving_interval = m_data->m_moving_interval;
	m_config.histogram_bins = m_data->m_histogram_bins;

	m_config.tags = tag::empty;

	m_data->fulfill_dependencies();
}

statistics::~statistics()
{
	m_data.reset();
}

statistics& statistics::operator= (const statistics& stats) {
	m_config = stats.m_config;
	m_data = std::unique_ptr<data>(new data(*stats.m_data));

	return *this;
}

statistics& statistics::operator= (statistics&& stats) {
	m_config = std::move(stats.m_config);
	m_data.swap(stats.m_data);

	stats.m_data.reset();

	return *this;
}

void statistics::reset() {
	m_data->reset();
}

void statistics::update(const value_type& value, const time_point& timestamp) {
	m_data->update(value, timestamp);
}

void statistics::update_time(const time_point& timestamp) {
	m_data->update_time(timestamp);
}

// get_impl
template <>
statistics::result_type<statistics::tag::value>::type
statistics::get_impl<statistics::tag::value>() const
{
	if (computed(tag::value)) {
		return m_data->m_value;
	}
	else {
		throw invalid_tag_error("invalid tag: value");
	}
}

template <>
statistics::result_type<statistics::tag::min>::type
statistics::get_impl<statistics::tag::min>() const
{
	if (computed(tag::min)) {
		return m_data->m_min;
	}
	else {
		throw invalid_tag_error("invalid tag: min");
	}
}

template <>
statistics::result_type<statistics::tag::max>::type
statistics::get_impl<statistics::tag::max>() const
{
	if (computed(tag::max)) {
		return m_data->m_max;
	}
	else {
		throw invalid_tag_error("invalid tag: max");
	}
}

template <>
statistics::result_type<statistics::tag::count>::type
statistics::get_impl<statistics::tag::count>() const
{
	if (computed(tag::count)) {
		return m_data->m_count;
	}
	else {
		throw invalid_tag_error("invalid tag: count");
	}
}

template <>
statistics::result_type<statistics::tag::sum>::type
statistics::get_impl<statistics::tag::sum>() const
{
	if (computed(tag::sum)) {
		return m_data->m_sum;
	}
	else {
		throw invalid_tag_error("invalid tag: sum");
	}
}

template <>
statistics::result_type<statistics::tag::avg>::type
statistics::get_impl<statistics::tag::avg>() const
{
	if (computed(tag::avg)) {
		if (m_data->m_count == 0) {
			return 0;
		}
		else {
			return result_type<tag::avg>::type(m_data->m_sum) / m_data->m_count;
		}
	}
	else {
		throw invalid_tag_error("invalid tag: avg");
	}
}

template <>
statistics::result_type<statistics::tag::moving_count>::type
statistics::get_impl<statistics::tag::moving_count>() const
{
	if (computed(tag::moving_count)) {
		return m_data->m_moving_count;
	}
	else {
		throw invalid_tag_error("invalid tag: moving_count");
	}
}

template <>
statistics::result_type<statistics::tag::moving_sum>::type
statistics::get_impl<statistics::tag::moving_sum>() const
{
	if (computed(tag::moving_sum)) {
		return m_data->m_moving_sum;
	}
	else {
		throw invalid_tag_error("invalid tag: moving_sum");
	}
}

template <>
statistics::result_type<statistics::tag::moving_avg>::type
statistics::get_impl<statistics::tag::moving_avg>() const
{
	if (computed(tag::moving_avg)) {
		if (math_utils::cmp<result_type<tag::moving_count>::type>(m_data->m_moving_count, 0) <= 0) {
			return 0;
		}
		else {
			return result_type<tag::moving_avg>::type(m_data->m_moving_sum) / m_data->m_moving_count;
		}
	}
	else {
		throw invalid_tag_error("invalid tag: moving_avg");
	}
}

template <>
statistics::result_type<statistics::tag::histogram>::type
statistics::get_impl<statistics::tag::histogram>() const
{
	if (computed(tag::histogram)) {
		return m_data->m_histogram;
	}
	else {
		throw invalid_tag_error("invalid tag: histogram");
	}
}

template <>
statistics::result_type<statistics::tag::quantile>::type
statistics::get_impl<statistics::tag::quantile>() const
{
	if (computed(tag::quantile)) {
		return result_type<tag::quantile>::type(this);
	}
	else {
		throw invalid_tag_error("invalid tag: quantile");
	}
}

template <>
statistics::result_type<statistics::tag::entropy>::type
statistics::get_impl<statistics::tag::entropy>() const
{
	if (computed(tag::entropy)) {
		const auto& histogram = m_data->m_histogram;

		if (histogram.size() <= 1) {
			return 0;
		}

		double moving_count = 0;
		for (auto bin = histogram.begin(); bin != histogram.end(); ++bin) {
			moving_count += std::get<BIN_COUNT>(*bin);
		}
		if (math_utils::cmp<double>(moving_count, 0) <= 0) {
			return 0;
		}

		double H = 0;
		for (size_t bin_index = 0; bin_index < histogram.size(); ++bin_index) {
			const auto& bin = histogram[bin_index];
			if (math_utils::cmp<double>(std::get<BIN_COUNT>(bin), 0) <= 0) {
				continue;
			}

			double bin_width = 0;
			if (bin_index > 0) {
				bin_width +=
					(std::get<BIN_CENTER>(bin) - std::get<BIN_CENTER>(histogram[bin_index - 1])) *
						std::get<BIN_COUNT>(bin) / (std::get<BIN_COUNT>(bin) + std::get<BIN_COUNT>(histogram[bin_index - 1]));
			}
			if (bin_index < histogram.size() - 1) {
				bin_width +=
					(std::get<BIN_CENTER>(histogram[bin_index + 1]) - std::get<BIN_CENTER>(bin)) *
						std::get<BIN_COUNT>(bin) / (std::get<BIN_COUNT>(bin) + std::get<BIN_COUNT>(histogram[bin_index + 1]));
			}
			if (bin_index == 0 || bin_index == histogram.size() - 1) {
				bin_width *= 2;
			}

			H -= std::get<BIN_COUNT>(bin) * log(std::get<BIN_COUNT>(bin) / moving_count / bin_width) / moving_count ;
		}

		return H;
	}
	else {
		throw invalid_tag_error("invalid tag: entropy");
	}
}

template <>
statistics::result_type<statistics::tag::throughput>::type
statistics::get_impl<statistics::tag::throughput>() const
{
	if (computed(tag::throughput)) {
		auto unit = chrono::time_unit::NSEC;
		return m_data->m_moving_sum * chrono::seconds(1).count(unit) / m_data->m_moving_interval.count(unit);
	}
	else {
		throw invalid_tag_error("invalid tag: throughput");
	}
}

template <>
statistics::result_type<statistics::tag::frequency>::type
statistics::get_impl<statistics::tag::frequency>() const
{
	if (computed(tag::frequency)) {
		auto unit = chrono::time_unit::NSEC;
		return m_data->m_moving_count * chrono::seconds(1).count(unit) / m_data->m_moving_interval.count(unit);
	}
	else {
		throw invalid_tag_error("invalid tag: frequency");
	}
}

template <>
statistics::result_type<statistics::tag::timestamp>::type
statistics::get_impl<statistics::tag::timestamp>() const
{
	if (computed(tag::timestamp)) {
		return m_data->m_timestamp;
	}
	else {
		throw invalid_tag_error("invalid tag: timestamp");
	}
}


// depricated iface
statistics::value_type statistics::value() const
{
	return get<statistics::tag::value>();
}

statistics::value_type statistics::min() const
{
	return get<statistics::tag::min>();
}

statistics::value_type statistics::max() const
{
	return get<statistics::tag::max>();
}

size_t statistics::count() const
{
	return get<statistics::tag::count>();
}

statistics::value_type statistics::sum() const
{
	return get<statistics::tag::sum>();
}

double statistics::avg() const
{
	return get<statistics::tag::avg>();
}

double statistics::moving_count() const
{
	return get<statistics::tag::moving_count>();
}

double statistics::moving_sum() const
{
	return get<statistics::tag::moving_sum>();
}

double statistics::moving_avg() const
{
	return get<statistics::tag::moving_avg>();
}

statistics::histogram_type statistics::histogram() const
{
	return get<statistics::tag::histogram>();
}

double statistics::quantile(const double& probability) const
{
	return get<statistics::tag::quantile>().at(probability);
}

statistics::time_point statistics::timestamp() const
{
	return get<statistics::tag::timestamp>();
}

} // namespace handystats
