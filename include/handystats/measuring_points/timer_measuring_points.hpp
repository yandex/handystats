#ifndef HANDYSTATS_TIMER_MEASURING_POINTS_H_
#define HANDYSTATS_TIMER_MEASURING_POINTS_H_

#include <string>
#include <cstdint>

#include <handystats/metrics/timer.hpp>

void HANDY_TIMER_INIT(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		);

void HANDY_TIMER_START(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		);

void HANDY_TIMER_STOP(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		);

void HANDY_TIMER_DISCARD(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		);

void HANDY_TIMER_HEARTBEAT(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		);

#endif // HANDYSTATS_TIMER_MEASURING_POINTS_H_
