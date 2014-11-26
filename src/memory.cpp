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

#include <new>

#include <handystats/common.h>

#include "core_impl.hpp"

#include "memory/message_block_impl.hpp"
#include "memory/pool_impl.hpp"

namespace handystats { namespace memory {

message_block::message_block() {
	m_allocated = 0;
	m_processed = 0;
}

events::event_message* message_block::allocate() {
	m_messages[m_allocated].block = this;
	return &m_messages[m_allocated++];
}

void message_block::acknowledge(const events::event_message*) {
	m_processed++;
}

void local_block_cleanup(message_block* block) {
	// condition: block is not NULL (POSIX guarantees)
	// thus block is not exhausted
	if (core) {
		// remaining messages from the block can be reused
		// that's why m_allocated is not reseted
		std::lock_guard<std::mutex> guard(core->m_pool.m_free_list_lock);
		core->m_pool.m_free_list.push_front(block);
		core->m_pool.m_stats.free_list_size.update(core->m_pool.m_free_list.size(), chrono::internal_clock::now());
	}
	else {
		// this thread "owns" block
		delete block;
	}
}

pool::stats::stats() {
	{
		config::statistics config;
		config.tags = statistics::tag::value | statistics::tag::max | statistics::tag::moving_avg;
		config.moving_interval = chrono::seconds(1);

		free_list_size = statistics(config);

		free_list_size.update(0);
	}
}

void pool::stats::update(const chrono::time_point& timestamp) {
	free_list_size.update_time(timestamp);
}

pool::~pool() {
	free();
}

void pool::free() {
	// condition: core is inactive, no thread will call allocation
	// thus, no need to acquire lock

	for (auto iter = m_free_list.begin(); iter != m_free_list.end(); ++iter) {
		delete *iter;
	}

	m_free_list.clear();
}

message_block* pool::acquire() {
	{
		std::lock_guard<std::mutex> guard(m_free_list_lock);
		if (!m_free_list.empty()) {
			auto* block = m_free_list.front();
			m_free_list.pop_front();
			m_stats.free_list_size.update(m_free_list.size(), chrono::internal_clock::now());
			return block;
		}
	}

	// free list is empty
	// allocate new block from system
	auto* block = new message_block();
	return block;
}

void pool::acknowledge(const events::event_message* message) {
	auto* block = message->block;
	block->acknowledge(message);

	if (block->m_processed == message_block::BLOCK_SIZE) {
		// block is exhausted and can be reused
		std::lock_guard<std::mutex> guard(m_free_list_lock);
		block->m_allocated = 0;
		block->m_processed = 0;

		m_free_list.push_back(block);

		m_stats.free_list_size.update(m_free_list.size(), chrono::internal_clock::now());
	}
}

}} // namespace handystats::memory
