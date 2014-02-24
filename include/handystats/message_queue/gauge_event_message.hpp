#ifndef HANDYSTATS_GAUGE_EVENT_MESSAGE_H_
#define HANDYSTATS_GAUGE_EVENT_MESSAGE_H_

#include "handystats/message_queue_impl.hpp"
#include "handystats/events/gauge_events.hpp"

inline void HANDY_GAUGE_INIT(
		const std::string gauge_name,
		handystats::metrics::gauge::value_type init_value,
		handystats::metrics::gauge::time_point timestamp = handystats::metrics::gauge::clock::now()
		)
{
	auto message = handystats::events::gauge_init_event(gauge_name, init_value, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_GAUGE_SET(
		const std::string gauge_name,
		handystats::metrics::gauge::value_type value,
		handystats::metrics::gauge::time_point timestamp = handystats::metrics::gauge::clock::now()
		)
{
	auto message = handystats::events::gauge_set_event(gauge_name, value, timestamp);
	handystats::message_queue::push_event_message(message);
}


#endif // HANDYSTATS_GAUGE_EVENT_MESSAGE_H_
