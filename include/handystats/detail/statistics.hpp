// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_STATISTICS_IMPL_HPP_
#define HANDYSTATS_STATISTICS_IMPL_HPP_

#include <string>
#include <map>

#include <handystats/statistics.hpp>
#include <handystats/config/statistics.hpp>

namespace handystats {

struct statistics::data {
	data();
	data(const config::statistics& config);

	void reset();

	void update(const value_type& value, const time_point& timestamp = clock::now());
	void update_time(const time_point& timestamp = clock::now());

	void append(data);
	void merge(const data&);

	void truncate_before(const time_point& timestamp);
	void truncate_after(const time_point& timestamp);

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
