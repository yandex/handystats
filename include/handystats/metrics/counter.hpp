#ifndef HANDYSTATS_METRICS_COUNTER_H_
#define HANDYSTATS_METRICS_COUNTER_H_

#include <utility>

#include <handystats/chrono.hpp>
#include <handystats/accumulators.hpp>

#include <handystats/metrics/gauge.hpp>

namespace handystats { namespace metrics {

class counter
{
public:

	typedef int64_t value_type;
	typedef typename chrono::default_duration time_duration;
	typedef typename chrono::default_clock clock;
	typedef typename chrono::default_time_point time_point;

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

	std::pair<value_type, time_point> get() const;

	internal_stats get_stats() const;

private:
	value_type value;
	time_point timestamp;

	internal_stats stats;

}; // class counter

}} // namespace handystats::metrics

#endif // HANDYSTATS_METRICS_COUNTER_H_
