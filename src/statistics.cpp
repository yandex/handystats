// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <limits>
#include <algorithm>
#include <iterator>
#include <vector>

#include <handystats/common.h>
#include <handystats/math_utils.hpp>

#include <handystats/statistics.hpp>

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

	if (histogram.size() == 1) {
		return histogram[0].first;
	}

	double required_count = m_statistics->m_moving_count * probability;
	// estimate on left tail
	if (math_utils::cmp(histogram[0].second / 2.0, required_count) > 0) {
		double a = histogram[0].second;
		double b = 0;
		double c = - 2 * required_count;
		double z = math_utils::solve_equation(a, b, c);
		return histogram[0].first +
			histogram[0].second / (histogram[0].second + histogram[1].second) * (histogram[1].first - histogram[0].first) * (z - 1);
	}
	required_count -= histogram[0].second / 2.0;

	size_t bin_index;
	for (bin_index = 0; bin_index < histogram.size() - 1; ++bin_index) {
		double volume = (histogram[bin_index].second + histogram[bin_index + 1].second) / 2.0;
		if (math_utils::cmp(volume, required_count) > 0) {
			break;
		}
		required_count -= volume;
	}

	// estimate on right tail
	if (bin_index == histogram.size() - 1) {
		double a = - histogram[bin_index].second;
		double b = 2 * histogram[bin_index].second;
		double c = - 2 * required_count;
		double z = math_utils::solve_equation(a, b, c);
		return histogram[bin_index].first +
			histogram[bin_index].second / (histogram[bin_index - 1].second + histogram[bin_index].second) *
			(histogram[bin_index].first - histogram[bin_index - 1].first) * z;
	}

	double a = histogram[bin_index + 1].second - histogram[bin_index].second;
	double b = 2 * histogram[bin_index].second;
	double c = - 2 * required_count;
	double z = math_utils::solve_equation(a, b, c);
	return histogram[bin_index].first + (histogram[bin_index + 1].first - histogram[bin_index].first) * z;
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

	throw invalid_tag_error();
}

bool statistics::enabled(const statistics::tag::type& t) const HANDYSTATS_NOEXCEPT {
	return m_tags & t;
}

bool statistics::computed(const statistics::tag::type& t) const HANDYSTATS_NOEXCEPT {
	switch (t) {
	case tag::value:
		return enabled(tag::value);

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
		return enabled(tag::moving_count) || computed(tag::moving_avg) || computed(tag::quantile);

	case tag::moving_sum:
		return enabled(tag::moving_sum) || computed(tag::moving_avg);

	case tag::moving_avg:
		return enabled(tag::moving_avg);

	case tag::histogram:
		return enabled(tag::histogram) || computed(tag::quantile);

	case tag::quantile:
		return enabled(tag::quantile);

	case tag::timestamp:
		return enabled(tag::timestamp) ||
			computed(tag::moving_count) || computed(tag::moving_sum) || computed(tag::moving_avg) ||
			computed(tag::histogram) || computed(tag::quantile);

	default:
		return false;
	};
}

statistics::tag::type statistics::tags() const HANDYSTATS_NOEXCEPT {
	return m_tags;
}

statistics::statistics(
			const config::statistics& opts
		)
	: m_moving_interval(opts.moving_interval)
	, m_histogram_bins(opts.histogram_bins)
	, m_tags(opts.tags)
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
	if (m_histogram_bins > 0) {
		m_histogram.reserve(m_histogram_bins + 1);
	}
	m_timestamp = time_point();
}

static double bin_merge_criteria(
		const statistics::bin_type& left_bin,
		const statistics::bin_type& right_bin
	)
{
	// possible variants:
	// * distance of bins' centers
	// * sum of bins' weights (number of elements)
	// * other heuristics

	// distance of bins' centers
	// return right_bin.first - left_bin.first;

	// sum of bins' weights
	// return left_bin.second + right_bin.second;

	// heuristic
	return (left_bin.second + right_bin.second) * (right_bin.first - left_bin.first);
}

static void update_histogram(
		statistics::histogram_type& histogram,
		const size_t& histogram_bins,
		const statistics::value_type& value
	)
{
	if (histogram_bins == 0) {
		// histogram is disabled
		return;
	}

	statistics::bin_type new_bin(value, 1.0);

	auto insert_iter = std::lower_bound(histogram.begin(), histogram.end(), new_bin);
	histogram.insert(insert_iter, new_bin);

	if (histogram.size() <= histogram_bins) {
		return;
	}

	size_t best_merge_index = -1;
	double best_merge_criteria = 0;
	for (size_t index = 0; index < histogram.size() - 1; ++index) {
		double merge_criteria = bin_merge_criteria(histogram[index], histogram[index + 1]);
		if (best_merge_index == -1 || math_utils::cmp(merge_criteria, best_merge_criteria) < 0) {
			best_merge_index = index;
			best_merge_criteria = merge_criteria;
		}
	}

	histogram[best_merge_index].first =
		histogram[best_merge_index].first * histogram[best_merge_index].second +
		histogram[best_merge_index + 1].first * histogram[best_merge_index + 1].second;

	histogram[best_merge_index].second += histogram[best_merge_index + 1].second;

	histogram[best_merge_index].first /= histogram[best_merge_index].second;

	histogram.erase(histogram.begin() + best_merge_index + 1);
}

void statistics::update(const value_type& value, const time_point& timestamp) {
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
		duration elapsed_duration = timestamp - m_timestamp;
		if (elapsed_duration > m_moving_interval) {
			m_moving_count = 1;
		}
		else if (elapsed_duration.count() > 0) {
			double elapsed = double(elapsed_duration.count()) / m_moving_interval.count();
			m_moving_count = m_moving_count * (1.0 - elapsed) + 1;
		}
		else if (elapsed_duration > -m_moving_interval) {
			m_moving_count += 1;
		}
	}

	if (computed(tag::moving_sum)) {
		duration elapsed_duration = timestamp - m_timestamp;
		if (elapsed_duration > m_moving_interval) {
			m_moving_sum = value;
		}
		else if (elapsed_duration.count() > 0) {
			double elapsed = double(elapsed_duration.count()) / m_moving_interval.count();
			m_moving_sum = m_moving_sum * (1.0 - elapsed) + value;
		}
		else if (elapsed_duration > -m_moving_interval) {
			m_moving_sum += value;
		}
	}

	if (computed(tag::histogram)) {
		duration elapsed_duration = timestamp - m_timestamp;
		if (elapsed_duration > m_moving_interval) {
			m_histogram.clear();
			update_histogram(m_histogram, m_histogram_bins, value);
		}
		else if (elapsed_duration.count() > 0) {
			double elapsed = double(elapsed_duration.count()) / m_moving_interval.count();
			for (auto bin = m_histogram.begin(); bin != m_histogram.end(); ++bin) {
				bin->second *= (1.0 - elapsed);
			}
			update_histogram(m_histogram, m_histogram_bins, value);
		}
		else if (elapsed_duration > -m_moving_interval) {
			update_histogram(m_histogram, m_histogram_bins, value);
		}
	}

	if (computed(tag::timestamp)) {
		if (m_timestamp < timestamp) {
			m_timestamp = timestamp;
		}
	}
}

void statistics::update_time(const time_point& timestamp) {
	if (timestamp <= m_timestamp) {
		return;
	}

	if (computed(tag::moving_count)) {
		duration elapsed_duration = timestamp - m_timestamp;
		if (elapsed_duration > m_moving_interval) {
			m_moving_count = 0;
		}
		else if (elapsed_duration.count() > 0) {
			double elapsed = double(elapsed_duration.count()) / m_moving_interval.count();
			m_moving_count = m_moving_count * (1.0 - elapsed);
		}
	}

	if (computed(tag::moving_sum)) {
		duration elapsed_duration = timestamp - m_timestamp;
		if (elapsed_duration > m_moving_interval) {
			m_moving_sum = 0;
		}
		else if (elapsed_duration.count() > 0) {
			double elapsed = double(elapsed_duration.count()) / m_moving_interval.count();
			m_moving_sum = m_moving_sum * (1.0 - elapsed);
		}
	}

	if (computed(tag::histogram)) {
		duration elapsed_duration = timestamp - m_timestamp;
		if (elapsed_duration > m_moving_interval) {
			m_histogram.clear();
		}
		else if (elapsed_duration.count() > 0) {
			double elapsed = double(elapsed_duration.count()) / m_moving_interval.count();
			for (auto bin = m_histogram.begin(); bin != m_histogram.end(); ++bin) {
				bin->second *= (1.0 - elapsed);
			}
		}
	}

	if (computed(tag::timestamp)) {
		m_timestamp = timestamp;
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
