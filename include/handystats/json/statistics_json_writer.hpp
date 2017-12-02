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

#ifndef HANDYSTATS_INCREMENTAL_STATISTICS_JSON_WRITER_HPP_
#define HANDYSTATS_INCREMENTAL_STATISTICS_JSON_WRITER_HPP_

#include <string>
#include <algorithm>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include <handystats/json/timestamp.hpp>
#include <handystats/statistics.hpp>

namespace handystats { namespace json {

template <typename Allocator>
inline void write_to_json_value(const statistics* const obj, rapidjson::Value* json_value, Allocator& allocator) {
	if (!obj) {
		return;
	}

	if (!json_value) {
		json_value = new rapidjson::Value(rapidjson::kObjectType);
	}
	else if (!json_value->IsObject()) {
		json_value->SetObject();
	}

	if (obj->tags() == statistics::tag::empty) {
		json_value->SetNull();
		return;
	}

	if (obj->enabled(statistics::tag::value)) {
		json_value->AddMember("value", obj->get<statistics::tag::value>(), allocator);
	}
	if (obj->enabled(statistics::tag::min)) {
		json_value->AddMember("min", obj->get<statistics::tag::min>(), allocator);
	}
	if (obj->enabled(statistics::tag::max)) {
		json_value->AddMember("max", obj->get<statistics::tag::max>(), allocator);
	}
	if (obj->enabled(statistics::tag::count)) {
		json_value->AddMember("count", obj->get<statistics::tag::count>(), allocator);
	}
	if (obj->enabled(statistics::tag::sum)) {
		json_value->AddMember("sum", obj->get<statistics::tag::sum>(), allocator);
	}
	if (obj->enabled(statistics::tag::avg)) {
		json_value->AddMember("avg", obj->get<statistics::tag::avg>(), allocator);
	}
	if (obj->enabled(statistics::tag::moving_count)) {
		json_value->AddMember("moving-count", obj->get<statistics::tag::moving_count>(), allocator);
	}
	if (obj->enabled(statistics::tag::moving_sum)) {
		json_value->AddMember("moving-sum", obj->get<statistics::tag::moving_sum>(), allocator);
	}
	if (obj->enabled(statistics::tag::moving_avg)) {
		json_value->AddMember("moving-avg", obj->get<statistics::tag::moving_avg>(), allocator);
	}
	if (obj->enabled(statistics::tag::histogram)) {
		auto histogram = obj->get<statistics::tag::histogram>();
		rapidjson::Value histogram_value(rapidjson::kArrayType);
		for (auto bin = histogram.begin(); bin != histogram.end(); ++bin) {
			histogram_value.PushBack(
					rapidjson::Value().SetArray()
					.PushBack(std::get<statistics::BIN_CENTER>(*bin), allocator)
					.PushBack(std::get<statistics::BIN_COUNT>(*bin), allocator),
					allocator
				);
		}
		json_value->AddMember("histogram", histogram_value, allocator);
	}
	if (obj->enabled(statistics::tag::quantile)) {
		auto quantile = obj->get<statistics::tag::quantile>();
		json_value->AddMember("p25", quantile.at(0.25), allocator);
		json_value->AddMember("p50", quantile.at(0.50), allocator);
		json_value->AddMember("p75", quantile.at(0.75), allocator);
		json_value->AddMember("p90", quantile.at(0.90), allocator);
		json_value->AddMember("p95", quantile.at(0.95), allocator);
	}
	if (obj->enabled(statistics::tag::timestamp)) {
		rapidjson::Value timestamp_value;
		write_to_json_value(obj->get<statistics::tag::timestamp>(), &timestamp_value);
		json_value->AddMember("timestamp", timestamp_value, allocator);
	}
	if (obj->enabled(statistics::tag::rate)) {
		json_value->AddMember("rate", obj->get<statistics::tag::rate>(), allocator);
	}
	if (obj->enabled(statistics::tag::entropy)) {
		json_value->AddMember("entropy", obj->get<statistics::tag::entropy>(), allocator);
	}
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(const statistics* const obj, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(obj, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

template<typename Allocator>
inline std::string write_to_json_string(const statistics* const obj, Allocator&& allocator = Allocator()) {
	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	write_to_json_buffer(obj, &buffer, allocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

#endif // HANDYSTATS_INCREMENTAL_STATISTICS_JSON_WRITER_HPP_
