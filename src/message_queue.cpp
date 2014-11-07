// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/timer.hpp>

#include "events/event_message_impl.hpp"
#include "message_queue_impl.hpp"

namespace handystats {

message_queue::stats::stats() {
	{
		config::statistics config;
		config.tags = statistics::tag::value | statistics::tag::max | statistics::tag::moving_avg;
		config.moving_interval = chrono::seconds(1);

		size = statistics::data(config);

		size.update(0);
	}

	{
		config::statistics config;
		config.tags = statistics::tag::moving_avg;
		config.moving_interval = chrono::seconds(1);

		message_wait_time = statistics::data(config);
	}

	{
		config::statistics config;
		config.tags = statistics::tag::throughput;
		config.moving_interval = chrono::seconds(1);

		pop_count = statistics::data(config);
	}
}

void message_queue::stats::update(const chrono::time_point& timestamp) {
	size.update_time(timestamp);
	message_wait_time.update_time(timestamp);
	pop_count.update_time(timestamp);
}

message_queue::message_queue()
	: m_head_node(&m_stub_node)
	, m_tail_node(&m_stub_node)
	, m_size(0)
{
	m_stub_node.next.store(nullptr, std::memory_order_release);
}

message_queue::~message_queue() {
	while (size() > 0) {
		auto* message = pop();
		events::delete_event_message(message);
	}
}

void message_queue::push_impl(node* n) {
	n->next.store(nullptr, std::memory_order_release);
	node* prev = m_head_node.exchange(n, std::memory_order_acquire);
	prev->next.store(static_cast<events::event_message*>(n), std::memory_order_release);
}

message_queue::node* message_queue::pop_impl() {
	node* tail = m_tail_node;
	node* next = tail->next.load(std::memory_order_acquire);

	if (tail == &m_stub_node) {
		if (next == nullptr) {
			return nullptr;
		}

		m_tail_node = next;
		tail = next;
		next = next->next.load(std::memory_order_acquire);
	}

	if (next) {
		m_tail_node = next;
		return tail;
	}

	node* head = m_head_node.load(std::memory_order_acquire);

	if (tail != head) {
		return nullptr;
	}

	push_impl(&m_stub_node);

	next = tail->next.load(std::memory_order_acquire);

	if (next) {
		m_tail_node = next;
		return tail;
	}

	return nullptr;
}

void message_queue::push(events::event_message* message) {
	push_impl(static_cast<node*>(message));
	m_size++;

	// update stats
	{
		m_stats.size.update(size());
	}
}

events::event_message* message_queue::pop() {
	if (size() == 0) {
		return nullptr;
	}

	node* n = nullptr;
	while (!n) {
		n = pop_impl();
		// XXX: backoff strategy?
		// if size() > 0 there's at least one message to process
		// then if popped message is nullptr there're pushing threads in action
		// backoff strategy could be used to work around such thread contention
	}

	m_size--;

	auto* message = static_cast<events::event_message*>(n);

	// update stats
	{
		auto current_time = statistics::clock::now();

		m_stats.pop_count.update(1, current_time);
		m_stats.size.update(size(), current_time);
		m_stats.message_wait_time.update(
				(current_time - message->timestamp)
					.count(metrics::timer::value_unit),
				current_time
			);
	}

	return message;
}

size_t message_queue::size() const {
	return m_size.load(std::memory_order_acquire);
}

} // namespace handystats
