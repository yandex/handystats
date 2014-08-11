// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <memory>

#include "events/gauge_events_impl.hpp"
#include "message_queue_impl.hpp"
#include "core_impl.hpp"

#include <handystats/measuring_points/gauge_measuring_points.hpp>
#include <handystats/measuring_points/gauge_measuring_points.h>


namespace handystats { namespace measuring_points {

void gauge_init(
		const std::string& gauge_name,
		const handystats::metrics::gauge::value_type& init_value,
		const handystats::metrics::gauge::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push_event_message(
				handystats::events::gauge::create_init_event(gauge_name, init_value, timestamp)
			);
	}
}

void gauge_set(
		const std::string& gauge_name,
		const handystats::metrics::gauge::value_type& value,
		const handystats::metrics::gauge::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push_event_message(
				handystats::events::gauge::create_set_event(gauge_name, value, timestamp)
			);
	}
}

}} // namespace handystats::measuring_points


extern "C" {

void handystats_gauge_init(
		const char* gauge_name,
		const double init_value
	)
{
	handystats::measuring_points::gauge_init(gauge_name, init_value);
}

void handystats_gauge_set(
		const char* gauge_name,
		const double value
	)
{
	handystats::measuring_points::gauge_set(gauge_name, value);
}

} // extern "C"
