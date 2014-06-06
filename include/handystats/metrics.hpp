#ifndef HANDYSTATS_METRICS_H_
#define HANDYSTATS_METRICS_H_

#include <boost/variant.hpp>

namespace handystats { namespace metrics {

struct counter;
struct gauge;
struct timer;

}} // namespace handystats::metrics


namespace handystats { namespace metrics {

typedef boost::variant <
		counter*,
		gauge*,
		timer*
	> metric;

enum metric_index {
	COUNTER = 0,
	GAUGE,
	TIMER
};

}} // namespace handystats::metrics

#endif // HANDYSTATS_METRICS_H_
