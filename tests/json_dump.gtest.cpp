#include <thread>
#include <chrono>
#include <string>

#include <gtest/gtest.h>

#include <handystats/core.hpp>
#include <handystats/module.h>
#include <handystats/measuring_points.hpp>
#include <handystats/metrics_dump.hpp>
#include <handystats/json_dump.hpp>

#include "message_queue_helper.hpp"

#ifndef _HAVE_HANDY_MODULE_TEST
#define _HAVE_HANDY_MODULE_TEST 1
#endif
HANDY_MODULE(TEST)

static void check_full_json_dump(const std::string& string_dump) {
	rapidjson::Document dump;
	dump.Parse<0>(string_dump.c_str());

	for (rapidjson::Value::MemberIterator iter = dump.MemberBegin(); iter != dump.MemberEnd(); ++iter) {
		ASSERT_TRUE(iter->value.IsObject());
		ASSERT_TRUE(iter->value.FindMember("type") != NULL);

		std::string type(iter->value.FindMember("type")->value.GetString(), iter->value.FindMember("type")->value.GetStringLength());
		std::string name(iter->name.GetString());

		// skip internal metrics
		if (name.find("handystats") == 0) {
			continue;
		}

		if (type == "gauge") {
//			ASSERT_TRUE(iter->value.FindMember("timestamp") != NULL);
//			ASSERT_TRUE(iter->value.FindMember("value") != NULL);

			ASSERT_TRUE(iter->value.FindMember("values") != NULL);
		}
		else if (type == "counter") {
//			ASSERT_TRUE(iter->value.FindMember("timestamp") != NULL);
//			ASSERT_TRUE(iter->value.FindMember("value") != NULL);

			ASSERT_TRUE(iter->value.FindMember("values") != NULL);
		}
		else if (type == "timer") {
//			ASSERT_TRUE(iter->value.FindMember("timestamp") != NULL);
//			ASSERT_TRUE(iter->value.FindMember("value") != NULL);

			ASSERT_TRUE(iter->value.FindMember("values") != NULL);
		}
		else if (type == "attribute") {
//			ASSERT_TRUE(iter->value.FindMember("timestamp") != NULL);
			ASSERT_TRUE(iter->value.FindMember("value") != NULL);
		}
	}
}

TEST(JsonDumpTest, TestJsonDumpMethods) {
	HANDY_CONFIG_JSON(
			"{\
				\"metrics-dump\": {\
					\"interval\": 1\
				}\
			}"
		);

	HANDY_INIT();

	for (int i = 0; i < 10; ++i) {
		TEST_TIMER_START("test.timer");
		TEST_GAUGE_SET("test.gauge", i);
		TEST_COUNTER_INCREMENT("test.counter", i);
		TEST_ATTRIBUTE_SET("cycle.interation", i);
		TEST_TIMER_STOP("test.timer");
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

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
				\"metrics\": {\
					\"gauge\": {\
						\"values\": {\
							\"tags\": []\
						}\
					},\
					\"counter\": {\
					}\
				},\
				\"metrics-dump\": {\
					\"interval\": 1\
				}\
			}"
		);

	HANDY_INIT();

	for (int i = 0; i < 10; ++i) {
		TEST_TIMER_START("test.timer");
		TEST_GAUGE_SET("test.gauge", i);
		TEST_COUNTER_INCREMENT("test.counter", i);
		TEST_ATTRIBUTE_SET("cycle.interation", i);
		TEST_TIMER_STOP("test.timer");
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	auto metrics_dump = HANDY_METRICS_DUMP();

	rapidjson::Document dump;
	handystats::json::fill(dump, dump.GetAllocator(), *metrics_dump);

	ASSERT_FALSE(dump["test.gauge"].HasMember("values"));

	ASSERT_TRUE(dump["test.counter"].HasMember("values"));

	ASSERT_TRUE(dump["test.timer"].HasMember("values"));

	HANDY_FINALIZE();
}
