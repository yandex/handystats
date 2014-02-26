#ifndef HANDYSTATS_INTERNAL_METRICS_IMPL_H_
#define HANDYSTATS_INTERNAL_METRICS_IMPL_H_

#include <boost/variant.hpp>


namespace handystats { namespace internal {

struct internal_gauge;
struct internal_counter;
struct internal_timer;

}} // namespace handystats::internal


namespace handystats { namespace internal {

typedef boost::variant <
		internal_counter*,
		internal_gauge*,
		internal_timer*
	> internal_metric;

enum internal_metric_index {
	INTERNAL_COUNTER = 0,
	INTERNAL_GAUGE,
	INTERNAL_TIMER
};

}} // namespace handystats::internal


#endif // HANDYSTATS_INTERNAL_METRICS_IMPL_H_
