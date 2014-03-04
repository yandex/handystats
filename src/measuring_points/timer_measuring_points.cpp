#include <memory>

#include "events/timer_events_impl.hpp"
#include "message_queue_impl.hpp"
#include "core_impl.hpp"

#include <handystats/measuring_points/timer_measuring_points.hpp>


void HANDY_TIMER_INIT(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	if (handystats::is_enabled()) {
		auto message = handystats::events::timer_init_event(timer_name, instance_id, timestamp);
		handystats::message_queue::push_event_message(message);
	}
}

void HANDY_TIMER_START(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	if (handystats::is_enabled()) {
		auto message = handystats::events::timer_start_event(timer_name, instance_id, timestamp);
		handystats::message_queue::push_event_message(message);
	}
}

void HANDY_TIMER_STOP(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	if (handystats::is_enabled()) {
		auto message = handystats::events::timer_stop_event(timer_name, instance_id, timestamp);
		handystats::message_queue::push_event_message(message);
	}
}

void HANDY_TIMER_DISCARD(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	if (handystats::is_enabled()) {
		auto message = handystats::events::timer_discard_event(timer_name, instance_id, timestamp);
		handystats::message_queue::push_event_message(message);
	}
}

void HANDY_TIMER_HEARTBEAT(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	if (handystats::is_enabled()) {
		auto message = handystats::events::timer_heartbeat_event(timer_name, instance_id, timestamp);
		handystats::message_queue::push_event_message(message);
	}
}


