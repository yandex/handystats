#include <gtest/gtest.h>

#include <boost/accumulators/accumulators.hpp>

#include <handystats/accumulators/moving_average.hpp>

using namespace boost::accumulators;

TEST(MovingAverageTest, TestMovingAverageConstruction) {
	accumulator_set<double, features<tag::moving_average>> avg(
			tag::moving_average::alpha = 0.66
			);

	ASSERT_EQ(count(avg), 0);
	ASSERT_NEAR(moving_average(avg), 0.0, 1E-8);
}

TEST(MovingAverageTest, AverageOfSingleEntryEqualToItsValue) {
	accumulator_set<double, features<tag::moving_average>> avg(
			tag::moving_average::alpha = 0.66
			);

	avg(10);

	ASSERT_NEAR(moving_average(avg), 10, 1E-8);
}

TEST(MovingAverageTest, AverageOfUnitElementsEqual1) {
	accumulator_set<double, features<tag::moving_average>> avg(
			tag::moving_average::alpha = 0.734
			);

	for (int step = 0; step < 100; ++step) {
		avg(1);
	}

	ASSERT_NEAR(moving_average(avg), 1, 1E-8);
}

TEST(MovingAverageTest, AverageWithAlpha1EqualLastElement) {
	accumulator_set<double, features<tag::moving_average>> avg(
			tag::moving_average::alpha = 1
			);

	avg(100);
	ASSERT_NEAR(moving_average(avg), 100, 1E-8);

	avg(1);
	ASSERT_NEAR(moving_average(avg), 1, 1E-8);

	avg(-200);
	ASSERT_NEAR(moving_average(avg), -200, 1E-8);
}

TEST(MovingAverageTest, AverageWithAlpha0EqualFirstElement) {
	accumulator_set<double, features<tag::moving_average>> avg(
			tag::moving_average::alpha = 0
			);

	avg(100);
	ASSERT_NEAR(moving_average(avg), 100, 1E-8);

	avg(1);
	ASSERT_NEAR(moving_average(avg), 100, 1E-8);

	avg(-200);
	ASSERT_NEAR(moving_average(avg), 100, 1E-8);
}

