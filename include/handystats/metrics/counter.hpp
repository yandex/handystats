// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_METRICS_COUNTER_H_
#define HANDYSTATS_METRICS_COUNTER_H_

#include <utility>

#include <handystats/chrono.hpp>
#include <handystats/incremental_statistics.hpp>


namespace handystats { namespace metrics {

struct counter
{
	typedef int64_t value_type;
	typedef chrono::time_duration time_duration;
	typedef chrono::clock clock;
	typedef clock::time_point time_point;

	counter(value_type value = value_type(), time_point timestamp = clock::now());

	void increment(value_type value = 1, time_point timestamp = clock::now());
	void decrement(value_type value = 1, time_point timestamp = clock::now());

	value_type value;
	time_point timestamp;

	incremental_statistics values;
	incremental_statistics deltas;
	incremental_statistics incr_deltas;
	incremental_statistics decr_deltas;

}; // struct counter

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_COUNTER_H_
