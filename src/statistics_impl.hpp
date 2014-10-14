// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_STATISTICS_IMPL_HPP_
#define HANDYSTATS_STATISTICS_IMPL_HPP_

#include <string>
#include <map>

#include <handystats/statistics.hpp>
#include <handystats/config/statistics.hpp>

namespace handystats {

struct statistics::data {
	data(const config::statistics& config);

	// applicable for moving_sum, moving_count, etc.
	double shift_interval_data(
			const double& data, const time_point& data_timestamp,
			const time_point& timestamp
		)
		const;
	double truncate_interval_data(
			const double& data, const time_point& data_timestamp,
			const time_point& timestamp
		)
		const;
	double update_interval_data(
			const double& data, const time_point& data_timestamp,
			const value_type& value, const time_point& timestamp
		)
		const;

	// histogram (in-place)
	void shift_histogram(const time_point& timestamp);
	void truncate_histogram(const time_point& timestamp);
	void update_histogram(const value_type& value, const time_point& timestamp);

	void reset();

	void update(const value_type& value, const time_point& timestamp = clock::now());
	void update_time(const time_point& timestamp = clock::now());
	void truncate_time(const time_point& timestamp);

	void append(data);

	void fulfill_dependencies();

	value_type m_value;
	value_type m_min;
	value_type m_max;
	value_type m_sum;
	size_t m_count;
	value_type m_moving_count;
	value_type m_moving_sum;
	histogram_type m_histogram;
	time_point m_data_timestamp;
	value_type m_rate;

	time_point m_timestamp;

	// configuration
	chrono::duration m_moving_interval;
	size_t m_histogram_bins;
	int m_tags; // computed tags
};

typedef std::map<std::string, statistics::data> stats_data_map;

} // namespace handystats

#endif // HANDYSTATS_STATISTICS_IMPL_HPP_
