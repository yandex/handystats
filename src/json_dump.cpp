// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <mutex>
#include <memory>
#include <string>
#include <map>

#include <handystats/rapidjson/document.h>

#include <handystats/chrono.hpp>
#include <handystats/json_dump.hpp>

#include <handystats/json/timestamp.hpp>
#include <handystats/json/gauge_json_writer.hpp>
#include <handystats/json/counter_json_writer.hpp>
#include <handystats/json/timer_json_writer.hpp>

#include "configuration_impl.hpp"

#include "metrics_dump_impl.hpp"

#include "json_dump_impl.hpp"

namespace handystats { namespace json_dump {


namespace stats {

metrics::gauge dump_time;

void initialize() {
	dump_time = metrics::gauge();
}

void finalize() {
	dump_time = metrics::gauge();
}

} // namespace stats


std::mutex dump_mutex;
std::shared_ptr<const std::string> dump(new std::string());

const std::shared_ptr<const std::string>
get_dump() {
	std::lock_guard<std::mutex> lock(dump_mutex);
	return dump;
}

template<typename Allocator>
std::shared_ptr<const std::string> create_dump(Allocator&& allocator = Allocator()) {
	rapidjson::Value dump_value(rapidjson::kObjectType);

	const auto metrics_dump = metrics_dump::get_dump();

	for (const auto& metric_entry : *metrics_dump) {
		rapidjson::Value metric_value;
		switch (metric_entry.second.which()) {
			case metrics::metric_index::GAUGE:
				json::write_to_json_value(&boost::get<metrics::gauge>(metric_entry.second), &metric_value, allocator);
				break;
			case metrics::metric_index::COUNTER:
				json::write_to_json_value(&boost::get<metrics::counter>(metric_entry.second), &metric_value, allocator);
				break;
			case metrics::metric_index::TIMER:
				json::write_to_json_value(&boost::get<metrics::timer>(metric_entry.second), &metric_value, allocator);
				break;
		}

		dump_value.AddMember(metric_entry.first.c_str(), allocator, metric_value, allocator);
	}

	{
		rapidjson::Value timestamp_value;
		json::write_to_json_value(metrics_dump::dump_timestamp, &timestamp_value);
		dump_value.AddMember("handystats.dump_timestamp", timestamp_value, allocator);
	}

	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator>> writer(buffer);
	dump_value.Accept(writer);

	auto dump_end_time = chrono::clock::now();

	return std::shared_ptr<const std::string>(new std::string(buffer.GetString(), buffer.GetSize()));
}

void update() {
	if (!config::metrics_dump.to_json) {
		return;
	}

	auto dump_start_time = chrono::clock::now();
	auto new_dump = create_dump<rapidjson::MemoryPoolAllocator<>>();
	auto dump_end_time = chrono::clock::now();

	stats::dump_time.set(
			chrono::duration_cast<chrono::time_duration>(dump_end_time - dump_start_time).count(),
			dump_end_time
		);

	{
		std::lock_guard<std::mutex> lock(dump_mutex);
		dump = new_dump;
	}
}

void initialize() {
	stats::initialize();

	{
		std::lock_guard<std::mutex> lock(dump_mutex);

		dump = std::shared_ptr<const std::string>(new std::string());
	}
}

void finalize() {
	stats::finalize();

	{
		std::lock_guard<std::mutex> lock(dump_mutex);

		dump = std::shared_ptr<const std::string>(new std::string());
	}
}

}} // namespace handystats::json_dump


const std::shared_ptr<const std::string> HANDY_JSON_DUMP() {
	return handystats::json_dump::get_dump();
}

