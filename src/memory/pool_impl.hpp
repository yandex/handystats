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

#ifndef HANDYSTATS_MEMORY_POOL_IMPL_HPP_
#define HANDYSTATS_MEMORY_POOL_IMPL_HPP_

#include <deque>
#include <mutex>

#include <handystats/chrono.hpp>
#include <handystats/statistics.hpp>

#include "memory/message_block_impl.hpp"

namespace handystats { namespace memory {

struct pool
{
	struct stats {
		statistics free_list_size;

		stats();
		void update(const chrono::time_point&);
	};

	~pool();

	message_block* acquire();
	void acknowledge(const events::event_message*);
	void free();

	std::deque<message_block*> m_free_list;
	std::mutex m_free_list_lock;

	stats m_stats;
};

}} // namespace handystats::memory

#endif // HANDYSTATS_MEMORY_POOL_IMPL_HPP_
