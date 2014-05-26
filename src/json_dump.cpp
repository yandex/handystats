#include <mutex>
#include <memory>
#include <string>
#include <map>

#include <handystats/chrono.hpp>
#include <handystats/json_dump.hpp>

#include "internal_metrics_impl.hpp"

#include <handystats/rapidjson/document.h>

#include <handystats/json/timestamp.hpp>

#include "json/internal_gauge_json_writer_impl.hpp"
#include "json/internal_counter_json_writer_impl.hpp"
#include "json/internal_timer_json_writer_impl.hpp"

#include "system_stats_impl.hpp"

namespace handystats { namespace internal {

extern std::map<std::string, internal_metric> internal_metrics;

}} // namespace handystats::internal


namespace handystats { namespace json {

chrono::default_time_point dump_timestamp;
chrono::default_duration DUMP_INTERVAL = std::chrono::duration_cast<chrono::default_duration>(std::chrono::milliseconds(500));

std::mutex json_dump_mutex;
std::shared_ptr<const std::string> json_dump(new std::string());

std::shared_ptr<const std::string> get_json_dump() {
	std::lock_guard<std::mutex> lock(json_dump_mutex);
	return json_dump;
}

template<typename Allocator>
std::shared_ptr<const std::string> create_json_dump(Allocator&& allocator = Allocator()) {
	rapidjson::Value dump_value(rapidjson::kObjectType);

	for (auto metric_entry : internal::internal_metrics) {
		rapidjson::Value metric_value;
		switch (metric_entry.second.which()) {
			case internal::internal_metric_index::INTERNAL_GAUGE:
				write_to_json_value(boost::get<internal::internal_gauge*>(metric_entry.second), &metric_value, allocator);
				break;
			case internal::internal_metric_index::INTERNAL_COUNTER:
				write_to_json_value(boost::get<internal::internal_counter*>(metric_entry.second), &metric_value, allocator);
				break;
			case internal::internal_metric_index::INTERNAL_TIMER:
				write_to_json_value(boost::get<internal::internal_timer*>(metric_entry.second), &metric_value, allocator);
				break;
		}

		dump_value.AddMember(metric_entry.first.c_str(), allocator, metric_value, allocator);
	}

	{
		rapidjson::Value queue_size_value;
		write_to_json_value(&message_queue_size, &queue_size_value, allocator);
		dump_value.AddMember("__message-queue-size", queue_size_value, allocator);
	}
	{
		rapidjson::Value internal_metrics_size_value;
		write_to_json_value(&internal_metrics_size, &internal_metrics_size_value, allocator);
		dump_value.AddMember("__internal-metrics-size", internal_metrics_size_value, allocator);
	}
	{
		rapidjson::Value process_time_value;
		write_to_json_value(&message_process_time, &process_time_value, allocator);
		dump_value.AddMember("__message-process-time", process_time_value, allocator);
	}
	{
		rapidjson::Value pop_time_value;
		write_to_json_value(&message_pop_time, &pop_time_value, allocator);
		dump_value.AddMember("__message-pop-time", pop_time_value, allocator);
	}
	{
		dump_timestamp = chrono::default_clock::now();
		rapidjson::Value timestamp_value;
		write_to_json_value(dump_timestamp, &timestamp_value);
		dump_value.AddMember("__dump-timestamp", timestamp_value, allocator);
	}

	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator>> writer(buffer);
	dump_value.Accept(writer);

	return std::shared_ptr<const std::string>(new std::string(buffer.GetString(), buffer.GetSize()));
}

void update_json_dump() {
	if (std::chrono::duration_cast<chrono::default_duration>(chrono::default_clock::now() - dump_timestamp) > DUMP_INTERVAL) {
		auto new_json_dump = create_json_dump<rapidjson::MemoryPoolAllocator<>>();
		{
			std::lock_guard<std::mutex> lock(json_dump_mutex);
			json_dump = new_json_dump;
		}
	}
}

}} // namespace handystats::json


std::shared_ptr<const std::string> HANDY_JSON_DUMP() {
	return handystats::json::get_json_dump();
}

