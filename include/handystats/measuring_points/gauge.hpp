// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_GAUGE_MEASURING_POINTS_HPP_
#define HANDYSTATS_GAUGE_MEASURING_POINTS_HPP_

#include <string>

#include <handystats/metrics/gauge.hpp>


namespace handystats { namespace measuring_points {

void gauge_init(
		std::string&& gauge_name,
		const handystats::metrics::gauge::value_type& init_value,
		const handystats::metrics::gauge::time_point& timestamp = handystats::metrics::gauge::clock::now()
	);

void gauge_set(
		std::string&& gauge_name,
		const handystats::metrics::gauge::value_type& value,
		const handystats::metrics::gauge::time_point& timestamp = handystats::metrics::gauge::clock::now()
	);

}} // namespace handystats::measuring_points


#ifndef HANDYSTATS_DISABLE

	#define HANDY_GAUGE_INIT(...) handystats::measuring_points::gauge_init(__VA_ARGS__)

	#define HANDY_GAUGE_SET(...) handystats::measuring_points::gauge_set(__VA_ARGS__)

#else

	#define HANDY_GAUGE_INIT(...)

	#define HANDY_GAUGE_SET(...)

#endif

#endif // HANDYSTATS_GAUGE_MEASURING_POINTS_HPP_
