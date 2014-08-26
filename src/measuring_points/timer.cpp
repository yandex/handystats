// Copyright (c) 2014 Yandex LLC. All rights reserved.

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
		const handystats::metrics::timer::instance_id_type& instance_id,
		const handystats::metrics::timer::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::timer::create_init_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_start(
		std::string&& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id,
		const handystats::metrics::timer::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::timer::create_start_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_stop(
		std::string&& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id,
		const handystats::metrics::timer::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::timer::create_stop_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_discard(
		std::string&& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id,
		const handystats::metrics::timer::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::timer::create_discard_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_heartbeat(
		std::string&& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id,
		const handystats::metrics::timer::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::timer::create_heartbeat_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

}} // namespace handystats::measuring_points


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

} // extern "C"
