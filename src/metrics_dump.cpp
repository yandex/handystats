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

#include <mutex>
#include <string>
#include <map>

#include <handystats/chrono.hpp>

#include "metrics_dump_impl.hpp"


namespace handystats { namespace metrics_dump {

const std::shared_ptr<const metrics_dump_type>
get_dump(core_t& core)
{
	std::lock_guard<std::mutex> lock(core.m_dump_mutex);
	return core.m_dump;
}

static
std::shared_ptr<const metrics_dump_type>
create_dump(core_t& core, const chrono::time_point& current_timestamp)
{
	auto dump_start_time = chrono::internal_clock::now();

	std::shared_ptr<metrics_dump_type> new_dump(new metrics_dump_type());

	auto& metrics_dump = new_dump->first;
	for (auto metric_iter = core.m_metrics.begin(); metric_iter != core.m_metrics.end(); ++metric_iter) {
		switch (metric_iter->second.which()) {
			case metrics::metric_index::GAUGE:
				boost::get<metrics::gauge*>(metric_iter->second)->update_statistics(core.m_internal_timestamp);
				metrics_dump.insert(
						std::pair<std::string, statistics>(
							metric_iter->first,
							boost::get<metrics::gauge*>(metric_iter->second)->values()
						)
					);
				break;
			case metrics::metric_index::COUNTER:
				boost::get<metrics::counter*>(metric_iter->second)->update_statistics(core.m_internal_timestamp);
				metrics_dump.insert(
						std::pair<std::string, statistics>(
							metric_iter->first,
							boost::get<metrics::counter*>(metric_iter->second)->values()
						)
					);
				break;
			case metrics::metric_index::TIMER:
				boost::get<metrics::timer*>(metric_iter->second)->update_statistics(core.m_internal_timestamp);
				metrics_dump.insert(
						std::pair<std::string, statistics>(
							metric_iter->first,
							boost::get<metrics::timer*>(metric_iter->second)->values()
						)
					);
				break;
		}
	}

	auto& attrs_dump = new_dump->second;
	attrs_dump = core.m_attributes;

	core.m_stats.update(current_timestamp);
	core.m_channel.m_stats.update(current_timestamp);
	core.m_pool.m_stats.update(current_timestamp);

	// handystats' statistics
	{
		// core
		{
			metrics_dump.insert(
					std::pair<std::string, statistics>(
						"handystats.core.process_time",
						core.m_stats.process_time
						)
					);
		}

		// message queue
		{
			metrics_dump.insert(
					std::pair<std::string, statistics>(
						"handystats.message_queue.size",
						core.m_channel.m_stats.size
						)
					);

			metrics_dump.insert(
					std::pair<std::string, statistics>(
						"handystats.message_queue.message_wait_time",
						core.m_channel.m_stats.message_wait_time
						)
					);

			metrics_dump.insert(
					std::pair<std::string, statistics>(
						"handystats.message_queue.pop_count",
						core.m_channel.m_stats.pop_count
						)
					);
		}

		// memory pool
		{
			metrics_dump.insert(
					std::pair<std::string, statistics>(
						"handystats.memory.free_list_size",
						core.m_pool.m_stats.free_list_size
						)
					);
		}
	}

	// current system time
	{
		// NOTE: possible call chrono::system_clock::now()
		const auto& timestamp = chrono::time_point::convert_to(chrono::clock_type::SYSTEM_CLOCK, current_timestamp);

		attribute timestamp_attr;
		timestamp_attr.set(timestamp.time_since_epoch().count(chrono::time_unit::MSEC));

		attrs_dump.insert(
				std::pair<std::string, attribute>(
					"handystats.system_timestamp",
					timestamp_attr
				)
			);
	}

	// current internal time
	{
		const auto& timestamp = chrono::time_point::convert_to(chrono::clock_type::SYSTEM_CLOCK, core.m_internal_timestamp);

		attribute timestamp_attr;
		timestamp_attr.set(timestamp.time_since_epoch().count(chrono::time_unit::MSEC));

		attrs_dump.insert(
				std::pair<std::string, attribute>(
					"handystats.internal_timestamp",
					timestamp_attr
				)
			);
	}

	auto dump_end_time = chrono::internal_clock::now();

	core.m_stats.dump_time.update(
			(dump_end_time - dump_start_time).count(metrics::timer::value_unit),
			current_timestamp
		);

	{
		metrics_dump.insert(
				std::pair<std::string, statistics>(
					"handystats.core.dump_time",
					core.m_stats.dump_time
				)
			);
	}

	return std::const_pointer_cast<const metrics_dump_type>(new_dump);
}

void update_dump(core_t& core, const chrono::time_point& current_timestamp) {
	if (core.m_opts->m_dump_interval.count() == 0) {
		return;
	}

	if (current_timestamp - core.m_dump_timestamp > core.m_opts->m_dump_interval) {
		auto new_dump = create_dump(core, current_timestamp);
		{
			std::lock_guard<std::mutex> lock(core.m_dump_mutex);
			core.m_dump = new_dump;
		}

		core.m_dump_timestamp = current_timestamp;
	}
}

}} // namespace handystats::metrics_dump

const std::shared_ptr<const handystats::metrics_dump_type> HANDY_METRICS_DUMP() {
	if (handystats::core) {
		return handystats::metrics_dump::get_dump(*handystats::core);
	}
	else {
		return std::shared_ptr<const handystats::metrics_dump_type>(new handystats::metrics_dump_type());
	}
}
