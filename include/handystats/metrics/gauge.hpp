// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_METRICS_GAUGE_HPP_
#define HANDYSTATS_METRICS_GAUGE_HPP_

#include <utility>

#include <handystats/chrono.hpp>
#include <handystats/incremental_statistics.hpp>
#include <handystats/config/incremental_statistics.hpp>

namespace handystats { namespace metrics {

struct gauge
{
	typedef double value_type;
	typedef chrono::time_duration time_duration;
	typedef chrono::clock clock;
	typedef clock::time_point time_point;

	gauge(const config::incremental_statistics& opts = config::incremental_statistics());

	void set(const value_type& value, const time_point& timestamp = clock::now());

	value_type value;
	time_point timestamp;

	incremental_statistics values;

}; // struct gauge

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_GAUGE_HPP_
