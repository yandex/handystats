#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

#include <gtest/gtest.h>

#include <handystats/math_utils.hpp>
#include <handystats/incremental_statistics.hpp>

class dummy_measurement {
public:
	typedef double value_type;

	dummy_measurement()
		: value(0)
	{}

	dummy_measurement(const double& value)
		: value(value)
	{}

	double get_value() const {
		return value;
	}

	double value;
};

TEST(IncrementalStatistics, CheckAllCollectedValues) {
	const double epsilon = std::numeric_limits<double>::epsilon();

	std::vector<dummy_measurement> data{-2, -1, 0, 1, 2};
	std::random_shuffle(data.begin(), data.end());

	handystats::incremental_statistics<dummy_measurement> stats;
	for (const auto& value : data) {
		stats.add_measurement(value);
	}

	EXPECT_EQ(stats.get_count(), 5);

	EXPECT_NEAR(stats.get_sum(), 0, epsilon);
	EXPECT_NEAR(stats.get_squared_sum(), 10, epsilon);

	EXPECT_NEAR(stats.get_mean(), 0, epsilon);

	EXPECT_NEAR(stats.get_variance(), 2, epsilon);
	EXPECT_NEAR(stats.get_deviation(), 1.414, 0.001);

	EXPECT_NEAR(stats.get_min(), -2, epsilon);
	EXPECT_NEAR(stats.get_max(), 2, epsilon);

	EXPECT_NEAR(stats.get_range(), 4, epsilon);
}

TEST(IncrementalStatistics, CheckThatDataOrderIsInsignificant) {
	const double epsilon = std::numeric_limits<double>::epsilon();

	std::vector<dummy_measurement> data{100, 45, 3, 200, 15, 36, -450, 90, 13};

	handystats::incremental_statistics<dummy_measurement> order_stats;
	for (const auto& elem : data) {
		order_stats.add_measurement(elem);
	}

	std::random_shuffle(std::begin(data), std::end(data));
	handystats::incremental_statistics<dummy_measurement> shuffle_stats;
	for (const auto& elem : data) {
		shuffle_stats.add_measurement(elem);
	}

	EXPECT_EQ(order_stats.get_count(), shuffle_stats.get_count());

	EXPECT_NEAR(order_stats.get_sum(), shuffle_stats.get_sum(), epsilon);
	EXPECT_NEAR(order_stats.get_squared_sum(), shuffle_stats.get_squared_sum(), epsilon);

	EXPECT_NEAR(order_stats.get_min(), shuffle_stats.get_min(), epsilon);
	EXPECT_NEAR(order_stats.get_max(), shuffle_stats.get_max(), epsilon);
}

TEST(IncrementalStatistics, CheckConstructorFromIterators) {
	const double epsilon = std::numeric_limits<double>::epsilon();

	std::vector<dummy_measurement> data{10, 4, -1, 15, 0, 1, 6, 10};

	handystats::incremental_statistics<dummy_measurement> simple_stats;
	for (const auto& value : data) {
		simple_stats.add_measurement(value);
	}

	handystats::incremental_statistics<dummy_measurement> smart_stats(std::begin(data), std::end(data));

	EXPECT_EQ(simple_stats.get_count(), smart_stats.get_count());

	EXPECT_NEAR(simple_stats.get_sum(), smart_stats.get_sum(), epsilon);
	EXPECT_NEAR(simple_stats.get_squared_sum(), smart_stats.get_squared_sum(), epsilon);

	EXPECT_NEAR(simple_stats.get_min(), smart_stats.get_min(), epsilon);
	EXPECT_NEAR(simple_stats.get_max(), smart_stats.get_max(), epsilon);
}
