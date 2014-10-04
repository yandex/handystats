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

std::shared_ptr<const metrics_dump_type> dump(new metrics_dump_type());

const std::shared_ptr<const metrics_dump_type>
get_dump()
{
	std::lock_guard<std::mutex> lock(dump_mutex);
	return dump;
}

static
std::shared_ptr<const metrics_dump_type>
create_dump()
{
	auto dump_start_time = chrono::tsc_clock::now();

	std::shared_ptr<metrics_dump_type> new_dump(new metrics_dump_type());

	auto& metrics_dump = new_dump->first;
	for (auto metric_iter = internal::metrics_map.cbegin(); metric_iter != internal::metrics_map.cend(); ++metric_iter) {
		switch (metric_iter->second.which()) {
			case metrics::metric_index::GAUGE:
				metrics_dump.insert(
						std::pair<std::string, statistics>(
							metric_iter->first,
							boost::get<metrics::gauge*>(metric_iter->second)->values()
						)
					);
				break;
			case metrics::metric_index::COUNTER:
				metrics_dump.insert(
						std::pair<std::string, statistics>(
							metric_iter->first,
							boost::get<metrics::counter*>(metric_iter->second)->values()
						)
					);
				break;
			case metrics::metric_index::TIMER:
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
	attrs_dump = internal::attributes_map;

	// handystats' statistics
	{
		// internal
		{
			metrics_dump.insert(
					std::pair<std::string, statistics>(
						"handystats.internal.process_time",
						internal::stats::process_time.values()
						)
					);
		}

		// message queue
		{
			metrics_dump.insert(
					std::pair<std::string, statistics>(
						"handystats.message_queue.size",
						message_queue::stats::size.values()
						)
					);

			metrics_dump.insert(
					std::pair<std::string, statistics>(
						"handystats.message_queue.message_wait_time",
						message_queue::stats::message_wait_time.values()
						)
					);

			metrics_dump.insert(
					std::pair<std::string, statistics>(
						"handystats.message_queue.pop_count",
						message_queue::stats::pop_count.values()
						)
					);
		}
	}

	{
		// NOTE: possible call chrono::system_clock::now()
		chrono::time_point system_timestamp =
			chrono::time_point::convert_to(chrono::clock_type::SYSTEM, chrono::tsc_clock::now());

		attribute timestamp_attr;
		timestamp_attr.set(
				chrono::duration::convert_to(chrono::time_unit::MSEC, system_timestamp.time_since_epoch()).count()
			);

		attrs_dump.insert(
				std::pair<std::string, attribute>(
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
		metrics_dump.insert(
				std::pair<std::string, statistics>(
					"handystats.metrics_dump.dump_time",
					stats::dump_time.values()
				)
			);
	}

	return std::const_pointer_cast<const metrics_dump_type>(new_dump);
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

static
void reset_dump() {
	std::lock_guard<std::mutex> lock(dump_mutex);

	dump_timestamp = chrono::time_point();
	dump = std::shared_ptr<const metrics_dump_type>(new metrics_dump_type());
}

void initialize() {
	stats::initialize();

	reset_dump();
}

void finalize() {
	stats::finalize();

	reset_dump();
}

}} // namespace handystats::metrics_dump

const std::shared_ptr<const handystats::metrics_dump_type> HANDY_METRICS_DUMP() {
	return handystats::metrics_dump::get_dump();
}
