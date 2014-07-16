// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <memory>

#include "events/counter_events_impl.hpp"
#include "message_queue_impl.hpp"
#include "core_impl.hpp"

#include <handystats/measuring_points/counter_measuring_points.hpp>


void HANDY_COUNTER_INIT(
		const std::string& counter_name,
		const handystats::metrics::counter::value_type& init_value,
		const handystats::metrics::counter::time_point& timestamp
		)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push_event_message(
				handystats::events::counter::create_init_event(counter_name, init_value, timestamp)
			);
	}
}

void HANDY_COUNTER_INCREMENT(
		const std::string& counter_name,
		const handystats::metrics::counter::value_type& value,
		const handystats::metrics::counter::time_point& timestamp
		)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push_event_message(
				handystats::events::counter::create_increment_event(counter_name, value, timestamp)
			);
	}
}

void HANDY_COUNTER_DECREMENT(
		const std::string& counter_name,
		const handystats::metrics::counter::value_type& value,
		const handystats::metrics::counter::time_point& timestamp
		)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push_event_message(
				handystats::events::counter::create_decrement_event(counter_name, value, timestamp)
			);
	}
}

void HANDY_COUNTER_CHANGE(
		const std::string& counter_name,
		const handystats::metrics::counter::value_type& value,
		const handystats::metrics::counter::time_point& timestamp
		)
{
	if (handystats::is_enabled()) {
		if (value >= 0) {
			HANDY_COUNTER_INCREMENT(counter_name, value, timestamp);
		}
		else {
			HANDY_COUNTER_DECREMENT(counter_name, -value, timestamp);
		}
	}
}
