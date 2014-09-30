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
	opts.moving_interval = handystats::chrono::duration_cast<handystats::chrono::clock::duration>(std::chrono::milliseconds(1));
	opts.tags = handystats::statistics::tag::moving_count;

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
	opts.moving_interval = handystats::chrono::duration_cast<handystats::chrono::clock::duration>(std::chrono::milliseconds(1));
	opts.tags = handystats::statistics::tag::moving_sum;

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
	opts.moving_interval = handystats::chrono::duration_cast<handystats::chrono::clock::duration>(std::chrono::milliseconds(1));
	opts.tags = handystats::statistics::tag::moving_avg;

	stats = handystats::statistics(opts);

	ASSERT_NEAR(stats.get<handystats::statistics::tag::moving_avg>(), 0.0, 1E-8);

	const int STEP_COUNT = 50;
	const int CYCLE_COUNT = 10;

	for (int cycle = 0; cycle < CYCLE_COUNT; ++cycle) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		handystats::config::statistics mean_opts;
		mean_opts.tags = handystats::statistics::tag::avg;
		handystats::statistics mean_stats(mean_opts);

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

TEST_F(IncrementalStatisticsTest, Quantile25RightTailTest) {
	opts.histogram_bins = 30;
	opts.moving_interval = handystats::chrono::duration_cast<handystats::chrono::clock::duration>(std::chrono::seconds(1));
	opts.tags = handystats::statistics::tag::quantile;

	stats = handystats::statistics(opts);

	const size_t TOTAL_COUNT = 10000;
	const double normal_value = 100;
	const double right_tail_value = 1000;

	const handystats::chrono::clock::duration time_span = opts.moving_interval / TOTAL_COUNT;

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

TEST_F(IncrementalStatisticsTest, QuantileNormalTest) {
	opts.histogram_bins = 30;
	opts.moving_interval = handystats::chrono::duration_cast<handystats::chrono::clock::duration>(std::chrono::seconds(1));
	opts.tags = handystats::statistics::tag::quantile;

	stats = handystats::statistics(opts);

	const size_t TOTAL_COUNT = 10000;
	double normal_value = 100;

	const handystats::chrono::clock::duration time_span = opts.moving_interval / TOTAL_COUNT;

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

TEST_F(IncrementalStatisticsTest, HistogramTest) {
	opts.histogram_bins = 10;
	opts.moving_interval = handystats::chrono::duration_cast<handystats::chrono::clock::duration>(std::chrono::seconds(30));
	opts.tags = handystats::statistics::tag::histogram;

	stats = handystats::statistics(opts);

	const size_t BIN_COUNT = 1000;

	for (size_t value = 0; value < 10; ++value) {
		for (size_t count = 0; count < BIN_COUNT; ++count) {
			stats.update(value);
		}
	}

	auto histogram = stats.get<handystats::statistics::tag::histogram>();

	ASSERT_EQ(histogram.size(), 10);
	for (size_t index = 0; index < 10; ++index) {
		ASSERT_NEAR(std::get<handystats::statistics::BIN_CENTER>(histogram[index]), index, 1E-3);
		ASSERT_NEAR(std::get<handystats::statistics::BIN_COUNT>(histogram[index]), BIN_COUNT, 0.05 * BIN_COUNT);
	}
}

TEST_F(IncrementalStatisticsTest, RateMovingCountTest) {
	opts.moving_interval = handystats::chrono::duration_cast<handystats::chrono::clock::duration>(std::chrono::seconds(1));
	opts.rate_unit = opts.moving_interval;
	opts.tags = handystats::statistics::tag::rate |
		handystats::statistics::tag::moving_count |
		handystats::statistics::tag::moving_sum;

	stats = handystats::statistics(opts);

	const size_t COUNT = 1000;

	for (int value = 1; value <= COUNT; ++value) {
		stats.update(value);
		std::this_thread::yield();
	}

	ASSERT_NEAR(
			stats.get<handystats::statistics::tag::rate>(),
			stats.get<handystats::statistics::tag::moving_count>(),
			1E-6
		);
}

TEST_F(IncrementalStatisticsTest, ZeroRateTest) {
	opts.rate_unit = handystats::chrono::duration_cast<handystats::chrono::clock::duration>(std::chrono::seconds(1));
	opts.tags = handystats::statistics::tag::rate;

	stats = handystats::statistics(opts);

	const size_t COUNT = 1000;

	for (int value = 0; value < COUNT; ++value) {
		stats.update(value);
		std::this_thread::yield();
	}

	ASSERT_NEAR(
			stats.get<handystats::statistics::tag::rate>(),
			COUNT - 1,
			0.05 * (COUNT - 1)
		);

	for (int value = COUNT - 1; value >= 0; --value) {
		stats.update(value);
		std::this_thread::yield();
	}

	ASSERT_NEAR(
			stats.get<handystats::statistics::tag::rate>(),
			0,
			0.05 * (COUNT - 1)
		);
}

class StatisticsTagDependency : public ::testing::Test {
protected:
	virtual void SetUp() {
		opts = handystats::config::statistics();
	}
	virtual void TearDown() {
	}

	handystats::statistics stats;
	handystats::config::statistics opts;
};

static size_t count_computed_tags(const handystats::statistics& stats) {
	size_t result = 0;
	result += stats.computed(handystats::statistics::tag::value);
	result += stats.computed(handystats::statistics::tag::min);
	result += stats.computed(handystats::statistics::tag::max);
	result += stats.computed(handystats::statistics::tag::count);
	result += stats.computed(handystats::statistics::tag::sum);
	result += stats.computed(handystats::statistics::tag::avg);
	result += stats.computed(handystats::statistics::tag::moving_count);
	result += stats.computed(handystats::statistics::tag::moving_sum);
	result += stats.computed(handystats::statistics::tag::moving_avg);
	result += stats.computed(handystats::statistics::tag::histogram);
	result += stats.computed(handystats::statistics::tag::quantile);
	result += stats.computed(handystats::statistics::tag::timestamp);

	return result;
}

template <handystats::statistics::tag::type Tag>
static bool check_independent_tag(handystats::statistics& stats) {
	handystats::config::statistics opts;
	opts.tags = Tag;
	stats = handystats::statistics(opts);

	return stats.computed(Tag) && count_computed_tags(stats) == 1;
}

TEST_F(StatisticsTagDependency, IndependentStatisticsCheck) {
	ASSERT_TRUE(check_independent_tag<handystats::statistics::tag::value>(stats));
	ASSERT_TRUE(check_independent_tag<handystats::statistics::tag::min>(stats));
	ASSERT_TRUE(check_independent_tag<handystats::statistics::tag::max>(stats));
	ASSERT_TRUE(check_independent_tag<handystats::statistics::tag::count>(stats));
	ASSERT_TRUE(check_independent_tag<handystats::statistics::tag::sum>(stats));
	ASSERT_TRUE(check_independent_tag<handystats::statistics::tag::timestamp>(stats));
}

TEST_F(StatisticsTagDependency, AvgStatisticsCheck) {
	opts.tags = handystats::statistics::tag::avg;
	stats = handystats::statistics(opts);

	ASSERT_TRUE(stats.enabled(handystats::statistics::tag::avg));
	ASSERT_TRUE(stats.computed(handystats::statistics::tag::count));
	ASSERT_TRUE(stats.computed(handystats::statistics::tag::sum));

	ASSERT_FALSE(stats.computed(handystats::statistics::tag::timestamp));
	ASSERT_FALSE(stats.computed(handystats::statistics::tag::value));
	ASSERT_FALSE(stats.computed(handystats::statistics::tag::min));
	ASSERT_FALSE(stats.computed(handystats::statistics::tag::moving_count));
}

TEST_F(StatisticsTagDependency, MovingAvgStatisticsCheck) {
	opts.tags = handystats::statistics::tag::moving_avg;
	stats = handystats::statistics(opts);

	ASSERT_TRUE(stats.enabled(handystats::statistics::tag::moving_avg));
	ASSERT_TRUE(stats.computed(handystats::statistics::tag::moving_count));
	ASSERT_TRUE(stats.computed(handystats::statistics::tag::moving_sum));
	ASSERT_TRUE(stats.computed(handystats::statistics::tag::timestamp));

	ASSERT_FALSE(stats.computed(handystats::statistics::tag::value));
	ASSERT_FALSE(stats.computed(handystats::statistics::tag::sum));
	ASSERT_FALSE(stats.computed(handystats::statistics::tag::count));
	ASSERT_FALSE(stats.computed(handystats::statistics::tag::avg));
}

TEST_F(StatisticsTagDependency, QuantileStatisticsCheck) {
	opts.tags = handystats::statistics::tag::quantile;
	stats = handystats::statistics(opts);

	ASSERT_TRUE(stats.enabled(handystats::statistics::tag::quantile));
	ASSERT_TRUE(stats.computed(handystats::statistics::tag::histogram));
//	ASSERT_TRUE(stats.computed(handystats::statistics::tag::moving_count));
	ASSERT_TRUE(stats.computed(handystats::statistics::tag::timestamp));

	ASSERT_FALSE(stats.computed(handystats::statistics::tag::value));
	ASSERT_FALSE(stats.computed(handystats::statistics::tag::sum));
	ASSERT_FALSE(stats.computed(handystats::statistics::tag::count));
	ASSERT_FALSE(stats.computed(handystats::statistics::tag::avg));
}

TEST_F(StatisticsTagDependency, HistogramStatisticsCheck) {
	opts.tags = handystats::statistics::tag::histogram;
	stats = handystats::statistics(opts);

	ASSERT_TRUE(stats.computed(handystats::statistics::tag::histogram));
	ASSERT_TRUE(stats.computed(handystats::statistics::tag::timestamp));

	ASSERT_FALSE(stats.computed(handystats::statistics::tag::value));
	ASSERT_FALSE(stats.computed(handystats::statistics::tag::moving_count));
	ASSERT_FALSE(stats.computed(handystats::statistics::tag::moving_sum));
	ASSERT_FALSE(stats.computed(handystats::statistics::tag::quantile));
}
