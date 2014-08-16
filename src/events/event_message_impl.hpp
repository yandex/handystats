// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_EVENT_MESSAGE_HPP_
#define HANDYSTATS_EVENT_MESSAGE_HPP_

#include <string>
#include <vector>

#include <handystats/chrono.hpp>

#include "message_queue_impl.hpp"

namespace handystats { namespace events {

namespace event_destination_type {
enum {
	COUNTER,
	GAUGE,
	TIMER,
	ATTRIBUTE
};
}

struct event_message : message_queue::node
{
	std::string destination_name;
	int destination_type;

	chrono::clock::time_point timestamp;

	int event_type;
	std::vector<void*> event_data;
};

void delete_event_message(event_message* message);

struct event_message_deleter {
	void operator() (event_message* message) const {
		delete_event_message(message);
	}
};

}} // namespace handystats::events

#endif // HANDYSTATS_EVENT_MESSAGE_HPP_
