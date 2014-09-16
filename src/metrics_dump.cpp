// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <mutex>
#include <string>
#include <map>

#include <handystats/chrono.hpp>
#include <handystats/metrics_dump.hpp>

#include "internal_impl.hpp"
#include "message_queue_impl.hpp"
#include "json_dump_impl.hpp"

#include "config_impl.hpp"

#include "metrics_dump_impl.hpp"


namespace handystats { namespace metrics_dump {


namespace stats {

metrics::gauge dump_time;

static void reset() {
	config::metrics::gauge dump_time_opts;
	dump_time_opts.values.tags = statistics::tag::moving_avg;
	dump_time_opts.values.moving_interval = chrono::duration_cast<chrono::clock::duration>(std::chrono::seconds(1));

	dump_time = metrics::gauge(dump_time_opts);
}

void initialize() {
	reset();
}

void finalize() {
	reset();
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

static
std::shared_ptr<const std::map<std::string, metrics::metric_variant>>
create_dump(const chrono::clock::time_point& timestamp)
{
	auto dump_start_time = chrono::clock::now();

	// update time
	for (auto metric_iter = internal::metrics_map.begin(); metric_iter != internal::metrics_map.end(); ++metric_iter) {
		switch (metric_iter->second.which()) {
			case metrics::metric_index::GAUGE:
			{
				auto* gauge = boost::get<metrics::gauge*>(metric_iter->second);
				gauge->update_statistics(timestamp);
				break;
			}
			case metrics::metric_index::COUNTER:
			{
				auto* counter = boost::get<metrics::counter*>(metric_iter->second);
				counter->update_statistics(timestamp);
				break;
			}
			case metrics::metric_index::TIMER:
			{
				auto* timer = boost::get<metrics::timer*>(metric_iter->second);
				timer->update_statistics(timestamp);
				break;
			}
			case metrics::metric_index::ATTRIBUTE:
				break;
		}
	}
	// handystats' statistics
	{
		// internal
		{
			internal::stats::size.update_statistics(timestamp);
			internal::stats::process_time.update_statistics(timestamp);
		}

		// message queue
		{
			message_queue::stats::size.update_statistics(timestamp);
			message_queue::stats::message_wait_time.update_statistics(timestamp);
			message_queue::stats::pop_count.update_statistics(timestamp);
		}

		// dump
		{
			stats::dump_time.update_statistics(timestamp);
			json_dump::stats::dump_time.update_statistics(timestamp);
		}
	}

	std::shared_ptr<std::map<std::string, metrics::metric_variant>> new_dump(new std::map<std::string, metrics::metric_variant>());

	for (auto metric_iter = internal::metrics_map.cbegin(); metric_iter != internal::metrics_map.cend(); ++metric_iter) {
		switch (metric_iter->second.which()) {
			case metrics::metric_index::GAUGE:
				new_dump->insert(
						std::pair<std::string, metrics::metric_variant>(
							metric_iter->first,
							*boost::get<metrics::gauge*>(metric_iter->second)
						)
					);
				break;
			case metrics::metric_index::COUNTER:
				new_dump->insert(
						std::pair<std::string, metrics::metric_variant>(
							metric_iter->first,
							*boost::get<metrics::counter*>(metric_iter->second)
						)
					);
				break;
			case metrics::metric_index::TIMER:
				new_dump->insert(
						std::pair<std::string, metrics::metric_variant>(
							metric_iter->first,
							*boost::get<metrics::timer*>(metric_iter->second)
						)
					);
				break;
			case metrics::metric_index::ATTRIBUTE:
				new_dump->insert(
						std::pair<std::string, metrics::metric_variant>(
							metric_iter->first,
							*boost::get<metrics::attribute*>(metric_iter->second)
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

	dump_timestamp = timestamp;

	{
		std::chrono::system_clock::time_point system_timestamp = chrono::to_system_time(dump_timestamp);

		metrics::attribute timestamp_attr;
		timestamp_attr.set(
				chrono::duration_cast<std::chrono::milliseconds>(system_timestamp.time_since_epoch()).count()
			);

		new_dump->insert(
				std::pair<std::string, metrics::metric_variant>(
					"handystats.dump_timestamp",
					timestamp_attr
					)
				);
	}

	auto dump_end_time = chrono::clock::now();

	stats::dump_time.set(chrono::duration_cast<chrono::time_duration>(dump_end_time - dump_start_time).count(), dump_timestamp);

	return std::const_pointer_cast<const std::map<std::string, metrics::metric_variant>>(new_dump);
}

void update(const chrono::clock::time_point& timestamp) {
	if (config::metrics_dump_opts.interval.count() == 0) {
		return;
	}

	if (timestamp - dump_timestamp > config::metrics_dump_opts.interval) {
		auto new_dump = create_dump(timestamp);
		{
			std::lock_guard<std::mutex> lock(dump_mutex);
			dump = new_dump;
		}

		if (config::metrics_dump_opts.to_json) {
			json_dump::update(timestamp);
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
