// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <cstring>
#include <limits>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cmath>

#include <handystats/common.h>
#include <handystats/math_utils.hpp>

#include <handystats/statistics.hpp>

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

statistics::quantile_extractor::quantile_extractor(const statistics* const statistics)
	: m_statistics(statistics)
{}

double statistics::quantile_extractor::at(const double& probability) const {
	if (m_statistics == nullptr) {
		return 0;
	}

	const auto& histogram = m_statistics->m_histogram;

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
	for (; bin_index < (int)histogram.size(); ++bin_index) {
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
			time_point()
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
			time_point()
		};
	}

	const double& a = std::get<BIN_COUNT>(right_bin) - std::get<BIN_COUNT>(left_bin);
	const double& b = 2 * std::get<BIN_COUNT>(left_bin);
	const double& c = -2 * required_count;

	const double& z = find_z(a, b, c);

	return std::get<BIN_CENTER>(left_bin) + (std::get<BIN_CENTER>(right_bin) - std::get<BIN_CENTER>(left_bin)) * z;
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
const statistics::tag::type statistics::tag::timestamp;
const statistics::tag::type statistics::tag::rate;
const statistics::tag::type statistics::tag::entropy;

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
	if (strcmp("timestamp", tag_name.c_str()) == 0) {
		return timestamp;
	}
	if (strcmp("rate", tag_name.c_str()) == 0) {
		return rate;
	}
	if (strcmp("entropy", tag_name.c_str()) == 0) {
		return entropy;
	}

	throw invalid_tag_error();
}

bool statistics::enabled(const statistics::tag::type& t) const HANDYSTATS_NOEXCEPT {
	return m_config.tags & t;
}

bool statistics::computed(const statistics::tag::type& t) const HANDYSTATS_NOEXCEPT {
	switch (t) {
	case tag::value:
		return enabled(tag::value) || computed(tag::rate);

	case tag::min:
		return enabled(tag::min);

	case tag::max:
		return enabled(tag::max);

	case tag::count:
		return enabled(tag::count) || computed(tag::avg);

	case tag::sum:
		return enabled(tag::sum) || computed(tag::avg);

	case tag::avg:
		return enabled(tag::avg);

	case tag::moving_count:
		return enabled(tag::moving_count) || computed(tag::moving_avg);

	case tag::moving_sum:
		return enabled(tag::moving_sum) || computed(tag::moving_avg);

	case tag::moving_avg:
		return enabled(tag::moving_avg);

	case tag::histogram:
		return enabled(tag::histogram) || computed(tag::quantile) || computed(tag::entropy);

	case tag::quantile:
		return enabled(tag::quantile);

	case tag::timestamp:
		return enabled(tag::timestamp) ||
			computed(tag::moving_count) || computed(tag::moving_sum) || computed(tag::moving_avg) ||
			computed(tag::histogram) || computed(tag::quantile) ||
			computed(tag::rate);

	case tag::rate:
		return enabled(tag::rate);

	case tag::entropy:
		return enabled(tag::entropy);

	default:
		return false;
	};
}

statistics::tag::type statistics::tags() const HANDYSTATS_NOEXCEPT {
	return m_config.tags;
}

statistics::statistics(
			const config::statistics& opts
		)
	: m_config(opts)
{
	reset();
}

void statistics::reset() {
	m_value = value_type(0);
	m_min = std::numeric_limits<value_type>::max();
	m_max = std::numeric_limits<value_type>::min();
	m_sum = value_type(0);
	m_count = 0;
	m_moving_count = 0.0;
	m_moving_sum = 0.0;
	m_histogram.clear();
	if (m_config.histogram_bins > 0) {
		m_histogram.reserve(m_config.histogram_bins + 1);
	}
	m_timestamp = time_point();
	m_rate = 0;

	m_data_timestamp = time_point();
}

double statistics::shift_interval_data(
		const double& data, const statistics::time_point& data_timestamp,
		const statistics::time_point& timestamp
	)
{
	if (timestamp <= m_timestamp) return data;

	const auto& stale_interval = data_timestamp - (timestamp - m_config.moving_interval);

	if (stale_interval.count() <= 0) return 0;

	return data * stale_interval.count() / (m_config.moving_interval - (m_timestamp - data_timestamp)).count();
}

double statistics::update_interval_data(
		const double& data, const statistics::time_point& data_timestamp,
		const statistics::value_type& value, const statistics::time_point& timestamp
	)
{
	if (timestamp <= m_timestamp) {
		if (timestamp < m_timestamp - m_config.moving_interval) {
			return data;
		}
		else {
			return data + value;
		}
	}
	else {
		return value + shift_interval_data(data, data_timestamp, timestamp);
	}

	return data;
}


void statistics::shift_histogram(const statistics::time_point& timestamp) {
	if (m_config.histogram_bins == 0) return;

	for (auto bin = m_histogram.begin(); bin != m_histogram.end(); ++bin) {
		auto& bin_count = std::get<BIN_COUNT>(*bin);
		const auto& bin_timestamp = std::get<BIN_TIMESTAMP>(*bin);

		bin_count = shift_interval_data(bin_count, bin_timestamp, timestamp);
	}
}

static double bin_merge_criteria(
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

void statistics::update_histogram(const statistics::value_type& value, const statistics::time_point& timestamp)
{
	if (m_config.histogram_bins == 0) return;

	statistics::bin_type new_bin(value, 1.0, timestamp);

	auto insert_iter = std::lower_bound(m_histogram.begin(), m_histogram.end(), new_bin);
	m_histogram.insert(insert_iter, new_bin);

	shift_histogram(timestamp);

	if (m_histogram.size() <= m_config.histogram_bins) {
		return;
	}

	size_t best_merge_index = -1;
	double best_merge_criteria = 0;
	for (size_t index = 0; index < m_histogram.size() - 1; ++index) {
		double merge_criteria = bin_merge_criteria(m_histogram[index], m_histogram[index + 1]);
		if (best_merge_index == -1 || math_utils::cmp(merge_criteria, best_merge_criteria) < 0) {
			best_merge_index = index;
			best_merge_criteria = merge_criteria;
		}
	}

	auto& left_bin = m_histogram[best_merge_index];
	auto& right_bin = m_histogram[best_merge_index + 1];

	if (math_utils::cmp(std::get<BIN_COUNT>(left_bin), 0.0) <= 0 &&
			math_utils::cmp(std::get<BIN_COUNT>(right_bin), 0.0) <= 0)
	{
		std::get<BIN_CENTER>(left_bin) =
			math_utils::weighted_average(
					std::get<BIN_CENTER>(left_bin), 1,
					std::get<BIN_CENTER>(right_bin), 1
				);

		std::get<BIN_COUNT>(left_bin) = 0;

		std::get<BIN_TIMESTAMP>(left_bin) = time_point();
	}
	else {
		std::get<BIN_CENTER>(left_bin) =
			math_utils::weighted_average(
					std::get<BIN_CENTER>(left_bin), std::get<BIN_COUNT>(left_bin),
					std::get<BIN_CENTER>(right_bin), std::get<BIN_COUNT>(right_bin)
				);

		std::get<BIN_COUNT>(left_bin) += std::get<BIN_COUNT>(right_bin);

		std::get<BIN_TIMESTAMP>(left_bin) = std::max(std::get<BIN_TIMESTAMP>(left_bin), std::get<BIN_TIMESTAMP>(right_bin));
	}

	m_histogram.erase(m_histogram.begin() + best_merge_index + 1);
}

void statistics::update(const value_type& value, const time_point& timestamp) {
	if (computed(tag::rate)) {
		const value_type delta = value - m_value;
		m_rate = update_interval_data(m_rate, m_data_timestamp, delta, timestamp);
	}

	if (computed(tag::value)) {
		m_value = value;
	}

	if (computed(tag::min)) {
		m_min = std::min(m_min, value);
	}

	if (computed(tag::max)) {
		m_max = std::max(m_max, value);
	}

	if (computed(tag::sum)) {
		m_sum += value;
	}

	if (computed(tag::count)) {
		++m_count;
	}

	if (computed(tag::moving_count)) {
		m_moving_count = update_interval_data(m_moving_count, m_data_timestamp, 1, timestamp);
	}

	if (computed(tag::moving_sum)) {
		m_moving_sum = update_interval_data(m_moving_sum, m_data_timestamp, value, timestamp);
	}

	if (computed(tag::histogram)) {
		update_histogram(value, timestamp);
	}

	if (computed(tag::timestamp)) {
		m_timestamp = std::max(m_timestamp, timestamp);

		m_data_timestamp = std::max(m_data_timestamp, timestamp);
	}
}

void statistics::update_time(const time_point& timestamp) {
	if (timestamp <= m_timestamp) return;

	if (computed(tag::rate)) {
		m_rate = shift_interval_data(m_rate, m_data_timestamp, timestamp);
	}

	if (computed(tag::moving_count)) {
		m_moving_count = shift_interval_data(m_moving_count, m_data_timestamp, timestamp);
	}

	if (computed(tag::moving_sum)) {
		m_moving_sum = shift_interval_data(m_moving_sum, m_data_timestamp, timestamp);
	}

	if (computed(tag::histogram)) {
		shift_histogram(timestamp);
	}

	if (computed(tag::timestamp)) {
		m_timestamp = std::max(m_timestamp, timestamp);
	}
}

// get_impl
template <>
statistics::result_type<statistics::tag::value>::type
statistics::get_impl<statistics::tag::value>() const
{
	if (computed(tag::value)) {
		return m_value;
	}
	else {
		throw invalid_tag_error();
	}
}

template <>
statistics::result_type<statistics::tag::min>::type
statistics::get_impl<statistics::tag::min>() const
{
	if (computed(tag::min)) {
		return m_min;
	}
	else {
		throw invalid_tag_error();
	}
}

template <>
statistics::result_type<statistics::tag::max>::type
statistics::get_impl<statistics::tag::max>() const
{
	if (computed(tag::max)) {
		return m_max;
	}
	else {
		throw invalid_tag_error();
	}
}

template <>
statistics::result_type<statistics::tag::count>::type
statistics::get_impl<statistics::tag::count>() const
{
	if (computed(tag::count)) {
		return m_count;
	}
	else {
		throw invalid_tag_error();
	}
}

template <>
statistics::result_type<statistics::tag::sum>::type
statistics::get_impl<statistics::tag::sum>() const
{
	if (computed(tag::sum)) {
		return m_sum;
	}
	else {
		throw invalid_tag_error();
	}
}

template <>
statistics::result_type<statistics::tag::avg>::type
statistics::get_impl<statistics::tag::avg>() const
{
	if (computed(tag::avg)) {
		if (m_count == 0) {
			return 0;
		}
		else {
			return result_type<tag::avg>::type(m_sum) / m_count;
		}
	}
	else {
		throw invalid_tag_error();
	}
}

template <>
statistics::result_type<statistics::tag::moving_count>::type
statistics::get_impl<statistics::tag::moving_count>() const
{
	if (computed(tag::moving_count)) {
		return m_moving_count;
	}
	else {
		throw invalid_tag_error();
	}
}

template <>
statistics::result_type<statistics::tag::moving_sum>::type
statistics::get_impl<statistics::tag::moving_sum>() const
{
	if (computed(tag::moving_sum)) {
		return m_moving_sum;
	}
	else {
		throw invalid_tag_error();
	}
}

template <>
statistics::result_type<statistics::tag::moving_avg>::type
statistics::get_impl<statistics::tag::moving_avg>() const
{
	if (computed(tag::moving_avg)) {
		if (math_utils::cmp<result_type<tag::moving_count>::type>(m_moving_count, 0) <= 0) {
			return 0;
		}
		else {
			return result_type<tag::moving_avg>::type(m_moving_sum) / m_moving_count;
		}
	}
	else {
		throw invalid_tag_error();
	}
}

template <>
statistics::result_type<statistics::tag::histogram>::type
statistics::get_impl<statistics::tag::histogram>() const
{
	if (computed(tag::histogram)) {
		return m_histogram;
	}
	else {
		throw invalid_tag_error();
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
		throw invalid_tag_error();
	}
}

template <>
statistics::result_type<statistics::tag::timestamp>::type
statistics::get_impl<statistics::tag::timestamp>() const
{
	if (computed(tag::timestamp)) {
		return m_timestamp;
	}
	else {
		throw invalid_tag_error();
	}
}

template <>
statistics::result_type<statistics::tag::rate>::type
statistics::get_impl<statistics::tag::rate>() const
{
	if (computed(tag::rate)) {
		if (std::less<chrono::time_unit>()(m_config.rate_unit, m_config.moving_interval.unit())) {
			const double& rate_factor =
				chrono::duration::convert_to(m_config.rate_unit, m_config.moving_interval).count();
			return double(m_rate) / rate_factor;
		}
		else {
			const double& rate_factor =
				chrono::duration::convert_to(m_config.moving_interval.unit(),
						chrono::duration(1, m_config.rate_unit)
					).count();
			return double(m_rate) * rate_factor / m_config.moving_interval.count();
		}
	}
	else {
		throw invalid_tag_error();
	}
}

template <>
statistics::result_type<statistics::tag::entropy>::type
statistics::get_impl<statistics::tag::entropy>() const
{
	if (computed(tag::entropy)) {
		const auto& histogram = m_histogram;

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
		throw invalid_tag_error();
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
