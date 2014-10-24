// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <stdexcept>

#include <handystats/utils/rapidjson_writer.hpp>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <handystats/detail/serialization/json.hpp>
#include <handystats/detail/serialization/rapidjson.hpp>

using namespace rapidjson;

namespace handystats { namespace serialization { namespace json {

template <>
statistics::data loads<statistics::data>(const std::string& dump) {
	Document dump_value;
	dump_value.Parse<0>(dump.c_str());
	if (dump_value.HasParseError()) {
		throw std::invalid_argument(dump_value.GetParseError());
	}

	statistics::data data;
	rapidjson::load_value(data, dump_value);

	return data;
}

template <>
std::string dumps<statistics::data>(const statistics::data& data) {
	Value json_dump;
	MemoryPoolAllocator<> allocator;

	rapidjson::fill_value(json_dump, data, allocator);

	return utils::rapidjson::to_string <
				Value, StringBuffer,
				Writer<StringBuffer>
			>
			(json_dump);
}

}}} // namespace handystats::serialization::json
