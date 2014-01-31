#include <gtest/gtest.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics/gauge.hpp>

using namespace handystats::metrics;

TEST(GaugeTest, TestGaugeConstruction) {
	gauge sample_gauge(-10, handystats::chrono::steady_clock<gauge::time_duration>::now());

	ASSERT_NEAR(sample_gauge.get().first, -10, 1E-9);
}

TEST(GaugeTest, TestGaugeSetMethod) {
	gauge sample_gauge;

	for (int test_value = -1E3; test_value < 1E3; ++test_value) {
		sample_gauge.set(test_value, handystats::chrono::steady_clock<gauge::time_duration>::now());

		ASSERT_NEAR(sample_gauge.get().first, test_value, 1E-9);
	}
}

TEST(GaugeTest, TestGaugeInternalStats) {
	gauge sample_gauge;

	const int min_test_value = -1E3;
	const int max_test_value = 1E3;

	for (int test_value = min_test_value; test_value <= max_test_value; ++test_value) {
		sample_gauge.set(test_value, handystats::chrono::steady_clock<gauge::time_duration>::now());
	}

	ASSERT_NEAR(sample_gauge.get_stats().min_value, min_test_value, 1E-9);
	ASSERT_NEAR(sample_gauge.get_stats().max_value, max_test_value, 1E-9);

	ASSERT_EQ(sample_gauge.get_stats().count, max_test_value - min_test_value + 1);
}
