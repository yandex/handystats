#ifndef HANDYSTATS_METRICS_TIMER_H_
#define HANDYSTATS_METRICS_TIMER_H_

#include <handystats/chrono.hpp>

namespace handystats { namespace metrics {

class timer {
public:

	typedef std::chrono::nanoseconds time_duration;
	typedef typename chrono::steady_clock<time_duration> clock;
	typedef typename clock::time_point time_point;

	struct internal_stats {
		time_duration duration;

		static void initialize(internal_stats& stats);
		static void update(internal_stats& stats, time_duration timer_time);
	};

	timer();
	timer(time_point start_timestamp);

	void start(time_point timestamp);
	void finish(time_point timestamp);

	internal_stats get_stats() const;

private:
	time_point start_timestamp;
	time_point finish_timestamp;

	internal_stats stats;

}; // class timer

}} // namespace handystats::metrics

#endif // HANDYSTATS_METRICS_TIMER_H_
