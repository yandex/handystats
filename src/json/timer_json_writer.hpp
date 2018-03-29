/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#ifndef HANDYSTATS_TIMER_JSON_WRITER_HPP_
#define HANDYSTATS_TIMER_JSON_WRITER_HPP_

#include <string>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include <handystats/metrics/timer.hpp>

#include "json/statistics_json_writer.hpp"

namespace handystats { namespace json {

template<typename Allocator>
inline void write_to_json_value(const metrics::timer* const obj, rapidjson::Value* json_value, Allocator& allocator) {
	if (!obj) {
		if (!json_value) {
			json_value = new rapidjson::Value();
		} else {
			json_value->SetNull();
		}
		return;
	}

	if (!json_value) {
		json_value = new rapidjson::Value(rapidjson::kObjectType);
	}
	else {
		json_value->SetObject();
	}

	json_value->AddMember("type", "timer", allocator);

	write_to_json_value(&obj->values(), json_value, allocator);
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(const metrics::timer* const obj, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(obj, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

template<typename Allocator>
inline std::string write_to_json_string(const metrics::timer* const obj, Allocator&& allocator = Allocator()) {
	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	write_to_json_buffer(obj, &buffer, allocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

#endif // HANDYSTATS_TIMER_JSON_WRITER_HPP_
