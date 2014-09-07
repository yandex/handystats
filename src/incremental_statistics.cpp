// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <limits>
#include <algorithm>
#include <iterator>

#include <handystats/math_utils.hpp>

#include <handystats/incremental_statistics.hpp>

namespace handystats {

incremental_statistics::incremental_statistics(
			const config::incremental_statistics& opts
		)
	: m_moving_interval(chrono::duration_cast<duration>(opts.moving_interval))
	, m_moving_average_alpha(opts.moving_average_alpha)
	, m_histogram_bins(opts.histogram_bins)
{
	clear();
}

void incremental_statistics::clear() {
	m_min = std::numeric_limits<value_type>::max();
	m_max = std::numeric_limits<value_type>::min();
	m_sum = 0;
	m_count = 0;
	m_interval_count = 0;
	m_interval_sum = 0;
	m_moving_average = 0;
	m_histogram.clear();
	m_histogram.reserve(m_histogram_bins);
	m_timestamp = time_point();
}

static double bin_merge_criteria(
		const incremental_statistics::bin_type& left_bin,
		const incremental_statistics::bin_type& right_bin
	)
{
	// possible variants:
	// * distance of bins' centers
	// * sum of bins' weights (number of elements)

	// distance of bins' centers
	// return right_bin.first - left_bin.first;

	// sum of bins' weights
	return left_bin.second + right_bin.second;
}

static void update_histogram(
		incremental_statistics::histogram_type& histogram,
		const size_t& histogram_bins,
		const incremental_statistics::value_type& value
	)
{
	if (histogram_bins == 0) {
		return; // histogram is disabled
	}

	incremental_statistics::bin_type new_bin(value, 1.0);

	auto insert_iter = std::lower_bound(histogram.begin(), histogram.end(), new_bin);
	histogram.insert(insert_iter, new_bin);

	if (histogram.size() < histogram_bins) {
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

void incremental_statistics::operator() (const value_type& value, const time_point& timestamp) {
	m_min = std::min(m_min, value);

	m_max = std::max(m_max, value);

	m_sum += value;

	++m_count;

	duration elapsed_duration = timestamp - m_timestamp;
	if (elapsed_duration > m_moving_interval) {
		m_interval_count = 1;
		m_interval_sum = value;

		m_histogram.clear();
		update_histogram(m_histogram, m_histogram_bins, value);
	}
	else if (elapsed_duration.count() > 0) {
		double elapsed = double(elapsed_duration.count()) / m_moving_interval.count();
		m_interval_count = m_interval_count * (1.0 - elapsed) + 1;
		m_interval_sum = m_interval_sum * (1.0 - elapsed) + value;

		for (auto bin = m_histogram.begin(); bin != m_histogram.end(); ++bin) {
			bin->second *= (1.0 - elapsed);
		}
		update_histogram(m_histogram, m_histogram_bins, value);
	}
	else if (elapsed_duration > -m_moving_interval) {
		m_interval_count += 1;
		m_interval_sum += value;

		update_histogram(m_histogram, m_histogram_bins, value);
	}

	if (m_count == 1) {
		m_moving_average = value;
	}
	else {
		m_moving_average += (value - m_moving_average) * m_moving_average_alpha;
	}

	if (elapsed_duration.count() > 0) {
		m_timestamp = timestamp;
	}
}


incremental_statistics::value_type incremental_statistics::min() const {
	return m_min;
}
incremental_statistics::value_type incremental_statistics::max() const {
	return m_max;
}
incremental_statistics::value_type incremental_statistics::sum() const {
	return m_sum;
}
size_t incremental_statistics::count() const {
	return m_count;
}
incremental_statistics::value_type incremental_statistics::mean() const {
	return m_count > 0 ? m_sum / m_count : 0;
}
incremental_statistics::value_type incremental_statistics::interval_count() const {
	return m_interval_count;
}
incremental_statistics::value_type incremental_statistics::interval_sum() const {
	return m_interval_sum;
}
incremental_statistics::value_type incremental_statistics::interval_mean() const {
	return math_utils::cmp<value_type>(m_interval_count, 0) <= 0 ? 0 : m_interval_sum / m_interval_count;
}
incremental_statistics::value_type incremental_statistics::moving_average() const {
	return m_moving_average;
}
incremental_statistics::histogram_type incremental_statistics::histogram() const {
	return m_histogram;
}
incremental_statistics::value_type incremental_statistics::quantile(const double& probability) const {
	if (m_histogram_bins == 0 || m_histogram.size() == 0) {
		return std::numeric_limits<value_type>::min();
	}

	if (m_histogram.size() == 1) {
		return m_histogram[0].first;
	}

	double required_count = interval_count() * probability;

	// estimate on left tail
	if (math_utils::cmp(m_histogram[0].second / 2.0, required_count) > 0) {
		double a = m_histogram[0].second;
		double b = 0;
		double c = - 2 * required_count;
		double z = math_utils::solve_equation(a, b, c);
		return m_histogram[0].first +
			m_histogram[0].second / (m_histogram[0].second + m_histogram[1].second) * (m_histogram[1].first - m_histogram[0].first) * (z - 1);
	}

	required_count -= m_histogram[0].second / 2.0;
	size_t bin_index;
	for (bin_index = 0; bin_index < m_histogram.size() - 1; ++bin_index) {
		double volume = (m_histogram[bin_index].second + m_histogram[bin_index + 1].second) / 2.0;
		if (math_utils::cmp(volume, required_count) > 0) {
			break;
		}
		required_count -= volume;
	}

	// estimate on right tail
	if (bin_index == m_histogram.size() - 1) {
		double a = - m_histogram[bin_index].second;
		double b = 2 * m_histogram[bin_index].second;
		double c = - 2 * required_count;
		double z = math_utils::solve_equation(a, b, c);
		return m_histogram[bin_index].first +
			m_histogram[bin_index].second / (m_histogram[bin_index - 1].second + m_histogram[bin_index].second) *
				(m_histogram[bin_index].first - m_histogram[bin_index - 1].first) * z;
	}

	double a = m_histogram[bin_index + 1].second - m_histogram[bin_index].second;
	double b = 2 * m_histogram[bin_index].second;
	double c = - 2 * required_count;
	double z = math_utils::solve_equation(a, b, c);
	return m_histogram[bin_index].first + (m_histogram[bin_index + 1].first - m_histogram[bin_index].first) * z;
}
incremental_statistics::time_point incremental_statistics::timestamp() const {
	return m_timestamp;
}

} // namespace handystats
