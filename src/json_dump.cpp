// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/json_dump.hpp>

namespace handystats { namespace json {

std::string to_string(const metrics_dump_type& metrics_dump) {
	typedef rapidjson::MemoryPoolAllocator<> allocator_type;

	rapidjson::Value dump;
	allocator_type allocator;
	fill(dump, allocator, metrics_dump);

	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, allocator_type> buffer(&allocator);
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<>, allocator_type>> writer(buffer);
	dump.Accept(writer);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

std::string HANDY_JSON_DUMP() {
	return handystats::json::to_string(*HANDY_METRICS_DUMP());
}

