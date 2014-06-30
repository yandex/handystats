// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_GAUGE_MEASURING_POINTS_H_
#define HANDYSTATS_GAUGE_MEASURING_POINTS_H_

#include <string>

#include <handystats/metrics/gauge.hpp>

void HANDY_GAUGE_INIT(
		const std::string& gauge_name,
		const handystats::metrics::gauge::value_type& init_value,
		const handystats::metrics::gauge::time_point& timestamp = handystats::metrics::gauge::clock::now()
	);

void HANDY_GAUGE_SET(
		const std::string& gauge_name,
		const handystats::metrics::gauge::value_type& value,
		const handystats::metrics::gauge::time_point& timestamp = handystats::metrics::gauge::clock::now()
	);

#endif // HANDYSTATS_GAUGE_MEASURING_POINTS_H_
