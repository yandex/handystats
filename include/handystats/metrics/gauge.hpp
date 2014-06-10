#ifndef HANDYSTATS_METRICS_GAUGE_H_
#define HANDYSTATS_METRICS_GAUGE_H_

#include <utility>

#include <handystats/chrono.hpp>
#include <handystats/incremental_statistics.hpp>

namespace handystats { namespace metrics {

struct gauge
{
	typedef double value_type;
	typedef chrono::default_duration time_duration;
	typedef chrono::default_clock clock;
	typedef chrono::default_time_point time_point;

	gauge();
	gauge(value_type value, time_point timestamp = clock::now());

	void set(value_type value, time_point timestamp = clock::now());

	value_type value;
	time_point timestamp;

	incremental_statistics values;

}; // struct gauge

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_GAUGE_H_
