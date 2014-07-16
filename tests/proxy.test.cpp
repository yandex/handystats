#include <gtest/gtest.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/operation.hpp>
#include <handystats/configuration.hpp>
#include <handystats/metrics_dump.hpp>

#include "message_queue_helper.hpp"


class HandyProxyTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_CONFIGURATION_JSON(
				"{\
					\"handystats\": {\
						\"metrics-dump\": {\
							\"interval\": 10\
						},\
						\"timer\": {\
							\"idle-timeout\": 500\
						}\
					}\
				}"
			);

		HANDY_INIT();
	}

	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};


TEST_F(HandyProxyTest, GaugeProxy) {
	const char* gauge_name = "gauge";
	const size_t VALUE_MIN = 10;
	const size_t VALUE_MAX = 20;

	handystats::measuring_points::gauge_proxy gauge_proxy(gauge_name);

	for (size_t value = VALUE_MIN; value <= VALUE_MAX; ++value) {
		gauge_proxy.set(value);
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();
	ASSERT_TRUE(metrics_dump->find(gauge_name) != metrics_dump->end());

	auto& gauge = boost::get<handystats::metrics::gauge>(metrics_dump->at(gauge_name));
	ASSERT_EQ(gauge.values.count(), VALUE_MAX - VALUE_MIN + 1);
	ASSERT_EQ(gauge.values.min(), VALUE_MIN);
	ASSERT_EQ(gauge.values.max(), VALUE_MAX);
	ASSERT_EQ(gauge.values.mean(), (VALUE_MIN + VALUE_MAX) / 2);
}

TEST_F(HandyProxyTest, CounterProxy) {
	const char* counter_name = "counter";
	const size_t INIT_VALUE = 10;
	const size_t DELTA = 2;
	const size_t DELTA_STEPS = 10;

	handystats::measuring_points::counter_proxy counter_proxy(counter_name, INIT_VALUE);

	for (size_t step = 0; step < DELTA_STEPS; ++step) {
		counter_proxy.increment(DELTA);
	}

	for (size_t step = 0; step < DELTA_STEPS; ++step) {
		counter_proxy.decrement(DELTA);
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();
	ASSERT_TRUE(metrics_dump->find(counter_name) != metrics_dump->end());

	auto& counter = boost::get<handystats::metrics::counter>(metrics_dump->at(counter_name));

	ASSERT_EQ(counter.value, INIT_VALUE);
	ASSERT_EQ(counter.values.count(), 1 + DELTA_STEPS * 2);
	ASSERT_EQ(counter.values.min(), INIT_VALUE);
	ASSERT_EQ(counter.values.max(), INIT_VALUE + DELTA * DELTA_STEPS);

	ASSERT_EQ(counter.incr_deltas.count(), DELTA_STEPS);
	ASSERT_EQ(counter.incr_deltas.min(), DELTA);
	ASSERT_EQ(counter.incr_deltas.max(), DELTA);

	ASSERT_EQ(counter.decr_deltas.count(), DELTA_STEPS);
	ASSERT_EQ(counter.decr_deltas.min(), DELTA);
	ASSERT_EQ(counter.decr_deltas.max(), DELTA);

	ASSERT_EQ(counter.deltas.count(), 2 * DELTA_STEPS);
	ASSERT_EQ(counter.deltas.min(), -(handystats::metrics::counter::value_type)DELTA);
	ASSERT_EQ(counter.deltas.max(), DELTA);
	ASSERT_EQ(counter.deltas.sum(), 0);
}

TEST_F(HandyProxyTest, TimerProxySingleInstance) {
	const char* timer_name = "timer";
	const handystats::metrics::timer::instance_id_type instance_id = 111;
	const std::chrono::milliseconds sleep_interval(1);
	const size_t SLEEP_COUNT = 10;

	handystats::measuring_points::timer_proxy timer_proxy(timer_name, instance_id);

	for (size_t sleep_step = 0; sleep_step < SLEEP_COUNT; ++sleep_step) {
		timer_proxy.start(sleep_step); // passed instance_id should not override default one
		std::this_thread::sleep_for(sleep_interval);
		timer_proxy.stop(sleep_step + 1); // again, no influence
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();
	ASSERT_TRUE(metrics_dump->find(timer_name) != metrics_dump->end());

	auto& timer = boost::get<handystats::metrics::timer>(metrics_dump->at(timer_name));

	ASSERT_EQ(timer.instances.size(), 0);

	ASSERT_GE(timer.value, handystats::chrono::duration_cast<handystats::chrono::time_duration>(sleep_interval));

	ASSERT_EQ(timer.values.count(), SLEEP_COUNT);
	ASSERT_GE(timer.values.min(), handystats::chrono::duration_cast<handystats::chrono::time_duration>(sleep_interval).count());
}

TEST_F(HandyProxyTest, TimerProxyMultiInstance) {
	const char* timer_name = "timer";
	const std::chrono::milliseconds sleep_interval(1);
	const size_t SLEEP_COUNT = 10;

	handystats::measuring_points::timer_proxy timer_proxy(timer_name);

	for (size_t sleep_step = 0; sleep_step < SLEEP_COUNT; ++sleep_step) {
		timer_proxy.start(sleep_step);
	}

	std::this_thread::sleep_for(sleep_interval);

	for (size_t sleep_step = 0; sleep_step < SLEEP_COUNT; ++sleep_step) {
		timer_proxy.stop(sleep_step);
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();
	ASSERT_TRUE(metrics_dump->find(timer_name) != metrics_dump->end());

	auto& timer = boost::get<handystats::metrics::timer>(metrics_dump->at(timer_name));

	ASSERT_EQ(timer.instances.size(), 0);

	ASSERT_GE(timer.value, handystats::chrono::duration_cast<handystats::chrono::time_duration>(sleep_interval));

	ASSERT_EQ(timer.values.count(), SLEEP_COUNT);
	ASSERT_GE(timer.values.min(), handystats::chrono::duration_cast<handystats::chrono::time_duration>(sleep_interval).count());
}
