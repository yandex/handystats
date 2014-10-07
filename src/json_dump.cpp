// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics_dump.hpp>
#include <handystats/json_dump.hpp>

#include <handystats/utils/rapidjson_writer.hpp>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

using namespace rapidjson;

std::string HANDY_JSON_DUMP() {
	Value json_dump;
	MemoryPoolAllocator<> allocator;

	handystats::utils::rapidjson::fill_value(json_dump, *HANDY_METRICS_DUMP(), allocator);

	return handystats::utils::rapidjson::to_string<Value, StringBuffer, PrettyWriter<StringBuffer>>(json_dump);
}

