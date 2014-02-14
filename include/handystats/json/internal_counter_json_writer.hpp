#ifndef HANDYSTATS_INTERNAL_COUNTER_JSON_WRITER_H_
#define HANDYSTATS_INTERNAL_COUNTER_JSON_WRITER_H_

#include <string>

#include <handystats/rapidjson/document.h>
#include <handystats/rapidjson/stringbuffer.h>
#include <handystats/rapidjson/prettywriter.h>

#include <handystats/json/counter_json_writer.hpp>

#include <handystats/internal/internal_counter.hpp>

namespace handystats { namespace json {

template<typename Allocator>
inline void write_to_json_value(internal::internal_counter* obj, rapidjson::Value* json_value, Allocator& allocator) {
	if (!obj) {
		json_value = new rapidjson::Value();
		return;
	}

	write_to_json_value(obj->base_counter, json_value, allocator);
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(internal::internal_counter* obj, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(obj, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

inline std::string write_to_json_string(internal::internal_counter* obj) {
	rapidjson::StringBuffer buffer(&crtAllocator);
	write_to_json_buffer(obj, &buffer, memoryPoolAllocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

#endif // HANDYSTATS_INTERNAL_COUNTER_JSON_WRITER_H_
