// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_SYSTEM_STATS_JSON_WRITER_IMPL_H_
#define HANDYSTATS_SYSTEM_STATS_JSON_WRITER_IMPL_H_

#include <string>

#include <handystats/rapidjson/document.h>
#include <handystats/json/gauge_json_writer.hpp>

#include "system_stats_impl.hpp"

namespace handystats { namespace json { namespace system_stats {

template<typename Allocator>
inline void write_to_json_value(rapidjson::Value* json_value, Allocator& allocator) {
	if (!json_value) {
		json_value = new rapidjson::Value(rapidjson::kObjectType);
	}
	else {
		json_value->SetObject();
	}

	{
		rapidjson::Value message_queue_size_value;
		json::write_to_json_value(&handystats::system_stats::message_queue_size, &message_queue_size_value, allocator);
		json_value->AddMember("message-queue-size", message_queue_size_value, allocator);
	}
	{
		rapidjson::Value internal_metrics_size_value;
		json::write_to_json_value(&handystats::system_stats::internal_metrics_size, &internal_metrics_size_value, allocator);
		json_value->AddMember("internal-metrics-size", internal_metrics_size_value, allocator);
	}
	{
		rapidjson::Value message_process_time_value;
		json::write_to_json_value(&handystats::system_stats::message_process_time, &message_process_time_value, allocator);
		json_value->AddMember("message-process-time", message_process_time_value, allocator);
	}
	{
		rapidjson::Value message_pop_time_value;
		json::write_to_json_value(&handystats::system_stats::message_pop_time, &message_pop_time_value, allocator);
		json_value->AddMember("message-pop-time", message_pop_time_value, allocator);
	}
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(&json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

template<typename Allocator>
inline std::string write_to_json_string(Allocator&& allocator = Allocator()) {
	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	write_to_json_buffer(&buffer, allocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}}} // namespace handystats::json::system_stats

#endif // HANDYSTATS_SYSTEM_STATS_JSON_WRITER_IMPL_H_
