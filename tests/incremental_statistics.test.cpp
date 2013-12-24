#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

#include <gtest/gtest.h>

#include <handystats/math_utils.hpp>
#include <handystats/incremental_statistics.hpp>

TEST(IncrementalStatistics, CheckAllCollectedValues) {
	const double epsilon = std::numeric_limits<double>::epsilon();

	std::vector<int> data{-2, -1, 0, 1, 2};
	std::random_shuffle(data.begin(), data.end());

	handystats::incremental_statistics stats;
	for (const auto& value : data) {
		stats.add_value(value);
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

