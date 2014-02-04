#ifndef HANDYSTATS_EVENT_MESSAGE_H_
#define HANDYSTATS_EVENT_MESSAGE_H_

#include <string>
#include <vector>

#include <tbb/concurrent_queue.h>

#include "handystats/chrono.hpp"

namespace handystats {
namespace events {

enum class event_destination_type {
	COUNTER,
	GAUGE,
	TRANSACTION
};

struct event_message {
	std::string destination_name;
	event_destination_type destination_type;

	chrono::steady_clock<std::chrono::nanoseconds>::time_point timestamp;

	void* event_type;
	std::vector<void*> event_data;
};

} // namespace handystats::events


static tbb::concurrent_queue<events::event_message*> event_message_queue;

} // namespace handystats

#endif // HANDYSTATS_EVENT_MESSAGE_H_
