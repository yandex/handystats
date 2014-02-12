#ifndef HANDYSTATS_METRICS_TIMER_H_
#define HANDYSTATS_METRICS_TIMER_H_

#include <utility>

#include <handystats/chrono.hpp>

namespace handystats { namespace metrics {

class timer
{
public:

	typedef chrono::default_duration value_type;
	typedef chrono::default_duration time_duration;
	typedef typename chrono::steady_clock<time_duration> clock;
	typedef typename clock::time_point time_point;

	timer();
	timer(time_point start_timestamp);

	void start(time_point timestamp);
	void finish(time_point timestamp);

	std::pair<value_type, time_point> get() const;

private:
	time_point timestamp;
	value_type duration;

}; // class timer

}} // namespace handystats::metrics

#endif // HANDYSTATS_METRICS_TIMER_H_
