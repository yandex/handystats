#ifndef HANDYSTATS_METRICS_COUNTER_H_
#define HANDYSTATS_METRICS_COUNTER_H_

#include <utility>

#include <handystats/chrono.hpp>
#include <handystats/incremental_statistics.hpp>


namespace handystats { namespace metrics {

struct counter
{
	typedef int64_t value_type;
	typedef chrono::default_duration time_duration;
	typedef chrono::default_clock clock;
	typedef chrono::default_time_point time_point;

	counter();
	counter(value_type value, time_point timestamp);

	void increment(value_type value, time_point timestamp);
	void decrement(value_type value, time_point timestamp);

	value_type value;
	time_point timestamp;

	incremental_statistics values;
	incremental_statistics deltas;
	incremental_statistics incr_deltas;
	incremental_statistics decr_deltas;

}; // struct counter

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_COUNTER_H_
