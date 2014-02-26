#ifndef HANDYSTATS_METRICS_TIMER_H_
#define HANDYSTATS_METRICS_TIMER_H_

#include <utility>

#include <handystats/chrono.hpp>


namespace handystats { namespace metrics {

struct timer
{
	typedef chrono::default_clock clock;
	typedef typename clock::duration value_type;
	typedef chrono::default_time_point time_point;

	timer();
	timer(time_point start_timestamp);

	void start(time_point timestamp);
	void stop(time_point timestamp);

	time_point timestamp;
	value_type value;

}; // struct timer

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_TIMER_H_
