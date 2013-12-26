#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include <handystats/storage.hpp>
#include <handystats/persistent_storage.hpp>

#include <handystats/statistics.hpp>
#include <handystats/incremental_statistics.hpp>

#include <handystats/observation.hpp>
#include <handystats/measurement_wrapper.hpp>

#include <handystats/monotonic_counter.hpp>

TEST(Observation, PersistentStoresValuesCorrectly) {
	std::vector<handystats::measurement_wrapper<int>> data_block{0, 1, 2, 3, 4, 5, 6};

	handystats::observation<
		handystats::measurement_wrapper<int>,
		handystats::persistent_storage,
		handystats::no_statistics
	> observation;

	for (const auto& item : data_block) {
		observation.add_measurement(item);
	}

	auto measurements = observation.storage::get_measurements();
	ASSERT_EQ(data_block.size(), measurements.size());

	for (int index = 0; index < measurements.size(); ++index) {
		ASSERT_EQ(data_block[index].get_value(), measurements[index].get_value());
	}
}

TEST(Observation, IncrementalStatisticsCollectsCorrectly) {
	std::vector<handystats::measurement_wrapper<int>> data_block{0, 1, 2, 3, 4, 5, 6};

	handystats::observation<
		handystats::measurement_wrapper<int>,
		handystats::no_storage,
		handystats::incremental_statistics
	> observation;

	for (const auto& item : data_block) {
		observation.add_measurement(item);
	}

	ASSERT_NEAR(observation.statistics::get_mean(), 3, 1E-9);
	ASSERT_NEAR(observation.statistics::get_min(), 0, 1E-9);
	ASSERT_NEAR(observation.statistics::get_sum(), 21, 1E-9);
}

TEST(Observation, TestIncrementalStatisticsOnHighLoad) {
	handystats::observation<
		handystats::measurement_wrapper<int>,
		handystats::no_storage,
		handystats::incremental_statistics
	> observation;

	const int MAX_MEASUREMENT = 50000000;

	for (int i = 0; i < MAX_MEASUREMENT; i++) {
		observation.add_measurement(i);
	}

	ASSERT_EQ(observation.statistics::get_count(), MAX_MEASUREMENT);

	ASSERT_NEAR(observation.statistics::get_max(), MAX_MEASUREMENT - 1, 1E-9);
	ASSERT_NEAR(observation.statistics::get_min(), 0, 1E-9);
	ASSERT_NEAR(observation.statistics::get_sum(), double(MAX_MEASUREMENT - 1) * MAX_MEASUREMENT / 2., 1E-9);
}

TEST(Observation, TestMonotonicCounterWithinObservation) {
	handystats::monotonic_counter<> counter;

	handystats::observation<
		decltype(counter),
		handystats::persistent_storage,
		handystats::incremental_statistics
	> observation;

	const int MAX_RUNS = 10;
	for (int run = 1; run <= MAX_RUNS; ++run) {
		counter.incident();
		observation.add_measurement(counter);
	}

	const auto& measurements = observation.storage::get_measurements();
	ASSERT_EQ(measurements.size(), MAX_RUNS);
	for (int run = 1; run <= MAX_RUNS; ++run) {
		ASSERT_EQ(measurements[run - 1].get_value(), run);
	}

	auto actual_sum = 0;
	for (int run = 1; run <= MAX_RUNS; ++run) {
		actual_sum += run;
	}
	ASSERT_NEAR(observation.statistics::get_sum(), actual_sum, 1E-9);

	ASSERT_NEAR(observation.statistics::get_min(), 1, 1E-9);
	ASSERT_NEAR(observation.statistics::get_max(), MAX_RUNS, 1E-9);

	ASSERT_EQ(observation.statistics::get_count(), MAX_RUNS);
}
