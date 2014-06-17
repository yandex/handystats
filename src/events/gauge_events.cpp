// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/gauge.hpp>

#include "events/event_message_impl.hpp"
#include "events/gauge_events_impl.hpp"


namespace handystats { namespace events {

std::shared_ptr<event_message> gauge_init_event(
		const std::string gauge_name,
		metrics::gauge::value_type init_value,
		metrics::gauge::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = gauge_name;
	message->destination_type = event_destination_type::GAUGE;

	message->timestamp = timestamp;

	message->event_type = gauge_event::INIT;
	message->event_data.push_back(new metrics::gauge::value_type(init_value));

	return std::shared_ptr<event_message>(message, delete_event_message);
}

void delete_gauge_init_event(event_message* message) {
	delete static_cast<metrics::gauge::value_type*>(message->event_data[0]);

	delete message;
}


std::shared_ptr<event_message> gauge_set_event(
		const std::string gauge_name,
		metrics::gauge::value_type value,
		metrics::gauge::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = gauge_name;
	message->destination_type = event_destination_type::GAUGE;

	message->timestamp = timestamp;

	message->event_type = gauge_event::SET;
	message->event_data.push_back(new metrics::gauge::value_type(value));

	return std::shared_ptr<event_message>(message, delete_event_message);
}

void delete_gauge_set_event(event_message* message) {
	delete static_cast<metrics::gauge::value_type*>(message->event_data[0]);

	delete message;
}


void delete_gauge_event(event_message* message) {
	switch (message->event_type) {
		case gauge_event::INIT:
			delete_gauge_init_event(message);
			break;
		case gauge_event::SET:
			delete_gauge_set_event(message);
			break;
	}
}


}} // namespace handystats::events

