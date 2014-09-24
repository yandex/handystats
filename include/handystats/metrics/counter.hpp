// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_METRICS_COUNTER_HPP_
#define HANDYSTATS_METRICS_COUNTER_HPP_

#include <cstdint>

#include <utility>

#include <handystats/chrono.hpp>
#include <handystats/statistics.hpp>
#include <handystats/config/metrics/counter.hpp>

namespace handystats { namespace metrics {

struct counter
{
	typedef int64_t value_type;
	typedef chrono::clock clock;
	typedef clock::time_point time_point;

	counter(const config::metrics::counter& opts = config::metrics::counter());

	void init(const value_type& value = 0, const time_point& timestamp = clock::now());
	void increment(const value_type& value = 1, const time_point& timestamp = clock::now());
	void decrement(const value_type& value = 1, const time_point& timestamp = clock::now());

	void update_statistics(const time_point& timestamp = clock::now());

	const statistics& values() const;

private:
	statistics m_values;

	value_type m_value;
	time_point m_timestamp;

}; // struct counter

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_COUNTER_HPP_
