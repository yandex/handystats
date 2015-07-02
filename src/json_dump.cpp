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

#include <handystats/json_dump.hpp>

namespace handystats { namespace json {

std::string to_string(const std::map<std::string, handystats::metrics::metric_variant>& metrics_map) {
	typedef rapidjson::MemoryPoolAllocator<> allocator_type;

	rapidjson::Value dump;
	allocator_type allocator;
	fill(dump, allocator, metrics_map);

	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, allocator_type> buffer(&allocator);
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<>, allocator_type>> writer(buffer);
	dump.Accept(writer);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

std::string HANDY_JSON_DUMP() {
	return handystats::json::to_string(*HANDY_METRICS_DUMP());
}

