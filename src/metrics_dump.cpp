// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <mutex>
#include <string>
#include <map>

#include <handystats/chrono.hpp>
#include <handystats/metrics_dump.hpp>

#include "internal_impl.hpp"
#include "message_queue_impl.hpp"
#include "json_dump_impl.hpp"

#include "configuration_impl.hpp"

#include "metrics_dump_impl.hpp"


namespace handystats { namespace metrics_dump {


namespace stats {

metrics::gauge dump_time;

void initialize() {
	dump_time = metrics::gauge();
}

void finalize() {
	dump_time = metrics::gauge();
}

} // namespace stats


chrono::clock::time_point dump_timestamp;
std::mutex dump_mutex;

std::shared_ptr<const std::map<std::string, metrics::metric_variant>> dump(new std::map<std::string, metrics::metric_variant>());

const std::shared_ptr<const std::map<std::string, metrics::metric_variant>>
get_dump()
{
	std::lock_guard<std::mutex> lock(dump_mutex);
	return dump;
}

std::shared_ptr<const std::map<std::string, metrics::metric_variant>>
create_dump()
{
	auto dump_start_time = chrono::clock::now();

	std::shared_ptr<std::map<std::string, metrics::metric_variant>> new_dump(new std::map<std::string, metrics::metric_variant>());

	for (const auto& metric_entry : internal::metrics_map) {
		switch (metric_entry.second.which()) {
			case metrics::metric_index::GAUGE:
				new_dump->insert(
						std::pair<std::string, metrics::metric_variant>(
							metric_entry.first,
							*boost::get<metrics::gauge*>(metric_entry.second)
						)
					);
				break;
			case metrics::metric_index::COUNTER:
				new_dump->insert(
						std::pair<std::string, metrics::metric_variant>(
							metric_entry.first,
							*boost::get<metrics::counter*>(metric_entry.second)
						)
					);
				break;
			case metrics::metric_index::TIMER:
				new_dump->insert(
						std::pair<std::string, metrics::metric_variant>(
							metric_entry.first,
							*boost::get<metrics::timer*>(metric_entry.second)
						)
					);
				break;
		}
	}

	// handystats' statistics
	{
		// internal
		{
			new_dump->insert(
					std::pair<std::string, metrics::metric_variant>(
						"handystats.internal.size",
						internal::stats::size
						)
					);

			new_dump->insert(
					std::pair<std::string, metrics::metric_variant>(
						"handystats.internal.process_time",
						internal::stats::process_time
						)
					);
		}

		// message queue
		{
			new_dump->insert(
					std::pair<std::string, metrics::metric_variant>(
						"handystats.message_queue.size",
						message_queue::stats::size
						)
					);

			new_dump->insert(
					std::pair<std::string, metrics::metric_variant>(
						"handystats.message_queue.message_wait_time",
						message_queue::stats::message_wait_time
						)
					);

			new_dump->insert(
					std::pair<std::string, metrics::metric_variant>(
						"handystats.message_queue.pop_count",
						message_queue::stats::pop_count
						)
					);
		}

		// dump
		{
			new_dump->insert(
					std::pair<std::string, metrics::metric_variant>(
						"handystats.metrics_dump.dump_time",
						stats::dump_time
						)
					);

			new_dump->insert(
					std::pair<std::string, metrics::metric_variant>(
						"handystats.json_dump.dump_time",
						json_dump::stats::dump_time
						)
					);
		}
	}

	dump_timestamp = chrono::clock::now();

	stats::dump_time.set(chrono::duration_cast<chrono::time_duration>(dump_timestamp - dump_start_time).count(), dump_timestamp);

	return std::const_pointer_cast<const std::map<std::string, metrics::metric_variant>>(new_dump);
}

void update() {
	if (config::metrics_dump.interval.count() == 0) {
		return;
	}
	if (chrono::duration_cast<std::chrono::nanoseconds>(chrono::clock::now() - dump_timestamp) > config::metrics_dump.interval) {
		auto new_dump = create_dump();
		{
			std::lock_guard<std::mutex> lock(dump_mutex);
			dump = new_dump;
		}

		if (config::metrics_dump.to_json) {
			json_dump::update();
		}
	}
}

void initialize() {
	stats::initialize();

	{
		std::lock_guard<std::mutex> lock(dump_mutex);

		dump_timestamp = chrono::clock::time_point();
		dump = std::shared_ptr<const std::map<std::string, metrics::metric_variant>>(new std::map<std::string, metrics::metric_variant>());
	}

	json_dump::initialize();
}

void finalize() {
	stats::finalize();

	{
		std::lock_guard<std::mutex> lock(dump_mutex);

		dump_timestamp = chrono::clock::time_point();
		dump = std::shared_ptr<const std::map<std::string, metrics::metric_variant>>(new std::map<std::string, metrics::metric_variant>());
	}

	json_dump::finalize();
}

}} // namespace handystats::metrics_dump

const std::shared_ptr<const std::map<std::string, handystats::metrics::metric_variant>> HANDY_METRICS_DUMP() {
	return handystats::metrics_dump::get_dump();
}
