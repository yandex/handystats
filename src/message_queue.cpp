#include <memory>

#include <tbb/concurrent_queue.h>

#include <handystats/metrics/gauge.hpp>

#include "events/event_message_impl.hpp"


namespace handystats { namespace internal {

extern metrics::gauge message_push_time;
extern metrics::gauge message_pop_time;
extern metrics::gauge event_message_queue_size;

}} // namespace handystats::internal


namespace handystats { namespace message_queue {

tbb::concurrent_queue<std::shared_ptr<events::event_message>>* event_message_queue = nullptr;

void push_event_message(std::shared_ptr<events::event_message> message) {
	if (event_message_queue) {
		auto push_start_time = chrono::default_clock::now();
		event_message_queue->push(message);
		auto push_end_time = chrono::default_clock::now();

		internal::message_push_time.set(std::chrono::duration_cast<chrono::default_duration>(push_end_time - push_start_time).count(), push_end_time);
		internal::event_message_queue_size.set(message_queue::event_message_queue->unsafe_size(), push_end_time);
	}
}

std::shared_ptr<events::event_message> pop_event_message() {
	std::shared_ptr<events::event_message> message;

	if (event_message_queue) {
		auto pop_start_time = chrono::default_clock::now();
		event_message_queue->try_pop(message);
		auto pop_end_time = chrono::default_clock::now();

		internal::message_pop_time.set(std::chrono::duration_cast<chrono::default_duration>(pop_end_time - pop_start_time).count(), pop_end_time);
		internal::event_message_queue_size.set(message_queue::event_message_queue->unsafe_size(), pop_end_time);
	}

	return message;
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
