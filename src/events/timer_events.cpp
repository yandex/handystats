// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/chrono.hpp>

#include "configuration_impl.hpp"

#include "events/timer_events_impl.hpp"


namespace handystats { namespace events { namespace timer {

event_message_ptr create_init_event(
		const std::string& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name = timer_name;
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = event_type::INIT;
	message->event_data.push_back(new metrics::timer::instance_id_type(instance_id));

	return event_message_ptr(message);
}

void delete_init_event(event_message* message) {
	delete static_cast<metrics::timer::instance_id_type*>(message->event_data[0]);

	delete message;
}


event_message_ptr create_start_event(
		const std::string& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name = timer_name;
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = event_type::START;
	message->event_data.push_back(new metrics::timer::instance_id_type(instance_id));

	return event_message_ptr(message);
}

void delete_start_event(event_message* message) {
	delete static_cast<metrics::timer::instance_id_type*>(message->event_data[0]);

	delete message;
}


event_message_ptr create_stop_event(
		const std::string& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name = timer_name;
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = event_type::STOP;
	message->event_data.push_back(new metrics::timer::instance_id_type(instance_id));

	return event_message_ptr(message);
}

void delete_stop_event(event_message* message) {
	delete static_cast<metrics::timer::instance_id_type*>(message->event_data[0]);

	delete message;
}


event_message_ptr create_discard_event(
		const std::string& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name = timer_name;
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = event_type::DISCARD;
	message->event_data.push_back(new metrics::timer::instance_id_type(instance_id));

	return event_message_ptr(message);
}

void delete_discard_event(event_message* message) {
	delete static_cast<metrics::timer::instance_id_type*>(message->event_data[0]);

	delete message;
}


event_message_ptr create_heartbeat_event(
		const std::string& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name = timer_name;
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = event_type::HEARTBEAT;
	message->event_data.push_back(new metrics::timer::instance_id_type(instance_id));

	return event_message_ptr(message);
}

void delete_heartbeat_event(event_message* message) {
	delete static_cast<metrics::timer::instance_id_type*>(message->event_data[0]);

	delete message;
}


void delete_event(event_message* message) {
	switch (message->event_type) {
		case event_type::INIT:
			delete_init_event(message);
			break;
		case event_type::START:
			delete_start_event(message);
			break;
		case event_type::STOP:
			delete_stop_event(message);
			break;
		case event_type::DISCARD:
			delete_discard_event(message);
			break;
		case event_type::HEARTBEAT:
			delete_heartbeat_event(message);
			break;
	}
}



void process_init_event(metrics::timer& timer, const event_message& message) {
	auto instance_id = *static_cast<metrics::timer::instance_id_type*>(message.event_data[0]);

	timer = metrics::timer();
	timer.start(instance_id, message.timestamp);
}

void process_start_event(metrics::timer& timer, const event_message& message) {
	auto instance_id = *static_cast<metrics::timer::instance_id_type*>(message.event_data[0]);
	timer.start(instance_id, message.timestamp);
}

void process_stop_event(metrics::timer& timer, const event_message& message) {
	auto instance_id = *static_cast<metrics::timer::instance_id_type*>(message.event_data[0]);
	timer.stop(instance_id, message.timestamp);
}

void process_discard_event(metrics::timer& timer, const event_message& message) {
	auto instance_id = *static_cast<metrics::timer::instance_id_type*>(message.event_data[0]);
	timer.discard(instance_id, message.timestamp);
}

void process_heartbeat_event(metrics::timer& timer, const event_message& message) {
	auto instance_id = *static_cast<metrics::timer::instance_id_type*>(message.event_data[0]);
	timer.heartbeat(instance_id, message.timestamp);
}


void process_event(metrics::timer& timer, const event_message& message) {
	switch (message.event_type) {
		case event_type::INIT:
			process_init_event(timer, message);
			break;
		case event_type::START:
			process_start_event(timer, message);
			break;
		case event_type::STOP:
			process_stop_event(timer, message);
			break;
		case event_type::DISCARD:
			process_discard_event(timer, message);
			break;
		case event_type::HEARTBEAT:
			process_heartbeat_event(timer, message);
			break;
		default:
			return;
	}
}


}}} // namespace handystats::events::timer


