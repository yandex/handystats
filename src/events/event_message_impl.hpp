// Copyright (c) 2014 Yandex LLC. All rights reserved.

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

	chrono::clock::time_point timestamp;

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
