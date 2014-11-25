/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstring>

#include <handystats/common.h>

#include "config_impl.hpp"

#include "events/counter_impl.hpp"


namespace handystats { namespace events { namespace counter {

event_message* create_init_event(
		std::string&& counter_name,
		const metrics::counter::value_type& init_value,
		const metrics::counter::time_point& timestamp
	)
{
	event_message* message = allocate_event_message();
	if (!message) {
		return nullptr;
	}

	strncpy(message->destination_name, counter_name.c_str(), event_message::NAME_SIZE);
	message->destination_type = event_destination_type::COUNTER;

	message->timestamp = timestamp;

	message->event_type = event_type::INIT;
	new (&message->event_data) metrics::counter::value_type(init_value);

	return message;
}

void delete_init_event(event_message* message) {
}


event_message* create_increment_event(
		std::string&& counter_name,
		const metrics::counter::value_type& value,
		const metrics::counter::time_point& timestamp
	)
{
	event_message* message = allocate_event_message();
	if (!message) {
		return nullptr;
	}

	strncpy(message->destination_name, counter_name.c_str(), event_message::NAME_SIZE);
	message->destination_type = event_destination_type::COUNTER;

	message->timestamp = timestamp;

	message->event_type = event_type::INCREMENT;
	new (&message->event_data) metrics::counter::value_type(value);

	return message;
}

void delete_increment_event(event_message* message) {
}


event_message* create_decrement_event(
		std::string&& counter_name,
		const metrics::counter::value_type& value,
		const metrics::counter::time_point& timestamp
	)
{
	event_message* message = allocate_event_message();
	if (!message) {
		return nullptr;
	}

	strncpy(message->destination_name, counter_name.c_str(), event_message::NAME_SIZE);
	message->destination_type = event_destination_type::COUNTER;

	message->timestamp = timestamp;

	message->event_type = event_type::DECREMENT;
	new (&message->event_data) metrics::counter::value_type(value);

	return message;
}

void delete_decrement_event(event_message* message) {
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
	const auto& init_value = reinterpret_cast<const metrics::counter::value_type>(message.event_data);

	counter.reset();
	counter.init(init_value, message.timestamp);
}

void process_increment_event(metrics::counter& counter, const event_message& message) {
	const auto& incr_value = reinterpret_cast<const metrics::counter::value_type>(message.event_data);
	counter.increment(incr_value, message.timestamp);
}

void process_decrement_event(metrics::counter& counter, const event_message& message) {
	const auto& decr_value = reinterpret_cast<const metrics::counter::value_type>(message.event_data);
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

