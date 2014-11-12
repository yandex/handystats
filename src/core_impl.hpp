// Copyright (c) 2014 Yandex LLC. All rights reserved.

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
