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

#ifndef HANDYSTATS_CORE_IMPL_HPP_
#define HANDYSTATS_CORE_IMPL_HPP_

#include <handystats/atomic.hpp>
#include <map>
#include <string>
#include <mutex>
#include <memory>
#include <thread>

#include <handystats/metrics.hpp>
#include <handystats/metrics_dump.hpp>

#include "config_impl.hpp"
#include "message_queue_impl.hpp"
#include "memory/pool_impl.hpp"

namespace handystats {

struct core_t {
	struct stats {
		statistics process_time;
		statistics dump_time;

		stats();
		void update(const chrono::time_point&);
	};

	// initialization & finalization
	core_t();
	~core_t();

	void run();

	// configuration
	std::shared_ptr<config::opts_t> m_opts;

	// enabled flag
	std::atomic<bool> m_enabled_flag;

	// message_block pool
	memory::pool m_pool;

	// channel (originally message queue)
	message_queue m_channel;

	// metrics & attributes
	std::map<std::string, metrics::metric_ptr_variant> m_metrics;
	std::map<std::string, attribute> m_attributes;

	// dump
	chrono::time_point m_dump_timestamp;
	std::mutex m_dump_mutex;
	std::shared_ptr<const metrics_dump_type> m_dump;

	// processing thread
	std::thread m_thread;

	// internal time trace
	chrono::time_point m_internal_timestamp;

	// stats
	stats m_stats;
};

extern std::unique_ptr<core_t> core;

} // namespace handystats


#endif // HANDYSTATS_CORE_IMPL_HPP_
