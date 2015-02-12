// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <mutex>
#include <string>
#include <map>

#include <handystats/chrono.hpp>
#include <handystats/metrics_dump.hpp>

#include "internal_impl.hpp"
#include "message_queue_impl.hpp"

#include "config_impl.hpp"

#include "metrics_dump_impl.hpp"


namespace handystats { namespace metrics_dump {


namespace stats {

metrics::gauge dump_time;

void update(const chrono::time_point& timestamp) {
	dump_time.update_statistics(timestamp);
}

static void reset() {
	config::metrics::gauge dump_time_opts;
	dump_time_opts.values.tags = statistics::tag::moving_avg;
	dump_time_opts.values.moving_interval = chrono::duration(1, chrono::time_unit::SEC);

	dump_time = metrics::gauge(dump_time_opts);
}

void initialize() {
	reset();
}

void finalize() {
	reset();
}

} // namespace stats


chrono::time_point dump_timestamp;
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
create_dump()
{
	auto dump_start_time = chrono::tsc_clock::now();

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
						"handystats.message_queue.mem_consume",
						message_queue::stats::mem_consume
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

		// metrics_dump.dump_time will be added later
	}

	{
		// NOTE: possible call chrono::system_clock::now()
		chrono::time_point system_timestamp =
			chrono::time_point::convert_to(chrono::clock_type::SYSTEM, chrono::tsc_clock::now());

		metrics::attribute timestamp_attr;
		timestamp_attr.set(
				chrono::duration::convert_to(chrono::time_unit::MSEC, system_timestamp.time_since_epoch()).count()
			);

		new_dump->insert(
				std::pair<std::string, metrics::metric_variant>(
					"handystats.dump_timestamp",
					timestamp_attr
					)
				);
	}

	auto dump_end_time = chrono::tsc_clock::now();

	stats::dump_time.set(
			chrono::duration::convert_to(metrics::timer::value_unit, dump_end_time - dump_start_time).count(),
			dump_end_time
		);

	{
		new_dump->insert(
				std::pair<std::string, metrics::metric_variant>(
					"handystats.metrics_dump.dump_time",
					stats::dump_time
					)
				);
	}

	return std::const_pointer_cast<const std::map<std::string, metrics::metric_variant>>(new_dump);
}

void update(const chrono::time_point& system_time, const chrono::time_point& internal_time) {
	if (config::metrics_dump_opts.interval.count() == 0) {
		return;
	}

	if (system_time - dump_timestamp > config::metrics_dump_opts.interval) {
		internal::update_metrics(internal_time);

		internal::stats::update(system_time);
		message_queue::stats::update(system_time);
		stats::update(system_time);

		auto new_dump = create_dump();
		{
			std::lock_guard<std::mutex> lock(dump_mutex);
			dump = new_dump;
		}

		dump_timestamp = system_time;
	}
}

void initialize() {
	stats::initialize();

	{
		std::lock_guard<std::mutex> lock(dump_mutex);

		dump_timestamp = chrono::time_point();
		dump = std::shared_ptr<const std::map<std::string, metrics::metric_variant>>(new std::map<std::string, metrics::metric_variant>());
	}
}

void finalize() {
	stats::finalize();

	{
		std::lock_guard<std::mutex> lock(dump_mutex);

		dump_timestamp = chrono::time_point();
		dump = std::shared_ptr<const std::map<std::string, metrics::metric_variant>>(new std::map<std::string, metrics::metric_variant>());
	}
}

}} // namespace handystats::metrics_dump

const std::shared_ptr<const std::map<std::string, handystats::metrics::metric_variant>> HANDY_METRICS_DUMP() {
	return handystats::metrics_dump::get_dump();
}
