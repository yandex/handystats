/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#ifndef HANDYSTATS_EVENT_MESSAGE_HPP_
#define HANDYSTATS_EVENT_MESSAGE_HPP_

#include <string>
#include <vector>

#include <handystats/chrono.hpp>

#include "message_queue_impl.hpp"

namespace handystats { namespace events {

namespace event_destination_type {
enum : char {
	COUNTER = 0,
	GAUGE,
	TIMER,
	ATTRIBUTE
};
}

struct event_message : message_queue::node
{
	char destination_type;
	char event_type;
	std::string destination_name;

	chrono::time_point timestamp;

	void* event_data;
};

void delete_event_message(event_message* message);

struct event_message_deleter {
	void operator() (event_message* message) const {
		delete_event_message(message);
	}
};

}} // namespace handystats::events

#endif // HANDYSTATS_EVENT_MESSAGE_HPP_
