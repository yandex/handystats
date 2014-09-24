// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_COUNTER_JSON_WRITER_HPP_
#define HANDYSTATS_COUNTER_JSON_WRITER_HPP_

#include <string>

#include <handystats/rapidjson/document.h>
#include <handystats/rapidjson/stringbuffer.h>
#include <handystats/rapidjson/prettywriter.h>

#include <handystats/json/gauge_json_writer.hpp>
#include <handystats/metrics/counter.hpp>


namespace handystats { namespace json {

template<typename Allocator>
inline void write_to_json_value(const metrics::counter* const obj, rapidjson::Value* json_value, Allocator& allocator) {
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

	json_value->AddMember("type", "counter", allocator);

	rapidjson::Value values;
	write_to_json_value(&obj->values(), &values, allocator);
	if (!values.IsNull()) {
		json_value->AddMember("values", values, allocator);
	}
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(const metrics::counter* const obj, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(obj, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

template<typename Allocator>
inline std::string write_to_json_string(const metrics::counter* const obj, Allocator&& allocator = Allocator()) {
	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	write_to_json_buffer(obj, &buffer, allocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

#endif // HANDYSTATS_COUNTER_JSON_WRITER_HPP_
