#ifndef HANDY_COUNTER_EVENT_MESSAGE_H_
#define HANDY_COUNTER_EVENT_MESSAGE_H_

#include "handystats/message_queue_impl.hpp"
#include "handystats/events/counter_events.hpp"

inline void HANDY_COUNTER_INIT(
		const std::string counter_name,
		handystats::metrics::counter::value_type init_value,
		handystats::metrics::counter::time_point timestamp = handystats::metrics::counter::clock::now()
		)
{
	auto message = handystats::events::counter_init_event(counter_name, init_value, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_COUNTER_INCREMENT(
		const std::string counter_name,
		handystats::metrics::counter::value_type value,
		handystats::metrics::counter::time_point timestamp = handystats::metrics::counter::clock::now()
		)
{
	auto message = handystats::events::counter_increment_event(counter_name, value, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_COUNTER_DECREMENT(
		const std::string counter_name,
		handystats::metrics::counter::value_type value,
		handystats::metrics::counter::time_point timestamp = handystats::metrics::counter::clock::now()
		)
{
	auto message = handystats::events::counter_decrement_event(counter_name, value, timestamp);
	handystats::message_queue::push_event_message(message);
}

#endif // HANDYSTATS_COUNTER_EVENT_MESSAGE_H_
