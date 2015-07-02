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

#ifndef HANDYSTATS_GAUGE_MEASURING_POINTS_HPP_
#define HANDYSTATS_GAUGE_MEASURING_POINTS_HPP_

#include <string>

#include <handystats/macros.h>
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

	#define HANDY_GAUGE_INIT(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::gauge_init, __VA_ARGS__)

	#define HANDY_GAUGE_SET(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::gauge_set, __VA_ARGS__)

#else

	#define HANDY_GAUGE_INIT(...)

	#define HANDY_GAUGE_SET(...)

#endif

#endif // HANDYSTATS_GAUGE_MEASURING_POINTS_HPP_
