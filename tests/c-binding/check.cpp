#include <thread>

#include <gtest/gtest.h>

#include <handystats/core.hpp>
#include <handystats/statistics.hpp>
#include <handystats/chrono.hpp>
#include <handystats/metrics_dump.hpp>

#include "../metrics_dump_helper.hpp"
#include "../message_queue_helper.hpp"

#include "test.h"

TEST(CBindingTest, TestGauge) {
	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find(TEST_GAUGE_NAME) != metrics_dump->first.end());

	const auto& test_gauge_values = metrics_dump->first.at(TEST_GAUGE_NAME);

	ASSERT_NEAR(test_gauge_values.get<handystats::statistics::tag::min>(), TEST_GAUGE_MIN, 1E-9);

	ASSERT_NEAR(test_gauge_values.get<handystats::statistics::tag::max>(), TEST_GAUGE_MAX, 1E-9);

	ASSERT_EQ(test_gauge_values.get<handystats::statistics::tag::count>(), TEST_GAUGE_MAX - TEST_GAUGE_MIN + 1);

	ASSERT_NEAR(test_gauge_values.get<handystats::statistics::tag::avg>(), (TEST_GAUGE_MIN + TEST_GAUGE_MAX) / 2.0, 1E-9);
}

TEST(CBindingTest, TestCounter) {
	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find(TEST_COUNTER_NAME) != metrics_dump->first.end());

	const auto& test_counter_values = metrics_dump->first.at(TEST_COUNTER_NAME);

	ASSERT_EQ(test_counter_values.get<handystats::statistics::tag::count>(), TEST_COUNTER_INCR_COUNT + TEST_COUNTER_DECR_COUNT + 1);
}

TEST(CBindingTest, TestScopedCounter) {
	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find(TEST_SCOPED_COUNTER_NAME) != metrics_dump->first.end());

	const auto& test_scoped_counter = metrics_dump->first.at(TEST_SCOPED_COUNTER_NAME);

	ASSERT_EQ(test_scoped_counter.get<handystats::statistics::tag::count>(), TEST_SCOPED_COUNTER_COUNT * 2 + 1);
	ASSERT_NEAR(test_scoped_counter.get<handystats::statistics::tag::value>(), 0, 1E-9);
}

TEST(CBindingTest, TestTimer) {
	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find(TEST_TIMER_NAME) != metrics_dump->first.end());

	const auto& test_timer_values = metrics_dump->first.at(TEST_TIMER_NAME);

	ASSERT_EQ(test_timer_values.get<handystats::statistics::tag::count>(), TEST_TIMER_NANOSLEEP_COUNT);
	ASSERT_GE(test_timer_values.get<handystats::statistics::tag::min>(), TEST_TIMER_NANOSLEEP_COUNT / 1000.0);
}

TEST(CBindingTest, TestScopedTimer) {
	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find(TEST_SCOPED_TIMER_NAME) != metrics_dump->first.end());

	const auto& test_scoped_timer = metrics_dump->first.at(TEST_SCOPED_TIMER_NAME);

	ASSERT_EQ(test_scoped_timer.get<handystats::statistics::tag::count>(), TEST_SCOPED_TIMER_NANOSLEEP_COUNT);
	ASSERT_GE(test_scoped_timer.get<handystats::statistics::tag::min>(), TEST_SCOPED_TIMER_NANOSLEEP_COUNT / 1000.0);
}

TEST(CBindingTest, TestBoolAttr) {
	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->second.find(TEST_BOOL_ATTR_NAME) != metrics_dump->second.end());

	const auto& test_bool_attr = metrics_dump->second.at(TEST_BOOL_ATTR_NAME);

	ASSERT_EQ(boost::get<bool>(test_bool_attr.value()), TEST_BOOL_ATTR_VALUE);
}

TEST(CBindingTest, TestDoubleAttr) {
	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->second.find(TEST_DOUBLE_ATTR_NAME) != metrics_dump->second.end());

	const auto& test_double_attr = metrics_dump->second.at(TEST_DOUBLE_ATTR_NAME);

	ASSERT_NEAR(boost::get<double>(test_double_attr.value()), TEST_DOUBLE_ATTR_VALUE, 1E-9);
}

TEST(CBindingTest, TestIntAttr) {
	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->second.find(TEST_INT_ATTR_NAME) != metrics_dump->second.end());

	const auto& test_int_attr = metrics_dump->second.at(TEST_INT_ATTR_NAME);

	ASSERT_EQ(boost::get<int>(test_int_attr.value()), TEST_INT_ATTR_VALUE);
}

TEST(CBindingTest, TestUintAttr) {
	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->second.find(TEST_UINT_ATTR_NAME) != metrics_dump->second.end());

	const auto& test_uint_attr = metrics_dump->second.at(TEST_UINT_ATTR_NAME);

	ASSERT_EQ(boost::get<unsigned>(test_uint_attr.value()), TEST_UINT_ATTR_VALUE);
}

TEST(CBindingTest, TestInt64Attr) {
	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->second.find(TEST_INT64_ATTR_NAME) != metrics_dump->second.end());

	const auto& test_int64_attr = metrics_dump->second.at(TEST_INT64_ATTR_NAME);

	ASSERT_EQ(boost::get<int64_t>(test_int64_attr.value()), TEST_INT64_ATTR_VALUE);
}

TEST(CBindingTest, TestUint64Attr) {
	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->second.find(TEST_UINT64_ATTR_NAME) != metrics_dump->second.end());

	const auto& test_uint64_attr = metrics_dump->second.at(TEST_UINT64_ATTR_NAME);

	ASSERT_EQ(boost::get<uint64_t>(test_uint64_attr.value()), TEST_UINT64_ATTR_VALUE);
}

TEST(CBindingTest, TestStringAttr) {
	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->second.find(TEST_STRING_ATTR_NAME) != metrics_dump->second.end());

	const auto& test_string_attr = metrics_dump->second.at(TEST_STRING_ATTR_NAME);

	ASSERT_EQ(boost::get<std::string>(test_string_attr.value()), TEST_STRING_ATTR_VALUE);
}


int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);

	run_tests();

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto ret = RUN_ALL_TESTS();

	HANDY_FINALIZE();

	return ret;
}
