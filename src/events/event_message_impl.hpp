#ifndef HANDYSTATS_EVENT_MESSAGE_H_
#define HANDYSTATS_EVENT_MESSAGE_H_

#include <string>
#include <vector>

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

	chrono::default_time_point timestamp;

	int event_type;
	std::vector<void*> event_data;

	event_message();
	~event_message();
};

void delete_event_message(event_message* message);

}} // namespace handystats::events

#endif // HANDYSTATS_EVENT_MESSAGE_H_
