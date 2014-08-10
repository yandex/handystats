// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_METRICS_HPP_
#define HANDYSTATS_METRICS_HPP_

#include <boost/variant.hpp>

#include <handystats/metrics/gauge.hpp>
#include <handystats/metrics/counter.hpp>
#include <handystats/metrics/timer.hpp>

namespace handystats { namespace metrics {

// Generic metric
typedef boost::variant <
		counter,
		gauge,
		timer
	> metric_variant;


// Generic metric pointer
typedef boost::variant <
		counter*,
		gauge*,
		timer*
	> metric_ptr_variant;


// Generic metrics index
enum metric_index {
	COUNTER = 0,
	GAUGE,
	TIMER
};

}} // namespace handystats::metrics

#endif // HANDYSTATS_METRICS_HPP_
