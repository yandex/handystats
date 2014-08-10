// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_EVENT_MESSAGE_HPP_
#define HANDYSTATS_EVENT_MESSAGE_HPP_

#include <string>
#include <vector>
#include <memory>

#include <handystats/chrono.hpp>


namespace handystats { namespace events {

namespace event_destination_type {
enum {
	COUNTER,
	GAUGE,
	TIMER
};
}

struct event_message {
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

typedef std::unique_ptr<event_message, event_message_deleter> event_message_ptr;

}} // namespace handystats::events

#endif // HANDYSTATS_EVENT_MESSAGE_HPP_
