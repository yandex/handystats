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

#ifndef HANDYSTATS_JSON_TIMESTAMP_HPP_
#define HANDYSTATS_JSON_TIMESTAMP_HPP_

#include <handystats/chrono.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

namespace handystats { namespace json {

inline void write_to_json_value(const chrono::time_point& timestamp, rapidjson::Value* json_value) {
	if (!json_value) {
		json_value = new rapidjson::Value();
	}

	chrono::time_point system_timestamp = chrono::time_point::convert_to(chrono::clock_type::SYSTEM, timestamp);

	json_value->SetUint64(chrono::duration::convert_to(chrono::time_unit::MSEC, system_timestamp.time_since_epoch()).count());
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(const chrono::time_point& timestamp, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(timestamp, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

template<typename Allocator>
inline std::string write_to_json_string(const chrono::time_point& timestamp, Allocator&& allocator = Allocator()) {
	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	write_to_json_buffer(timestamp, &buffer, allocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}
}} // namespace handystats::json

#endif // HANDYSTATS_JSON_TIMESTAMP_HPP_
