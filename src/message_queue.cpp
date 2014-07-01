// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <memory>
#include <atomic>
#include <algorithm>

#include "events/event_message_impl.hpp"


namespace handystats { namespace message_queue {


/*
 * Taken from Boost's Atomic usage examples.
 */
template <typename DataType>
struct waitfree_queue
{
	typedef DataType data_type;

	struct node
	{
		node(data_type&& data, node* next = nullptr)
			: data(std::move(data))
			, next(next)
		{}

		data_type data;
		node* next;
	};

	waitfree_queue() : head_node(nullptr) {}

	void push(data_type&& data)
	{
		node* n = new node(std::move(data));
		node* stale_head = head_node.load(std::memory_order_relaxed);
		do {
			n->next = stale_head;
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
			tmp->next = first;
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

	~waitfree_queue() {
		// what if queue is not empty?
		// this will be memory leak for sure
	}

	std::atomic<node*> head_node;
};

waitfree_queue<events::event_message_ptr>* event_message_queue = nullptr;
waitfree_queue<events::event_message_ptr>::node* popped_head = nullptr;

std::atomic<size_t> mq_size(0);

void push_event_message(events::event_message_ptr&& message) {
	if (event_message_queue) {
		event_message_queue->push(std::move(message));
		++mq_size;
	}
}

events::event_message_ptr pop_event_message() {
	events::event_message_ptr message;

	if (!popped_head && event_message_queue) {
		popped_head = event_message_queue->pop_all();
	}

	if (popped_head) {
		auto* tmp = popped_head;
		message = std::move(popped_head->data);
		--mq_size;

		popped_head = popped_head->next;
		delete tmp;
	}

	return std::move(message);
}

bool empty() {
	return mq_size.load(std::memory_order_acquire) == 0;
}

size_t size() {
	return mq_size.load(std::memory_order_acquire);
}

void initialize() {
	if (!event_message_queue) {
		event_message_queue = new waitfree_queue<events::event_message_ptr>();
		mq_size.store(0, std::memory_order_release);
	}
}

void finalize() {
	while (!empty()) {
		pop_event_message();
	}
	mq_size.store(0);

	delete event_message_queue;
	event_message_queue = nullptr;
}

}} // namespace handystats::message_queue
