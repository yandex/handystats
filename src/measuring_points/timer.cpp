// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <memory>

#include "events/timer_events_impl.hpp"
#include "message_queue_impl.hpp"
#include "core_impl.hpp"

#include <handystats/measuring_points/timer.hpp>
#include <handystats/measuring_points/timer.h>


namespace handystats { namespace measuring_points {

void timer_init(
		const std::string& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id,
		const handystats::metrics::timer::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push_event_message(
				handystats::events::timer::create_init_event(timer_name, instance_id, timestamp)
			);
	}
}

void timer_start(
		const std::string& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id,
		const handystats::metrics::timer::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push_event_message(
				handystats::events::timer::create_start_event(timer_name, instance_id, timestamp)
			);
	}
}

void timer_stop(
		const std::string& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id,
		const handystats::metrics::timer::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push_event_message(
				handystats::events::timer::create_stop_event(timer_name, instance_id, timestamp)
			);
	}
}

void timer_discard(
		const std::string& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id,
		const handystats::metrics::timer::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push_event_message(
				handystats::events::timer::create_discard_event(timer_name, instance_id, timestamp)
			);
	}
}

void timer_heartbeat(
		const std::string& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id,
		const handystats::metrics::timer::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push_event_message(
				handystats::events::timer::create_heartbeat_event(timer_name, instance_id, timestamp)
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
