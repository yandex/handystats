#include <memory>

#include <tbb/concurrent_queue.h>

#include "events/event_message_impl.hpp"
#include "system_stats_impl.hpp"


namespace handystats { namespace message_queue {

tbb::concurrent_queue<std::shared_ptr<events::event_message>>* event_message_queue = nullptr;

void push_event_message(std::shared_ptr<events::event_message> message) {
	if (event_message_queue) {
		event_message_queue->push(message);
	}
}

std::shared_ptr<events::event_message> pop_event_message() {
	std::shared_ptr<events::event_message> message;

	if (event_message_queue) {
		auto pop_start_time = chrono::default_clock::now();
		event_message_queue->try_pop(message);
		auto pop_end_time = chrono::default_clock::now();

		if (message) {
			message_pop_time.set(std::chrono::duration_cast<chrono::default_duration>(pop_end_time - pop_start_time).count(), pop_end_time);
			message_queue_size.set(event_message_queue->unsafe_size(), pop_end_time);
		}
	}

	return message;
}

bool empty() {
	if (!event_message_queue) {
		return true;
	}

	return event_message_queue->empty();
}

void initialize() {
	if (!event_message_queue) {
		event_message_queue = new tbb::concurrent_queue<std::shared_ptr<events::event_message>>();
	}
}

void finalize() {
	if (event_message_queue) {
		event_message_queue->clear();
		delete event_message_queue;
		event_message_queue = nullptr;
	}
}

}} // namespace handystats::message_queue
