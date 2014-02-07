#ifndef HANDYSTATS_TIMER_EVENT_MESSAGE_H_
#define HANDYSTATS_TIMER_EVENT_MESSAGE_H_

#include "handystats/message_queue.hpp"
#include "handystats/events/timer_events.hpp"

inline void HANDY_TIMER_INIT(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		)
{
	auto message = handystats::events::timer_init_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_TIMER_START(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		)
{
	auto message = handystats::events::timer_start_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_TIMER_FINISH(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		)
{
	auto message = handystats::events::timer_finish_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_TIMER_DISCARD(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		)
{
	auto message = handystats::events::timer_discard_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_TIMER_HEARTBEAT(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		)
{
	auto message = handystats::events::timer_heartbeat_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

#endif // HANDYSTATS_TIMER_EVENT_MESSAGE_H_

