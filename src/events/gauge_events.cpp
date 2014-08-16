// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include "events/gauge_events_impl.hpp"


namespace handystats { namespace events { namespace gauge {

event_message* create_init_event(
		const std::string& gauge_name,
		const metrics::gauge::value_type& init_value,
		const metrics::gauge::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name = gauge_name;
	message->destination_type = event_destination_type::GAUGE;

	message->timestamp = timestamp;

	message->event_type = event_type::INIT;
	message->event_data.push_back(new metrics::gauge::value_type(init_value));

	return message;
}

void delete_init_event(event_message* message) {
	delete static_cast<metrics::gauge::value_type*>(message->event_data[0]);

	delete message;
}


event_message* create_set_event(
		const std::string& gauge_name,
		const metrics::gauge::value_type& value,
		const metrics::gauge::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name = gauge_name;
	message->destination_type = event_destination_type::GAUGE;

	message->timestamp = timestamp;

	message->event_type = event_type::SET;
	message->event_data.push_back(new metrics::gauge::value_type(value));

	return message;
}

void delete_set_event(event_message* message) {
	delete static_cast<metrics::gauge::value_type*>(message->event_data[0]);

	delete message;
}


void delete_event(event_message* message) {
	switch (message->event_type) {
		case event_type::INIT:
			delete_init_event(message);
			break;
		case event_type::SET:
			delete_set_event(message);
			break;
	}
}



void process_init_event(metrics::gauge& gauge, const event_message& message) {
	auto init_value = *static_cast<metrics::gauge::value_type*>(message.event_data[0]);
	gauge = metrics::gauge(init_value, message.timestamp);
}

void process_set_event(metrics::gauge& gauge, const event_message& message) {
	auto value = *static_cast<metrics::gauge::value_type*>(message.event_data[0]);
	gauge.set(value, message.timestamp);
}


void process_event(metrics::gauge& gauge, const event_message& message) {
	switch (message.event_type) {
		case event_type::INIT:
			process_init_event(gauge, message);
			break;
		case event_type::SET:
			process_set_event(gauge, message);
			break;
		default:
			return;
	}
}


}}} // namespace handystats::events::gauge

