#include <thread>

#include <gtest/gtest.h>

#include <handystats/statistics.hpp>

#include <handystats/chrono.hpp>

class IncrementalStatisticsTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		opts = handystats::config::statistics();
	}
	virtual void TearDown() {
	}

	handystats::statistics stats;
	handystats::config::statistics opts;
};

TEST_F(IncrementalStatisticsTest, TestIntervalCount) {
	opts.moving_interval = std::chrono::milliseconds(1);

	stats = handystats::statistics(opts);

	ASSERT_NEAR(stats.get<handystats::statistics::tag::moving_count>(), 0.0, 1E-8);

	const int STEP_COUNT = 50;
	const int CYCLE_COUNT = 10;

	for (int cycle = 0; cycle < CYCLE_COUNT; ++cycle) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		for (int step = 0; step < STEP_COUNT; ++step) {
			stats.update(step);
		}

		ASSERT_NEAR(stats.get<handystats::statistics::tag::moving_count>(), STEP_COUNT, 0.05 * STEP_COUNT);
	}
}

TEST_F(IncrementalStatisticsTest, TestIntervalSum) {
	opts.moving_interval = std::chrono::milliseconds(1);

	stats = handystats::statistics(opts);

	ASSERT_NEAR(stats.get<handystats::statistics::tag::moving_sum>(), 0.0, 1E-8);

	const int STEP_COUNT = 50;
	const int CYCLE_COUNT = 10;

	for (int cycle = 0; cycle < CYCLE_COUNT; ++cycle) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		for (int step = 0; step < STEP_COUNT; ++step) {
			stats.update(1);
		}

		ASSERT_NEAR(stats.get<handystats::statistics::tag::moving_sum>(), STEP_COUNT, 0.05 * STEP_COUNT);
	}
}

TEST_F(IncrementalStatisticsTest, TestIntervalMean) {
	opts.moving_interval = std::chrono::milliseconds(1);

	stats = handystats::statistics(opts);

	ASSERT_NEAR(stats.get<handystats::statistics::tag::moving_avg>(), 0.0, 1E-8);

	const int STEP_COUNT = 50;
	const int CYCLE_COUNT = 10;

	for (int cycle = 0; cycle < CYCLE_COUNT; ++cycle) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		handystats::statistics mean_stats;

		for (int step = 0; step < STEP_COUNT; ++step) {
			auto value = rand() % 1000;
			stats.update(value);
			mean_stats.update(value);
		}

		ASSERT_NEAR(
				stats.get<handystats::statistics::tag::moving_avg>(),
				mean_stats.get<handystats::statistics::tag::avg>(),
				0.05 * mean_stats.get<handystats::statistics::tag::avg>()
			);
	}
}

TEST_F(IncrementalStatisticsTest, Histogram25RightTailTest) {
	opts.histogram_bins = 30;
	opts.moving_interval = std::chrono::seconds(1);

	stats = handystats::statistics(opts);

	const size_t TOTAL_COUNT = 10000;
	const double normal_value = 100;
	const double right_tail_value = 1000;

	const handystats::chrono::clock::duration time_span(
			handystats::chrono::duration_cast<handystats::chrono::clock::duration>(opts.moving_interval).count() / TOTAL_COUNT
		);

	for (size_t index = 1; index <= TOTAL_COUNT; ++index) {
		handystats::chrono::clock::time_point current_time(time_span * index);

		if (index % 100 <= 75) {
			stats.update(normal_value + double(rand()) / RAND_MAX, current_time);
		}
		else {
			stats.update(right_tail_value + double(rand()) / RAND_MAX, current_time);
		}
	}

	ASSERT_NEAR(stats.get<handystats::statistics::tag::quantile>().at(0.5), normal_value, normal_value * 0.05);
	ASSERT_NEAR(stats.get<handystats::statistics::tag::quantile>().at(0.99), right_tail_value, right_tail_value * 0.05);
}

TEST_F(IncrementalStatisticsTest, HistogramNormalTest) {
	opts.histogram_bins = 30;
	opts.moving_interval = std::chrono::seconds(1);

	stats = handystats::statistics(opts);

	const size_t TOTAL_COUNT = 10000;
	double normal_value = 100;

	const handystats::chrono::clock::duration time_span(
			handystats::chrono::duration_cast<handystats::chrono::clock::duration>(opts.moving_interval).count() / TOTAL_COUNT
		);

	for (size_t index = 1; index <= TOTAL_COUNT; ++index) {
		handystats::chrono::clock::time_point current_time(time_span * index);
		stats.update(normal_value + double(rand()) / RAND_MAX, current_time);
	}

	ASSERT_NEAR(stats.get<handystats::statistics::tag::quantile>().at(0.5), normal_value, normal_value * 0.05);
	ASSERT_NEAR(stats.get<handystats::statistics::tag::quantile>().at(0.99), normal_value, normal_value * 0.05);

	normal_value *= 2;

	for (size_t index = 1; index <= TOTAL_COUNT; ++index) {
		handystats::chrono::clock::time_point current_time(time_span * (TOTAL_COUNT + index));
		stats.update(normal_value + double(rand()) / RAND_MAX, current_time);
	}

	ASSERT_NEAR(stats.get<handystats::statistics::tag::quantile>().at(0.5), normal_value, normal_value * 0.05);
	ASSERT_NEAR(stats.get<handystats::statistics::tag::quantile>().at(0.99), normal_value, normal_value * 0.05);
}
