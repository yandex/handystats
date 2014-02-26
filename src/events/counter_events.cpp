#include "events/event_message_impl.hpp"
#include "events/counter_events_impl.hpp"


namespace handystats { namespace events {

std::shared_ptr<event_message> counter_init_event(
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

	return std::shared_ptr<event_message>(message, delete_event_message);
}

void delete_counter_init_event(event_message* message) {
	delete static_cast<counter_event*>(message->event_type);
	delete static_cast<metrics::counter::value_type*>(message->event_data[0]);

	delete message;
}


std::shared_ptr<event_message> counter_increment_event(
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

	return std::shared_ptr<event_message>(message, delete_event_message);
}

void delete_counter_increment_event(event_message* message) {
	delete static_cast<counter_event*>(message->event_type);
	delete static_cast<metrics::counter::value_type*>(message->event_data[0]);

	delete message;
}


std::shared_ptr<event_message> counter_decrement_event(
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

	return std::shared_ptr<event_message>(message, delete_event_message);
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

