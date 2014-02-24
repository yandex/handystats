#ifndef HANDYSTATS_METRICS_TIMER_H_
#define HANDYSTATS_METRICS_TIMER_H_

#include <utility>

#include <handystats/chrono_impl.hpp>

namespace handystats { namespace metrics {

class timer
{
public:

	typedef chrono::default_duration value_type;
	typedef chrono::default_duration time_duration;
	typedef typename chrono::default_clock clock;
	typedef typename chrono::default_time_point time_point;

	timer();
	timer(time_point start_timestamp);

	void start(time_point timestamp);
	void stop(time_point timestamp);

	std::pair<value_type, time_point> get() const;

private:
	time_point timestamp;
	value_type value;

}; // class timer

}} // namespace handystats::metrics

#endif // HANDYSTATS_METRICS_TIMER_H_
