#include <handystats/measuring_points/timer_measuring_points.hpp>

#include <handystats/events/timer_events.hpp>
#include <handystats/message_queue_impl.hpp>

void HANDY_TIMER_INIT(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	auto message = handystats::events::timer_init_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

void HANDY_TIMER_START(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	auto message = handystats::events::timer_start_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

void HANDY_TIMER_STOP(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	auto message = handystats::events::timer_stop_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

void HANDY_TIMER_DISCARD(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	auto message = handystats::events::timer_discard_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

void HANDY_TIMER_HEARTBEAT(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	auto message = handystats::events::timer_heartbeat_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}


