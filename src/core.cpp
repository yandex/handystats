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

#include <chrono>
#include <algorithm>
#include <thread>
#include <sys/prctl.h>
#include <handystats/atomic.hpp>

#include <handystats/chrono.hpp>
#include <handystats/core.hpp>
#include <handystats/core.h>

#include "events/event_message_impl.hpp"
#include "metrics_dump_impl.hpp"
#include "config_impl.hpp"

#include "core_impl.hpp"


namespace handystats {

core_t::stats::stats() {
	{
		config::statistics config;
		config.tags = statistics::tag::moving_avg;
		config.moving_interval = chrono::seconds(1);

		process_time = statistics(config);
	}

	{
		config::statistics config;
		config.tags = statistics::tag::moving_avg;
		config.moving_interval = chrono::seconds(1);

		dump_time = statistics(config);
	}
}

void core_t::stats::update(const chrono::time_point& timestamp) {
	process_time.update_time(timestamp);
	dump_time.update_time(timestamp);
}

static
void process_event_message(core_t& core, const events::event_message& message) {
	auto process_start_time = chrono::internal_clock::now();

	if (message.destination_type == events::event_destination_type::ATTRIBUTE) {
		auto& attr = core.m_attributes[message.destination_name];
		process_event_message(attr, message);
	}
	else {
		auto metric_iter = core.m_metrics.find(message.destination_name);

		if (metric_iter == core.m_metrics.end()) {
			std::pair<std::string, metrics::metric_ptr_variant> insert_value;
			insert_value.first = message.destination_name;

			rapidjson::Value* pattern_cfg = core.m_opts->select_pattern(message.destination_name);

			switch (message.destination_type) {
				case events::event_destination_type::GAUGE:
					{
						config::metrics::gauge opts = core.m_opts->m_gauge;
						if (pattern_cfg) {
							config::apply(*pattern_cfg, opts);
						}
						insert_value.second = new metrics::gauge(opts);
						break;
					}
					break;
				case events::event_destination_type::COUNTER:
					{
						config::metrics::counter opts = core.m_opts->m_counter;
						if (pattern_cfg) {
							config::apply(*pattern_cfg, opts);
						}
						insert_value.second = new metrics::counter(opts);
						break;
					}
				case events::event_destination_type::TIMER:
					{
						config::metrics::timer opts = core.m_opts->m_timer;
						if (pattern_cfg) {
							config::apply(*pattern_cfg, opts);
						}
						insert_value.second = new metrics::timer(opts);
						break;
					}
					break;
			}

			metric_iter = core.m_metrics.insert(insert_value).first;
		}

		events::process_event_message(metric_iter->second, message);
	}

	auto process_end_time = chrono::internal_clock::now();

	core.m_stats.process_time.update(
			(process_end_time - process_start_time).count(metrics::timer::value_unit),
			process_end_time
		);
}

static
void process(core_t& core) {
	auto* message = core.m_channel.pop();

	if (message) {
		core.m_internal_timestamp = std::max(core.m_internal_timestamp, message->timestamp);
		process_event_message(core, *message);

		// deletion actually frees event data
		events::delete_event_message(message);

		core.m_pool.acknowledge(message);
	}
}

static
void run_processor(core_t& core) {
	char thread_name[16];
	memset(thread_name, 0, sizeof(thread_name));

	sprintf(thread_name, "handystats");

	prctl(PR_SET_NAME, thread_name);

	while (core.m_enabled_flag.load(std::memory_order_acquire)) {
		if (core.m_channel.size() > 0) {
			process(core);
		}
		else {
			core.m_internal_timestamp = std::max(core.m_internal_timestamp, chrono::internal_clock::now());
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}

		metrics_dump::update_dump(core, chrono::internal_clock::now());
	}
}

core_t::core_t() {
	// dump initialization
	m_dump_timestamp = chrono::internal_clock::now();
	m_dump.reset(new metrics_dump_type());

	// internal time
	m_internal_timestamp = chrono::internal_clock::now();

	// enabled flag
	m_enabled_flag.store(true, std::memory_order_release);

	m_opts = config::opts;

	if (m_opts.use_count() == 0 || !m_opts->m_core_enable) {
		m_enabled_flag.store(false, std::memory_order_release);
		return;
	}
}

core_t::~core_t() {
	// enabled flag
	m_enabled_flag.store(false, std::memory_order_release);

	if (m_thread.joinable()) {
		m_thread.join();
	}

	for (auto metric_iter = m_metrics.begin(); metric_iter != m_metrics.end(); ++metric_iter) {
		switch (metric_iter->second.which()) {
			case metrics::metric_index::COUNTER:
				delete boost::get<metrics::counter*>(metric_iter->second);
				break;
			case metrics::metric_index::GAUGE:
				delete boost::get<metrics::gauge*>(metric_iter->second);
				break;
			case metrics::metric_index::TIMER:
				delete boost::get<metrics::timer*>(metric_iter->second);
				break;
			default:
				break;
		}
	}

	// "process" all remaining messages
	while (m_channel.size() > 0) {
		auto* message = m_channel.pop();
		events::delete_event_message(message);

		m_pool.acknowledge(message);
	}

	// pool's clean up
	m_pool.free();
}

void core_t::run() {
	if (m_enabled_flag.load(std::memory_order_acquire)) {
		m_thread = std::thread(run_processor, std::ref(*this));
	}
}

std::unique_ptr<core_t> core;


void initialize() {
	core.reset(new core_t());
	core->run();
}

void finalize() {
	core.reset();
	config::finalize();
}

} // namespace handystats


extern "C" {

void handystats_initialize() {
	handystats::initialize();
}

void handystats_finalize() {
	handystats::finalize();
}

} // extern "C"
