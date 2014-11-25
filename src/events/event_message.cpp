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

#include <boost/thread/tss.hpp>

#include <handystats/common.h>

#include "events/gauge_impl.hpp"
#include "events/counter_impl.hpp"
#include "events/timer_impl.hpp"
#include "events/attribute_impl.hpp"

#include "memory/message_block_impl.hpp"
#include "memory/pool_impl.hpp"

#include "core_impl.hpp"

#include "events/event_message_impl.hpp"


namespace handystats { namespace events {

event_message* allocate_event_message() {
	static boost::thread_specific_ptr<memory::message_block> local_block(memory::local_block_cleanup);

	auto* block = local_block.get();
	if (!block) {
		block = core->m_pool.acquire();
		local_block.reset(block);
	}

	auto* message = block->allocate();

	if (block->m_allocated == memory::message_block::BLOCK_SIZE) {
		local_block.release();
	}

	return message;
}

void process_event_message(metrics::metric_ptr_variant& metric_ptr, const event_message& message) {
	switch (metric_ptr.which()) {
		case metrics::metric_index::COUNTER:
			counter::process_event(*boost::get<metrics::counter*>(metric_ptr), message);
			break;
		case metrics::metric_index::GAUGE:
			gauge::process_event(*boost::get<metrics::gauge*>(metric_ptr), message);
			break;
		case metrics::metric_index::TIMER:
			timer::process_event(*boost::get<metrics::timer*>(metric_ptr), message);
			break;
		default:
			return;
	}
}

void process_event_message(::handystats::attribute& attr, const event_message& message) {
	attribute::process_event(attr, message);
}


void delete_event_message(event_message* message) {
	if (!message) {
		return;
	}

	switch (message->destination_type) {
		case event_destination_type::COUNTER:
			counter::delete_event(message);
			break;
		case event_destination_type::GAUGE:
			gauge::delete_event(message);
			break;
		case event_destination_type::TIMER:
			timer::delete_event(message);
			break;
		case event_destination_type::ATTRIBUTE:
			attribute::delete_event(message);
			break;
		default:
			return;
	}
}

}} // namespace handystats::events
