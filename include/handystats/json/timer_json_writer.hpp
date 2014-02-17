#ifndef HANDYSTATS_TIMER_JSON_WRITER_H_
#define HANDYSTATS_TIMER_JSON_WRITER_H_

#include <string>

#include <handystats/rapidjson/document.h>
#include <handystats/rapidjson/stringbuffer.h>
#include <handystats/rapidjson/prettywriter.h>

#include <handystats/json/allocators.hpp>
#include <handystats/json/timestamp.hpp>
#include <handystats/metrics/timer.hpp>

namespace handystats { namespace json {

template<typename Allocator>
inline void write_to_json_value(metrics::timer* obj, rapidjson::Value* json_value, Allocator& allocator) {
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

	json_value->AddMember("type", "timer", allocator);
	json_value->AddMember("value", obj->get().first.count(), allocator);

	rapidjson::Value timestamp;
	write_to_json_value(obj->get().second, &timestamp);
	json_value->AddMember("timestamp", timestamp, allocator);
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(metrics::timer* obj, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(obj, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

inline std::string write_to_json_string(metrics::timer* obj) {
	rapidjson::StringBuffer buffer;
	write_to_json_buffer(obj, &buffer, memoryPoolAllocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

#endif // HANDYSTATS_TIMER_JSON_WRITER_H_
