#include <handystats/message_queue_impl.hpp>

#include <handystats/metrics/gauge.hpp>

namespace handystats {

extern metrics::gauge message_push_time;
extern metrics::gauge message_pop_time;
extern metrics::gauge message_delete_time;

}

namespace handystats { namespace message_queue {

tbb::concurrent_queue<events::event_message*>* event_message_queue = nullptr;

void push_event_message(events::event_message* message) {
	if (event_message_queue) {
		auto push_start_time = chrono::default_clock::now();
		event_message_queue->push(message);
		auto push_end_time = chrono::default_clock::now();
		message_push_time.set(std::chrono::duration_cast<chrono::default_duration>(push_end_time - push_start_time).count(), push_end_time);
	}
	else {
		auto delete_start_time = chrono::default_clock::now();
		events::delete_event_message(message);
		auto delete_end_time = chrono::default_clock::now();
		message_delete_time.set(std::chrono::duration_cast<chrono::default_duration>(delete_end_time - delete_start_time).count(), delete_end_time);
	}
}

events::event_message* pop_event_message() {
	events::event_message* message = nullptr;

	if (event_message_queue) {
		auto pop_start_time = chrono::default_clock::now();
		event_message_queue->try_pop(message);
		auto pop_end_time = chrono::default_clock::now();
		message_pop_time.set(std::chrono::duration_cast<chrono::default_duration>(pop_end_time - pop_start_time).count(), pop_end_time);
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
