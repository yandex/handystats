#ifndef HANDY_COUNTER_MEASURING_POINTS_H_
#define HANDY_COUNTER_MEASURING_POINTS_H_

#include <string>

#include <handystats/metrics/counter.hpp>

void HANDY_COUNTER_INIT(
		const std::string counter_name,
		handystats::metrics::counter::value_type init_value = 0,
		handystats::metrics::counter::time_point timestamp = handystats::metrics::counter::clock::now()
		);

void HANDY_COUNTER_INCREMENT(
		const std::string counter_name,
		handystats::metrics::counter::value_type value = 1,
		handystats::metrics::counter::time_point timestamp = handystats::metrics::counter::clock::now()
		);

void HANDY_COUNTER_DECREMENT(
		const std::string counter_name,
		handystats::metrics::counter::value_type value = 1,
		handystats::metrics::counter::time_point timestamp = handystats::metrics::counter::clock::now()
		);

void HANDY_COUNTER_CHANGE(
		const std::string counter_name,
		handystats::metrics::counter::value_type value,
		handystats::metrics::counter::time_point timestamp = handystats::metrics::counter::clock::now()
		);

#endif // HANDYSTATS_COUNTER_MEASURING_POINTS_H_
