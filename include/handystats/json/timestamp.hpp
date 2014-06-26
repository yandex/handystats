// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_JSON_TIMESTAMP_H_
#define HANDYSTATS_JSON_TIMESTAMP_H_

#include <handystats/chrono.hpp>
#include <handystats/rapidjson/document.h>
#include <handystats/rapidjson/stringbuffer.h>
#include <handystats/rapidjson/prettywriter.h>

namespace handystats { namespace json {

typedef std::chrono::milliseconds time_duration;

inline void write_to_json_value(const chrono::clock::time_point& timestamp, rapidjson::Value* json_value) {
	if (!json_value) {
		json_value = new rapidjson::Value();
	}

	std::chrono::system_clock::time_point system_timestamp = chrono::to_system_time(timestamp);

	json_value->SetUint64(chrono::duration_cast<time_duration>(system_timestamp.time_since_epoch()).count());
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(const chrono::clock::time_point& timestamp, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(timestamp, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

template<typename Allocator>
inline std::string write_to_json_string(const chrono::clock::time_point& timestamp, Allocator&& allocator = Allocator()) {
	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	write_to_json_buffer(timestamp, &buffer, allocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}
}} // namespace handystats::json

#endif // HANDYSTATS_JSON_TIMESTAMP_H_
