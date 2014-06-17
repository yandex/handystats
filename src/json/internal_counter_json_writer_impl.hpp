// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_INTERNAL_COUNTER_JSON_WRITER_H_
#define HANDYSTATS_INTERNAL_COUNTER_JSON_WRITER_H_

#include <string>

#include <handystats/rapidjson/document.h>
#include <handystats/rapidjson/stringbuffer.h>
#include <handystats/rapidjson/prettywriter.h>

#include <handystats/json/counter_json_writer.hpp>

#include "internal_metrics/internal_counter_impl.hpp"


namespace handystats { namespace json {

template<typename Allocator>
inline void write_to_json_value(const internal::internal_counter* const obj, rapidjson::Value* json_value, Allocator& allocator) {
	if (!obj) {
		json_value = new rapidjson::Value();
		return;
	}

	write_to_json_value(obj->base_counter, json_value, allocator);
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(const internal::internal_counter* const obj, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(obj, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

template<typename Allocator>
inline std::string write_to_json_string(const internal::internal_counter* const obj, Allocator&& allocator = Allocator()) {
	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	write_to_json_buffer(obj, &buffer, allocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

#endif // HANDYSTATS_INTERNAL_COUNTER_JSON_WRITER_H_
