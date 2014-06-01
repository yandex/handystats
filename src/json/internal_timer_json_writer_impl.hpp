#ifndef HANDYSTATS_INTERNAL_TIMER_JSON_WRITER_H_
#define HANDYSTATS_INTERNAL_TIMER_JSON_WRITER_H_

#include <string>

#include <handystats/rapidjson/document.h>
#include <handystats/rapidjson/stringbuffer.h>
#include <handystats/rapidjson/prettywriter.h>

#include <handystats/json/incremental_statistics_json_writer.hpp>

#include "internal_metrics/internal_timer_impl.hpp"


namespace handystats { namespace json {

template<typename Allocator>
inline void write_to_json_value(internal::internal_timer* obj, rapidjson::Value* json_value, Allocator& allocator) {
	if (!obj) {
		json_value = new rapidjson::Value();
		return;
	}

	if (!json_value) {
		json_value = new rapidjson::Value(rapidjson::kObjectType);
	}
	else {
		json_value->SetObject();
	}

	json_value->AddMember("type", "internal-timer", allocator);

	rapidjson::Value timestamp;
	write_to_json_value(obj->timestamp, &timestamp);
	json_value->AddMember("timestamp", timestamp, allocator);

	json_value->AddMember("active-instances", obj->instances.size(), allocator);

	rapidjson::Value values;
	write_to_json_value(&obj->values, &values, allocator);
	json_value->AddMember("values", values, allocator);
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(internal::internal_timer* obj, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(obj, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

template<typename Allocator>
inline std::string write_to_json_string(internal::internal_timer* obj, Allocator&& allocator = Allocator()) {
	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	write_to_json_buffer(obj, &buffer, allocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

#endif // HANDYSTATS_INTERNAL_TIMER_JSON_WRITER_H_
