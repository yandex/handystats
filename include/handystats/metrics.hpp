/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#ifndef HANDYSTATS_METRICS_HPP_
#define HANDYSTATS_METRICS_HPP_

#include <boost/variant.hpp>

#include <handystats/metrics/gauge.hpp>
#include <handystats/metrics/counter.hpp>
#include <handystats/metrics/timer.hpp>
#include <handystats/metrics/attribute.hpp>

namespace handystats { namespace metrics {

// Generic metric
typedef boost::variant <
		counter,
		gauge,
		timer,
		attribute
	> metric_variant;


// Generic metric pointer
typedef boost::variant <
		counter*,
		gauge*,
		timer*,
		attribute*
	> metric_ptr_variant;


// Generic metrics index
enum metric_index {
	COUNTER = 0,
	GAUGE,
	TIMER,
	ATTRIBUTE
};

}} // namespace handystats::metrics

#endif // HANDYSTATS_METRICS_HPP_
