#include <memory>
#include <atomic>

#include "events/event_message_impl.hpp"
#include "system_stats_impl.hpp"


namespace handystats { namespace message_queue {


/*
 * Taken from Boost's Atomic usage examples.
 */
template <typename T>
struct waitfree_queue
{
	struct node
	{
		node* next;
		T data;
	};

	void push(const T& data)
	{
		node* n = new node;
		n->data = data;
		node* stale_head = head_.load(std::memory_order_relaxed);
		do {
			n->next = stale_head;
		}
		while (!head_.compare_exchange_weak(stale_head, n, std::memory_order_release));
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
		return head_.exchange(0, std::memory_order_acquire);
	}

	waitfree_queue() : head_(0) {}

	bool empty() const {
		return !head_.load();
	}

	std::atomic<node*> head_;
};

waitfree_queue<std::shared_ptr<events::event_message>>* event_message_queue = nullptr;
waitfree_queue<std::shared_ptr<events::event_message>>::node* popped_head = nullptr;

std::atomic<size_t> size_(0);

void push_event_message(std::shared_ptr<events::event_message> message) {
	if (event_message_queue) {
		event_message_queue->push(message);
		++size_;
	}
}

std::shared_ptr<events::event_message> pop_event_message() {
	std::shared_ptr<events::event_message> message;

	if (!popped_head && event_message_queue) {
		popped_head = event_message_queue->pop_all();
	}

	if (popped_head) {
		auto tmp = popped_head;
		message = popped_head->data;
		--size_;

		popped_head = popped_head->next;
		delete tmp;
	}

	return message;
}

bool empty() {
//	return (!event_message_queue || event_message_queue->empty()) && !popped_head;
	return size_.load() == 0;
}

size_t size() {
	return size_.load();
}

void initialize() {
	if (!event_message_queue) {
		event_message_queue = new waitfree_queue<std::shared_ptr<events::event_message>>();
		size_.store(0);
	}
}

void finalize() {
	while (!empty()) {
		pop_event_message();
	}
	size_.store(0);

	delete event_message_queue;
	event_message_queue = nullptr;
}

}} // namespace handystats::message_queue
