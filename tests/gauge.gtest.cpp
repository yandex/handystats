#include <gtest/gtest.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics/gauge.hpp>

using namespace handystats::metrics;

TEST(GaugeTest, TestGaugeConstruction) {
	gauge sample_gauge;
	sample_gauge.set(-10);

	ASSERT_NEAR(sample_gauge.values().get<handystats::statistics::tag::value>(), -10, 1E-9);
}

TEST(GaugeTest, TestGaugeSetMethod) {
	gauge sample_gauge;

	for (int test_value = -1E3; test_value < 1E3; ++test_value) {
		sample_gauge.set(test_value);

		ASSERT_NEAR(sample_gauge.values().get<handystats::statistics::tag::value>(), test_value, 1E-9);
	}
}

TEST(GaugeTest, TestGaugeInternalStats) {
	gauge sample_gauge;

	const int min_test_value = -1E3;
	const int max_test_value = 1E3;

	for (int test_value = min_test_value; test_value <= max_test_value; ++test_value) {
		sample_gauge.set(test_value);
	}


	const auto& stats = sample_gauge.values();
	ASSERT_NEAR(stats.get<handystats::statistics::tag::min>(), min_test_value, 1E-9);
	ASSERT_NEAR(stats.get<handystats::statistics::tag::max>(), max_test_value, 1E-9);

	ASSERT_EQ(stats.get<handystats::statistics::tag::count>(), max_test_value - min_test_value + 1);
	ASSERT_NEAR(stats.get<handystats::statistics::tag::sum>(), 0, 1E-9);
	ASSERT_NEAR(stats.get<handystats::statistics::tag::avg>(), 0, 1E-9);
}
