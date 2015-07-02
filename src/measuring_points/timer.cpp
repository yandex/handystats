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

#include "events/timer_impl.hpp"
#include "message_queue_impl.hpp"
#include "core_impl.hpp"

#include <handystats/measuring_points/timer.hpp>
#include <handystats/measuring_points/timer.h>


namespace handystats { namespace measuring_points {

void timer_init(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::timer::create_init_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_start(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::timer::create_start_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_stop(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::timer::create_stop_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_discard(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::timer::create_discard_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_heartbeat(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::timer::create_heartbeat_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_set(
		std::string&& timer_name,
		const metrics::timer::value_type& measurement,
		const metrics::timer::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::timer::create_set_event(std::move(timer_name), measurement, timestamp)
			);
	}
}

}} // namespace measuring_points

namespace {

handystats::chrono::time_unit tsc_clock_unit;

__attribute__((constructor(300)))
void init_tsc_clock_unit() {
	tsc_clock_unit = handystats::chrono::tsc_clock::now().time_since_epoch().unit();
}

} // unnamed namespace

extern "C" {

void handystats_timer_init(
		const char* timer_name,
		const uint64_t instance_id
	)
{
	handystats::measuring_points::timer_init(timer_name, instance_id);
}

void handystats_timer_start(
		const char* timer_name,
		const uint64_t instance_id
	)
{
	handystats::measuring_points::timer_start(timer_name, instance_id);
}

void handystats_timer_stop(
		const char* timer_name,
		const uint64_t instance_id
	)
{
	handystats::measuring_points::timer_stop(timer_name, instance_id);
}

void handystats_timer_discard(
		const char* timer_name,
		const uint64_t instance_id
	)
{
	handystats::measuring_points::timer_discard(timer_name, instance_id);
}

void handystats_timer_heartbeat(
		const char* timer_name,
		const uint64_t instance_id
	)
{
	handystats::measuring_points::timer_heartbeat(timer_name, instance_id);
}

void handystats_timer_set(
		const char* timer_name,
		const int64_t measurement
	)
{
	handystats::measuring_points::timer_set(
			timer_name,
			handystats::chrono::duration(measurement, tsc_clock_unit)
		);
}

} // extern "C"
