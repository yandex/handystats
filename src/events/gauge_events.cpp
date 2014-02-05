#include "handystats/internal.hpp"
#include "handystats/events/gauge_events.hpp"

namespace handystats { namespace events {

event_message* gauge_init_event(
		const std::string gauge_name,
		metrics::gauge::value_type init_value,
		metrics::gauge::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = gauge_name;
	message->destination_type = event_destination_type::GAUGE;

	message->timestamp = timestamp;

	message->event_type = new gauge_event(gauge_event::INIT);
	message->event_data.push_back(new metrics::gauge::value_type(init_value));

	return message;
}

void delete_gauge_init_event(event_message* message) {
	delete static_cast<gauge_event*>(message->event_type);
	delete static_cast<metrics::gauge::value_type*>(message->event_data[0]);

	delete message;
}


event_message* gauge_set_event(
		const std::string gauge_name,
		metrics::gauge::value_type value,
		metrics::gauge::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = gauge_name;
	message->destination_type = event_destination_type::GAUGE;

	message->timestamp = timestamp;

	message->event_type = new gauge_event(gauge_event::SET);
	message->event_data.push_back(new metrics::gauge::value_type(value));

	return message;
}

void delete_gauge_set_event(event_message* message) {
	delete static_cast<gauge_event*>(message->event_type);
	delete static_cast<metrics::gauge::value_type*>(message->event_data[0]);

	delete message;
}


void delete_gauge_event(event_message* message) {
	switch (*static_cast<gauge_event*>(message->event_type)) {
		case gauge_event::INIT:
			delete_gauge_init_event(message);
			break;
		case gauge_event::SET:
			delete_gauge_set_event(message);
			break;
	}
}


}} // namespace handystats::events


inline void HANDY_GAUGE_INIT(
		const std::string gauge_name,
		handystats::metrics::gauge::value_type init_value,
		handystats::metrics::gauge::time_point timestamp
		)
{
	auto message = handystats::events::gauge_init_event(gauge_name, init_value, timestamp);
	handystats::internal::event_message_queue.push(message);
}

inline void HANDY_GAUGE_SET(
		const std::string gauge_name,
		handystats::metrics::gauge::value_type value,
		handystats::metrics::gauge::time_point timestamp
		)
{
	auto message = handystats::events::gauge_set_event(gauge_name, value, timestamp);
	handystats::internal::event_message_queue.push(message);
}

