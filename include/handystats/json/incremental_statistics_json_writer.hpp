// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_INCREMENTAL_STATISTICS_JSON_WRITER_H_
#define HANDYSTATS_INCREMENTAL_STATISTICS_JSON_WRITER_H_

#include <string>

#include <handystats/rapidjson/document.h>
#include <handystats/rapidjson/stringbuffer.h>
#include <handystats/rapidjson/prettywriter.h>

#include <handystats/json/timestamp.hpp>
#include <handystats/incremental_statistics.hpp>

namespace handystats { namespace json {

template <typename Allocator>
inline void write_to_json_value(const incremental_statistics* const obj, rapidjson::Value* json_value, Allocator& allocator) {
	if (!obj) {
		return;
	}

	if (!json_value) {
		json_value = new rapidjson::Value(rapidjson::kObjectType);
	}
	else {
		json_value->SetObject();
	}

	json_value->AddMember("min", obj->min(), allocator);
	json_value->AddMember("max", obj->max(), allocator);
	json_value->AddMember("sum", obj->sum(), allocator);
	json_value->AddMember("count", obj->count(), allocator);
	json_value->AddMember("mean", obj->count() == 0 ? 0.0 : obj->mean(), allocator);
	json_value->AddMember("moving-avg", obj->moving_average(), allocator);
	json_value->AddMember("interval-count", obj->interval_count(), allocator);
	json_value->AddMember("interval-sum", obj->interval_sum(), allocator);
	json_value->AddMember("interval-mean", obj->interval_mean(), allocator);
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(const incremental_statistics* const obj, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(obj, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

template<typename Allocator>
inline std::string write_to_json_string(const incremental_statistics* const obj, Allocator&& allocator = Allocator()) {
	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	write_to_json_buffer(obj, &buffer, allocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

#endif // HANDYSTATS_INCREMENTAL_STATISTICS_JSON_WRITER_H_
