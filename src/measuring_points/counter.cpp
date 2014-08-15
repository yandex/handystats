// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <memory>

#include "events/counter_events_impl.hpp"
#include "message_queue_impl.hpp"
#include "core_impl.hpp"

#include <handystats/measuring_points/counter.hpp>
#include <handystats/measuring_points/counter.h>


namespace handystats { namespace measuring_points {

void counter_init(
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

void counter_increment(
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

void counter_decrement(
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

void counter_change(
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

}} // namespace handystats::measuring_points


extern "C" {

void handystats_counter_init(
		const char* counter_name,
		const int64_t init_value
	)
{
	handystats::measuring_points::counter_init(counter_name, init_value);
}

void handystats_counter_increment(
		const char* counter_name,
		const int64_t value
	)
{
	handystats::measuring_points::counter_increment(counter_name, value);
}

void handystats_counter_decrement(
		const char* counter_name,
		const int64_t value
	)
{
	handystats::measuring_points::counter_decrement(counter_name, value);
}

void handystats_counter_change(
		const char* counter_name,
		const int64_t value
	)
{
	handystats::measuring_points::counter_change(counter_name, value);
}

} // extern "C"
