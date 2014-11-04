// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_MESSAGE_QUEUE_IMPL_HPP_
#define HANDYSTATS_MESSAGE_QUEUE_IMPL_HPP_

#include <handystats/atomic.hpp>
#include <handystats/chrono.hpp>

#include <handystats/detail/statistics.hpp>

namespace handystats { namespace events {

struct event_message;

}} // namespace handystats::events

namespace handystats {

/*
 * http://www.1024cores.net/home/lock-free-algorithms/queues/intrusive-mpsc-node-based-queue
 */
struct message_queue
{
	struct node {
		std::atomic<events::event_message*> next;
	};

	struct stats {
		statistics::data size;
		statistics::data message_wait_time;
		statistics::data pop_count;

		stats();
		void update(const chrono::time_point&);
	};

	message_queue();
	~message_queue();

	void push_impl(node* n);
	node* pop_impl();

	void push(events::event_message* message);
	events::event_message* pop();

	size_t size() const;

	std::atomic<node*> m_head_node;
	node* m_tail_node;
	node m_stub_node;

	std::atomic<size_t> m_size;

	stats m_stats;
};

} // namespace handystats

#endif // HANDYSTATS_MESSAGE_QUEUE_IMPL_HPP_
