#ifndef HANDYSTATS_METRICS_COUNTER_H_
#define HANDYSTATS_METRICS_COUNTER_H_

#include <utility>

#include <handystats/chrono.hpp>
#include <handystats/metrics/gauge.hpp>


namespace handystats { namespace metrics {

struct counter
{
	typedef int64_t value_type;
	typedef chrono::default_duration time_duration;
	typedef chrono::default_clock clock;
	typedef chrono::default_time_point time_point;

	struct internal_stats {
		gauge values;

		gauge incr_deltas;
		gauge decr_deltas;
		gauge deltas;

		internal_stats()
		{}

		void update_value(value_type value, time_point timestamp);
		void update_increment(value_type delta, time_point timestamp);
		void update_decrement(value_type delta, time_point timestamp);
	};

	counter();
	counter(value_type value, time_point timestamp);

	void increment(value_type value, time_point timestamp);
	void decrement(value_type value, time_point timestamp);

	value_type value;
	time_point timestamp;

	internal_stats stats;

}; // struct counter

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_COUNTER_H_
