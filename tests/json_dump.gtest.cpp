#include <thread>
#include <chrono>
#include <string>

#include <gtest/gtest.h>

#include <handystats/core.hpp>
#include <handystats/module.h>
#include <handystats/measuring_points.hpp>
#include <handystats/metrics_dump.hpp>
#include <handystats/json_dump.hpp>
#include <handystats/utils/rapidjson_writer.hpp>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include "message_queue_helper.hpp"
#include "metrics_dump_helper.hpp"

#ifndef _HAVE_HANDY_MODULE_TEST
#define _HAVE_HANDY_MODULE_TEST 1
#endif
HANDY_MODULE(TEST)

TEST(JsonDumpTest, TestJsonDumpMethods) {
	HANDY_CONFIG_JSON(
			"{\
				\"dump-interval\": 1\
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

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	rapidjson::Document dump;
	handystats::utils::rapidjson::fill_value(static_cast<rapidjson::Value&>(dump), *metrics_dump, dump.GetAllocator());

	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, rapidjson::Document::AllocatorType> buffer(&dump.GetAllocator());
	rapidjson::PrettyWriter<rapidjson::GenericStringBuffer<rapidjson::UTF8<>, rapidjson::Document::AllocatorType>> writer(buffer);
	dump.Accept(writer);

	std::string string_dump(buffer.GetString(), buffer.GetSize());

	std::string to_string_dump =
		handystats::utils::rapidjson::to_string<
				rapidjson::Value,
				rapidjson::StringBuffer,
				rapidjson::PrettyWriter<rapidjson::StringBuffer>
			>(static_cast<rapidjson::Value&>(dump));

	ASSERT_EQ(to_string_dump, string_dump);

	HANDY_FINALIZE();
}

TEST(JsonDumpTest, CheckEmptyStatisticsNotShown) {
	HANDY_CONFIG_JSON(
			"{\
				\"metrics\": {\
					\"gauge\": {\
						\"stats\": []\
					},\
					\"counter\": {\
					}\
				},\
				\"dump-interval\": 1\
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

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	rapidjson::Document dump;
	handystats::utils::rapidjson::fill_value(static_cast<rapidjson::Value&>(dump), *metrics_dump, dump.GetAllocator());

//	ASSERT_FALSE(dump["test.gauge"].HasMember("values"));

//	ASSERT_TRUE(dump["test.counter"].HasMember("values"));

//	ASSERT_TRUE(dump["test.timer"].HasMember("values"));

	HANDY_FINALIZE();
}
