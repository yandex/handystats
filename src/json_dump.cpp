#include <handystats/json_dump.hpp>
#include <handystats/json_dump_impl.hpp>

#include <handystats/chrono_impl.hpp>
#include <handystats/json_impl.hpp>
#include <handystats/internal_metrics_impl.hpp>
#include <handystats/internal_statistics_impl.hpp>

namespace handystats { namespace json {

chrono::default_time_point dump_timestamp;
chrono::default_duration DUMP_INTERVAL = std::chrono::duration_cast<chrono::default_duration>(std::chrono::milliseconds(500));

std::mutex json_dump_mutex;
std::shared_ptr<const std::string> json_dump(new std::string());

std::shared_ptr<const std::string> get_json_dump() {
	std::lock_guard<std::mutex> lock(json_dump_mutex);
	return json_dump;
}

std::shared_ptr<const std::string> create_json_dump() {
	rapidjson::Value dump_value(rapidjson::kObjectType);

	dump_timestamp = chrono::default_clock::now();
	rapidjson::Value timestamp_value;
	write_to_json_value(dump_timestamp, &timestamp_value);
	dump_value.AddMember("dump-timestamp", timestamp_value, memoryPoolAllocator);

	for (auto metric_entry : internal::internal_metrics) {
		rapidjson::Value metric_value;
		switch (metric_entry.second.which()) {
			case internal::internal_metric_index::INTERNAL_GAUGE:
				write_to_json_value(boost::get<internal::internal_gauge*>(metric_entry.second), &metric_value, memoryPoolAllocator);
				break;
			case internal::internal_metric_index::INTERNAL_COUNTER:
				write_to_json_value(boost::get<internal::internal_counter*>(metric_entry.second), &metric_value, memoryPoolAllocator);
				break;
			case internal::internal_metric_index::INTERNAL_TIMER:
				write_to_json_value(boost::get<internal::internal_timer*>(metric_entry.second), &metric_value, memoryPoolAllocator);
				break;
		}

		dump_value.AddMember(metric_entry.first.c_str(), memoryPoolAllocator, metric_value, memoryPoolAllocator);
	}

	{
		rapidjson::Value queue_size_value;
		write_to_json_value(&internal::event_message_queue_size, &queue_size_value, memoryPoolAllocator);
		dump_value.AddMember("__message-queue-size", queue_size_value, memoryPoolAllocator);
	}
	{
		rapidjson::Value internal_metrics_size_value;
		write_to_json_value(&internal::internal_metrics_size, &internal_metrics_size_value, memoryPoolAllocator);
		dump_value.AddMember("__internal-metrics-size", internal_metrics_size_value, memoryPoolAllocator);
	}
	{
		rapidjson::Value processing_time_value;
		write_to_json_value(&internal::message_processing_time, &processing_time_value, memoryPoolAllocator);
		dump_value.AddMember("__message-processing-time", processing_time_value, memoryPoolAllocator);
	}
	{
		rapidjson::Value push_time_value;
		write_to_json_value(&internal::message_push_time, &push_time_value, memoryPoolAllocator);
		dump_value.AddMember("__message-push-time", push_time_value, memoryPoolAllocator);
	}
	{
		rapidjson::Value pop_time_value;
		write_to_json_value(&internal::message_pop_time, &pop_time_value, memoryPoolAllocator);
		dump_value.AddMember("__message-pop-time", pop_time_value, memoryPoolAllocator);
	}

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	dump_value.Accept(writer);

	std::shared_ptr<const std::string> new_json_dump(new std::string(buffer.GetString(), buffer.GetSize()));
	return new_json_dump;
}

void update_json_dump() {
	if (std::chrono::duration_cast<chrono::default_duration>(chrono::default_clock::now() - dump_timestamp) > DUMP_INTERVAL) {
		auto new_json_dump = create_json_dump();
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

