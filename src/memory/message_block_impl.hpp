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

#ifndef HANDYSTATS_MEMORY_MESSAGE_BLOCK_IMPL_HPP_
#define HANDYSTATS_MEMORY_MESSAGE_BLOCK_IMPL_HPP_

#include "events/event_message_impl.hpp"

namespace handystats { namespace memory {

struct message_block
{
	static const size_t BLOCK_SIZE = 1 << 10;

	message_block();

	events::event_message* allocate();
	void acknowledge(const events::event_message*);

	// members' layout tries to avoid false sharing
	// between m_allocated and m_processed

	// number of allocated messages
	uint64_t m_allocated;

	events::event_message m_messages[BLOCK_SIZE];

	// number of processed messages
	uint64_t m_processed;
};

void local_block_cleanup(message_block*);

}} // namespace handystats::memory

#endif // HANDYSTATS_MEMORY_MESSAGE_BLOCK_IMPL_HPP_
