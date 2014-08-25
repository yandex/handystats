// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <limits>
#include <algorithm>

#include <handystats/math_utils.hpp>

#include "config_impl.hpp"

#include <handystats/incremental_statistics.hpp>

namespace handystats {

incremental_statistics::incremental_statistics(
			const chrono::time_duration& moving_interval,
			const double& moving_average_alpha
		)
	: m_moving_interval(chrono::duration_cast<duration>(moving_interval))
	, m_moving_average_alpha(moving_average_alpha)
	, m_min(std::numeric_limits<value_type>::max())
	, m_max(std::numeric_limits<value_type>::min())
	, m_sum(0)
	, m_count(0)
	, m_interval_count(0)
	, m_interval_sum(0)
	, m_moving_average(0)
	, m_timestamp()
{
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
	}
	else if (elapsed_duration.count() > 0) {
		double elapsed = double(elapsed_duration.count()) / m_moving_interval.count();
		m_interval_count = m_interval_count * (1.0 - elapsed) + 1;
		m_interval_sum = m_interval_sum * (1.0 - elapsed) + value;
	}
	else if (elapsed_duration > -m_moving_interval) {
		m_interval_count += 1;
		m_interval_sum += value;
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
incremental_statistics::time_point incremental_statistics::timestamp() const {
	return m_timestamp;
}

} // namespace handystats
