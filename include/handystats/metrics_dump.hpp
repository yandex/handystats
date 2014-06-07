#ifndef HANDYSTATS_METRICS_DUMP_H_
#define HANDYSTATS_METRICS_DUMP_H_

#include <memory>
#include <string>
#include <map>

#include <boost/variant.hpp>

#include <handystats/metrics/counter.hpp>
#include <handystats/metrics/gauge.hpp>
#include <handystats/metrics/timer.hpp>

namespace handystats { namespace metrics {

typedef const counter const_counter;
typedef const gauge const_gauge;
typedef const timer const_timer;

typedef const boost::variant <
			const_counter,
			const_gauge,
			const_timer
		> const_metric;

enum const_metric_index {
	CONST_COUNTER = 0,
	CONST_GAUGE,
	CONST_TIMER
};

typedef std::map<std::string, const_metric> const_metrics_dump;

}} // namespace handystats::metrics


const std::shared_ptr<const handystats::metrics::const_metrics_dump> HANDY_METRICS_DUMP();

#endif // HANDYSTATS_METRICS_DUMP_H_
