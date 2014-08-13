// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_ATTRIBUTE_JSON_WRITER_HPP_
#define HANDYSTATS_ATTRIBUTE_JSON_WRITER_HPP_

#include <cstdint>

#include <string>

#include <handystats/rapidjson/document.h>
#include <handystats/rapidjson/stringbuffer.h>
#include <handystats/rapidjson/prettywriter.h>

#include <handystats/json/timestamp.hpp>
#include <handystats/metrics/attribute.hpp>

namespace handystats { namespace json {

template<typename Allocator>
inline void write_to_json_value(const metrics::attribute* const obj, rapidjson::Value* json_value, Allocator& allocator) {
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

	json_value->AddMember("type", "attribute", allocator);

	switch (obj->value.which()) {
		case metrics::attribute::value_index::BOOL:
			json_value->AddMember("value", boost::get<bool>(obj->value), allocator);
			break;
		case metrics::attribute::value_index::INT:
			json_value->AddMember("value", boost::get<int>(obj->value), allocator);
			break;
		case metrics::attribute::value_index::UINT:
			json_value->AddMember("value", boost::get<unsigned>(obj->value), allocator);
			break;
		case metrics::attribute::value_index::INT64:
			json_value->AddMember("value", boost::get<int64_t>(obj->value), allocator);
			break;
		case metrics::attribute::value_index::UINT64:
			json_value->AddMember("value", boost::get<uint64_t>(obj->value), allocator);
			break;
		case metrics::attribute::value_index::DOUBLE:
			json_value->AddMember("value", boost::get<double>(obj->value), allocator);
			break;
		case metrics::attribute::value_index::STRING:
			json_value->AddMember("value", boost::get<std::string>(obj->value).c_str(), allocator);
			break;
	}

	rapidjson::Value timestamp;
	write_to_json_value(obj->timestamp, &timestamp);
	json_value->AddMember("timestamp", timestamp, allocator);
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(const metrics::attribute* const obj, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(obj, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

template<typename Allocator>
inline std::string write_to_json_string(const metrics::attribute* const obj, Allocator&& allocator = Allocator()) {
	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	write_to_json_buffer(obj, &buffer, allocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

#endif // HANDYSTATS_ATTRIBUTE_JSON_WRITER_HPP_
