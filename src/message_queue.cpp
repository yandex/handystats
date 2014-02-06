#include "handystats/message_queue.hpp"

namespace handystats { namespace message_queue {

tbb::concurrent_queue<events::event_message*>* event_message_queue = nullptr;

void push_event_message(events::event_message* message) {
	if (event_message_queue) {
		event_message_queue->push(message);
	}
}

events::event_message* pop_event_message() {
	events::event_message* message = nullptr;

	if (event_message_queue) {
		event_message_queue->try_pop(message);
	}

	return message;
}

void initialize() {
	if (!event_message_queue) {
		event_message_queue = new tbb::concurrent_queue<events::event_message*>();
	}
}

void clean_up() {
	if (event_message_queue) {
		while (!event_message_queue->empty()) {
			events::event_message* message = pop_event_message();
			events::delete_event_message(message);
		}

		delete event_message_queue;
		event_message_queue = nullptr;
	}
}

}} // namespace handystats::message_queue
