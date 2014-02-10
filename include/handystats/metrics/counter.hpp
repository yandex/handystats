#ifndef HANDYSTATS_METRICS_COUNTER_H_
#define HANDYSTATS_METRICS_COUNTER_H_

#include <utility>

#include <handystats/chrono.hpp>

namespace handystats { namespace metrics {

class counter
{
public:

	typedef long long value_type;
	typedef typename chrono::default_duration time_duration;
	typedef typename chrono::steady_clock<time_duration> clock;
	typedef typename clock::time_point time_point;

	struct internal_stats {
		value_type min_value;
		time_point min_value_timestamp;

		value_type max_value;
		time_point max_value_timestamp;

		size_t count;

		static void initialize(internal_stats& stats);
		static void initialize(internal_stats& stats, value_type value, time_point timestamp);
		static void update(internal_stats& stats, value_type value, time_point timestamp);
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
