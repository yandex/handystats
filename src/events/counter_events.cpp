#include "handystats/events/counter_events.hpp"

namespace handystats { namespace events {

event_message* counter_init_event(
		const std::string counter_name,
		metrics::counter::value_type init_value,
		metrics::counter::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = counter_name;
	message->destination_type = event_destination_type::COUNTER;

	message->timestamp = timestamp;

	message->event_type = new counter_event(counter_event::INIT);
	message->event_data.push_back(new metrics::counter::value_type(init_value));

	return message;
}

void delete_counter_init_event(event_message* message) {
	delete static_cast<counter_event*>(message->event_type);
	delete static_cast<metrics::counter::value_type*>(message->event_data[0]);

	delete message;
}


event_message* counter_increment_event(
		const std::string counter_name,
		metrics::counter::value_type value,
		metrics::counter::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = counter_name;
	message->destination_type = event_destination_type::COUNTER;

	message->timestamp = timestamp;

	message->event_type = new counter_event(counter_event::INCREMENT);
	message->event_data.push_back(new metrics::counter::value_type(value));

	return message;
}

void delete_counter_increment_event(event_message* message) {
	delete static_cast<counter_event*>(message->event_type);
	delete static_cast<metrics::counter::value_type*>(message->event_data[0]);

	delete message;
}


event_message* counter_decrement_event(
		const std::string counter_name,
		metrics::counter::value_type value,
		metrics::counter::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = counter_name;
	message->destination_type = event_destination_type::COUNTER;

	message->timestamp = timestamp;

	message->event_type = new counter_event(counter_event::DECREMENT);
	message->event_data.push_back(new metrics::counter::value_type(value));

	return message;
}

void delete_counter_decrement_event(event_message* message) {
	delete static_cast<counter_event*>(message->event_type);
	delete static_cast<metrics::counter::value_type*>(message->event_data[0]);

	delete message;
}


void delete_counter_event(event_message* message) {
	switch (*static_cast<counter_event*>(message->event_type)) {
		case counter_event::INIT:
			delete_counter_init_event(message);
			break;
		case counter_event::INCREMENT:
			delete_counter_increment_event(message);
			break;
		case counter_event::DECREMENT:
			delete_counter_decrement_event(message);
			break;
	}
}


}} // namespace handystats::events


inline void HANDY_COUNTER_INIT(
		const std::string counter_name,
		handystats::metrics::counter::value_type init_value,
		handystats::metrics::counter::time_point timestamp
		)
{
	auto message = handystats::events::counter_init_event(counter_name, init_value, timestamp);
	handystats::event_message_queue.push(message);
}

inline void HANDY_COUNTER_INCREMENT(
		const std::string counter_name,
		handystats::metrics::counter::value_type value,
		handystats::metrics::counter::time_point timestamp
		)
{
	auto message = handystats::events::counter_increment_event(counter_name, value, timestamp);
	handystats::event_message_queue.push(message);
}

inline void HANDY_COUNTER_DECREMENT(
		const std::string counter_name,
		handystats::metrics::counter::value_type value,
		handystats::metrics::counter::time_point timestamp
		)
{
	auto message = handystats::events::counter_decrement_event(counter_name, value, timestamp);
	handystats::event_message_queue.push(message);
}

