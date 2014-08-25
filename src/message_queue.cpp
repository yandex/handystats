// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/atomic.hpp>
#include <algorithm>

#include <handystats/chrono.hpp>

#include "events/event_message_impl.hpp"
#include "config_impl.hpp"

#include "message_queue_impl.hpp"

namespace {

/*
 * Taken from Boost's Atomic usage examples.
 */
struct __event_message_queue
{
	typedef handystats::message_queue::node node;

	__event_message_queue() : head_node(nullptr) {}

	void push(node* n)
	{
		node* stale_head = head_node.load(std::memory_order_relaxed);
		do {
			n->next = static_cast<handystats::events::event_message*>(stale_head);
		}
		while (
				!head_node.compare_exchange_weak(
					stale_head, // expected
					n, // desired
					std::memory_order_release // order
				)
			);
	}

	node* pop_all(void)
	{
		node* last = pop_all_reverse();
		node* first = 0;
		while(last) {
			node* tmp = last;
			last = last->next;
			tmp->next = static_cast<handystats::events::event_message*>(first);
			first = tmp;
		}
		return first;
	}

	// alternative interface if ordering is of no importance
	node* pop_all_reverse(void)
	{
		// TODO: memory order should be std::memory_order_consume here, but it causes compilation error
		return head_node.exchange(0, std::memory_order_acquire);
	}

	bool empty() const {
		return !head_node.load(std::memory_order_acquire);
	}

	~__event_message_queue() {
		// what if queue is not empty?
		// this will be memory leak for sure
	}

	std::atomic<node*> head_node;
};

} // unnamed namespace


namespace handystats { namespace message_queue {


namespace stats {

metrics::gauge size;
metrics::gauge message_wait_time;
metrics::counter pop_count;

void initialize() {
	size = metrics::gauge(config::incremental_statistics_opts);
	size.set(0);

	message_wait_time = metrics::gauge(config::incremental_statistics_opts);

	pop_count = metrics::counter(config::incremental_statistics_opts);
}

void finalize() {
	size = metrics::gauge(config::incremental_statistics_opts);
	size.set(0);

	message_wait_time = metrics::gauge(config::incremental_statistics_opts);

	pop_count = metrics::counter(config::incremental_statistics_opts);
}

} // namespace stats


__event_message_queue* event_message_queue = nullptr;
__event_message_queue::node* popped_head = nullptr;

std::atomic<size_t> mq_size(0);

void push(node* n) {
	if (event_message_queue) {
		event_message_queue->push(n);
		++mq_size;
	}
}

events::event_message* pop() {
	events::event_message* message = nullptr;

	if (!popped_head && event_message_queue) {
		popped_head = event_message_queue->pop_all();
	}

	if (popped_head) {
		message = static_cast<events::event_message*>(popped_head);
		--mq_size;

		popped_head = popped_head->next;
	}

	if (message) {
		auto current_time = chrono::clock::now();
		stats::size.set(size(), current_time);
		stats::pop_count.increment(1, current_time);

		stats::message_wait_time.set(
				chrono::duration_cast<chrono::time_duration>(current_time - message->timestamp).count(),
				current_time
			);
	}

	return message;
}

bool empty() {
	return mq_size.load(std::memory_order_acquire) == 0;
}

size_t size() {
	return mq_size.load(std::memory_order_acquire);
}

void initialize() {
	if (!event_message_queue) {
		event_message_queue = new __event_message_queue();
		mq_size.store(0, std::memory_order_release);
	}

	stats::initialize();
}

void finalize() {
	while (!empty()) {
		auto* message = pop();
		events::delete_event_message(message);
	}
	mq_size.store(0);

	delete event_message_queue;
	event_message_queue = nullptr;

	stats::finalize();
}

}} // namespace handystats::message_queue
