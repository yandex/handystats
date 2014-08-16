// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include "events/counter_events_impl.hpp"


namespace handystats { namespace events { namespace counter {

event_message* create_init_event(
		std::string&& counter_name,
		const metrics::counter::value_type& init_value,
		const metrics::counter::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name.swap(counter_name);
	message->destination_type = event_destination_type::COUNTER;

	message->timestamp = timestamp;

	message->event_type = event_type::INIT;
	message->event_data.push_back(new metrics::counter::value_type(init_value));

	return message;
}

void delete_init_event(event_message* message) {
	delete static_cast<metrics::counter::value_type*>(message->event_data[0]);

	delete message;
}


event_message* create_increment_event(
		std::string&& counter_name,
		const metrics::counter::value_type& value,
		const metrics::counter::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name.swap(counter_name);
	message->destination_type = event_destination_type::COUNTER;

	message->timestamp = timestamp;

	message->event_type = event_type::INCREMENT;
	message->event_data.push_back(new metrics::counter::value_type(value));

	return message;
}

void delete_increment_event(event_message* message) {
	delete static_cast<metrics::counter::value_type*>(message->event_data[0]);

	delete message;
}


event_message* create_decrement_event(
		std::string&& counter_name,
		const metrics::counter::value_type& value,
		const metrics::counter::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name.swap(counter_name);
	message->destination_type = event_destination_type::COUNTER;

	message->timestamp = timestamp;

	message->event_type = event_type::DECREMENT;
	message->event_data.push_back(new metrics::counter::value_type(value));

	return message;
}

void delete_decrement_event(event_message* message) {
	delete static_cast<metrics::counter::value_type*>(message->event_data[0]);

	delete message;
}


void delete_event(event_message* message) {
	switch (message->event_type) {
		case event_type::INIT:
			delete_init_event(message);
			break;
		case event_type::INCREMENT:
			delete_increment_event(message);
			break;
		case event_type::DECREMENT:
			delete_decrement_event(message);
			break;
	}
}



void process_init_event(metrics::counter& counter, const event_message& message) {
	auto init_value = *static_cast<metrics::counter::value_type*>(message.event_data[0]);
	counter = metrics::counter(init_value, message.timestamp);
}

void process_increment_event(metrics::counter& counter, const event_message& message) {
	auto incr_value = *static_cast<metrics::counter::value_type*>(message.event_data[0]);
	counter.increment(incr_value, message.timestamp);
}

void process_decrement_event(metrics::counter& counter, const event_message& message) {
	auto decr_value = *static_cast<metrics::counter::value_type*>(message.event_data[0]);
	counter.decrement(decr_value, message.timestamp);
}


void process_event(metrics::counter& counter, const event_message& message) {
	switch (message.event_type) {
		case event_type::INIT:
			process_init_event(counter, message);
			break;
		case event_type::INCREMENT:
			process_increment_event(counter, message);
			break;
		case event_type::DECREMENT:
			process_decrement_event(counter, message);
			break;
		default:
			return;
	}
}


}}} // namespace handystats::events::counter

