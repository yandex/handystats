#ifndef HANDYSTATS_INTERNAL_METRICS_IMPL_H_
#define HANDYSTATS_INTERNAL_METRICS_IMPL_H_

#include <string>
#include <unordered_map>

#include <boost/variant.hpp>

#include <handystats/internal_metrics/internal_counter.hpp>
#include <handystats/internal_metrics/internal_gauge.hpp>
#include <handystats/internal_metrics/internal_timer.hpp>

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

extern std::unordered_map<std::string, internal_metric> internal_metrics;

}} // namespace handystats::internal

#endif // HANDYSTATS_INTERNAL_METRICS_IMPL_H_
