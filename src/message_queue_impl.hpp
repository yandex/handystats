/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
		statistics size;
		statistics message_wait_time;
		statistics pop_count;

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
