// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/chrono.hpp>

#include "core_impl.hpp"

#include "events/timer_impl.hpp"


namespace handystats { namespace events { namespace timer {

event_message* create_init_event(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name.swap(timer_name);
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = event_type::INIT;
	new (&message->event_data) metrics::timer::instance_id_type(instance_id);

	return message;
}

void delete_init_event(event_message* message) {
	delete message;
}


event_message* create_start_event(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name.swap(timer_name);
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = event_type::START;
	new (&message->event_data) metrics::timer::instance_id_type(instance_id);

	return message;
}

void delete_start_event(event_message* message) {
	delete message;
}


event_message* create_stop_event(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name.swap(timer_name);
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = event_type::STOP;
	new (&message->event_data) metrics::timer::instance_id_type(instance_id);

	return message;
}

void delete_stop_event(event_message* message) {
	delete message;
}


event_message* create_discard_event(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name.swap(timer_name);
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = event_type::DISCARD;
	new (&message->event_data) metrics::timer::instance_id_type(instance_id);

	return message;
}

void delete_discard_event(event_message* message) {
	delete message;
}


event_message* create_heartbeat_event(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name.swap(timer_name);
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = event_type::HEARTBEAT;
	new (&message->event_data) metrics::timer::instance_id_type(instance_id);

	return message;
}

void delete_heartbeat_event(event_message* message) {
	delete message;
}


event_message* create_set_event(
		std::string&& timer_name,
		const metrics::timer::value_type& measurement,
		const metrics::timer::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name.swap(timer_name);
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = event_type::SET;
	new (&message->event_data)
		int64_t(
			chrono::duration::convert_to(
				metrics::timer::value_unit,
				measurement
			)
			.count()
		);

	return message;
}

void delete_set_event(event_message* message) {
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
		case event_type::SET:
			delete_set_event(message);
			break;
	}
}


void process_init_event(metrics::timer& timer, const event_message& message) {
	const auto& instance_id = reinterpret_cast<const metrics::timer::instance_id_type>(message.event_data);

	timer.reset();
	timer.start(instance_id, message.timestamp);
}

void process_start_event(metrics::timer& timer, const event_message& message) {
	const auto& instance_id = reinterpret_cast<const metrics::timer::instance_id_type>(message.event_data);
	timer.start(instance_id, message.timestamp);
}

void process_stop_event(metrics::timer& timer, const event_message& message) {
	const auto& instance_id = reinterpret_cast<const metrics::timer::instance_id_type>(message.event_data);
	timer.stop(instance_id, message.timestamp);
}

void process_discard_event(metrics::timer& timer, const event_message& message) {
	const auto& instance_id = reinterpret_cast<const metrics::timer::instance_id_type>(message.event_data);
	timer.discard(instance_id, message.timestamp);
}

void process_heartbeat_event(metrics::timer& timer, const event_message& message) {
	const auto& instance_id = reinterpret_cast<const metrics::timer::instance_id_type>(message.event_data);
	timer.heartbeat(instance_id, message.timestamp);
}

void process_set_event(metrics::timer& timer, const event_message& message) {
	const auto& duration_rep = reinterpret_cast<const int64_t>(message.event_data);
	timer.set(chrono::duration(duration_rep, metrics::timer::value_unit), message.timestamp);
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
		case event_type::SET:
			process_set_event(timer, message);
			break;
		default:
			return;
	}
}


}}} // namespace handystats::events::timer


