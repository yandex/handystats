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

#include <thread>
#include <chrono>
#include <string>

#include <gtest/gtest.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include <handystats/core.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/metrics_dump.hpp>
#include <handystats/json_dump.hpp>

#include "message_queue_helper.hpp"
#include "metrics_dump_helper.hpp"

namespace handystats { namespace json {

template<typename Allocator>
void fill(
		rapidjson::Value& dump, Allocator& allocator,
		const std::map<std::string, handystats::metrics::metric_variant>& metrics_map
	);

}} // namespace handystats::json

static void check_full_json_dump(const std::string& string_dump) {
	rapidjson::Document dump;
	dump.Parse<0>(string_dump.c_str());

	for (rapidjson::Value::MemberIterator iter = dump.MemberBegin(); iter != dump.MemberEnd(); ++iter) {
		ASSERT_TRUE(iter->value.IsObject());
		ASSERT_TRUE(iter->value.HasMember("type"));

		std::string type(iter->value.FindMember("type")->value.GetString(), iter->value.FindMember("type")->value.GetStringLength());
		std::string name(iter->name.GetString());

		// skip internal metrics
		if (name.find("handystats") == 0) {
			continue;
		}

		if (type == "gauge") {
//			ASSERT_TRUE(iter->value.HasMember("timestamp"));
//			ASSERT_TRUE(iter->value.HasMember("value"));

//			ASSERT_TRUE(iter->value.HasMember("values"));
		}
		else if (type == "counter") {
//			ASSERT_TRUE(iter->value.HasMember("timestamp"));
//			ASSERT_TRUE(iter->value.HasMember("value"));

//			ASSERT_TRUE(iter->value.HasMember("values"));
		}
		else if (type == "timer") {
//			ASSERT_TRUE(iter->value.HasMember("timestamp"));
//			ASSERT_TRUE(iter->value.HasMember("value"));

//			ASSERT_TRUE(iter->value.HasMember("values"));
		}
		else if (type == "attribute") {
//			ASSERT_TRUE(iter->value.HasMember("timestamp"));
			ASSERT_TRUE(iter->value.HasMember("value"));
		}
	}
}

TEST(JsonDumpTest, TestJsonDumpMethods) {
	HANDY_CONFIG_JSON(
			"{\
				\"dump-interval\": 1\
			}"
		);

	HANDY_INIT();

	for (int i = 0; i < 10; ++i) {
		HANDY_TIMER_START("test.timer");
		HANDY_GAUGE_SET("test.gauge", i);
		HANDY_COUNTER_INCREMENT("test.counter", i);
		HANDY_ATTRIBUTE_SET("cycle.interation", i);
		HANDY_TIMER_STOP("test.timer");
	}

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	rapidjson::Document dump;
	handystats::json::fill(dump, dump.GetAllocator(), *metrics_dump);

	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, rapidjson::Document::AllocatorType> buffer(&dump.GetAllocator());
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<>, rapidjson::Document::AllocatorType>> writer(buffer);
	dump.Accept(writer);

	std::string string_dump(buffer.GetString(), buffer.GetSize());

	std::string to_string_dump = handystats::json::to_string(*metrics_dump);

	ASSERT_EQ(to_string_dump, string_dump);

	check_full_json_dump(to_string_dump);

	check_full_json_dump(HANDY_JSON_DUMP());

	HANDY_FINALIZE();
}

TEST(JsonDumpTest, CheckEmptyStatisticsNotShown) {
	HANDY_CONFIG_JSON(
			"{\
				\"gauge\": {\
					\"tags\": []\
				},\
				\"counter\": {\
				},\
				\"dump-interval\": 1\
			}"
		);

	HANDY_INIT();

	for (int i = 0; i < 10; ++i) {
		HANDY_TIMER_START("test.timer");
		HANDY_GAUGE_SET("test.gauge", i);
		HANDY_COUNTER_INCREMENT("test.counter", i);
		HANDY_ATTRIBUTE_SET("cycle.interation", i);
		HANDY_TIMER_STOP("test.timer");
	}

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	rapidjson::Document dump;
	handystats::json::fill(dump, dump.GetAllocator(), *metrics_dump);

//	ASSERT_FALSE(dump["test.gauge"].HasMember("values"));

//	ASSERT_TRUE(dump["test.counter"].HasMember("values"));

//	ASSERT_TRUE(dump["test.timer"].HasMember("values"));

	HANDY_FINALIZE();
}
