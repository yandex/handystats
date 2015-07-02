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

#include <memory>
#include <algorithm>

#include "events/counter_impl.hpp"
#include "message_queue_impl.hpp"
#include "core_impl.hpp"

#include <handystats/measuring_points/counter.hpp>
#include <handystats/measuring_points/counter.h>


namespace handystats { namespace measuring_points {

void counter_init(
		std::string&& counter_name,
		const handystats::metrics::counter::value_type& init_value,
		const handystats::metrics::counter::time_point& timestamp
		)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::counter::create_init_event(std::move(counter_name), init_value, timestamp)
			);
	}
}

void counter_increment(
		std::string&& counter_name,
		const handystats::metrics::counter::value_type& value,
		const handystats::metrics::counter::time_point& timestamp
		)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::counter::create_increment_event(std::move(counter_name), value, timestamp)
			);
	}
}

void counter_decrement(
		std::string&& counter_name,
		const handystats::metrics::counter::value_type& value,
		const handystats::metrics::counter::time_point& timestamp
		)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::counter::create_decrement_event(std::move(counter_name), value, timestamp)
			);
	}
}

void counter_change(
		std::string&& counter_name,
		const handystats::metrics::counter::value_type& value,
		const handystats::metrics::counter::time_point& timestamp
		)
{
	if (handystats::is_enabled()) {
		if (value >= 0) {
			HANDY_COUNTER_INCREMENT(std::move(counter_name), value, timestamp);
		}
		else {
			HANDY_COUNTER_DECREMENT(std::move(counter_name), -value, timestamp);
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
