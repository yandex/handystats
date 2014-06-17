// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_INTERNAL_TIMER_H_
#define HANDYSTATS_INTERNAL_TIMER_H_

#include <chrono>

#include <handystats/metrics/timer.hpp>
#include <handystats/incremental_statistics.hpp>

namespace handystats { namespace events {

struct event_message;

}} // namespace handystats::events


namespace handystats { namespace internal {

struct internal_timer
{
	typedef typename metrics::timer::value_type value_type;
	typedef typename metrics::timer::clock clock;
	typedef typename clock::duration time_duration;
	typedef typename clock::time_point time_point;

	metrics::timer* base_timer;
	time_point timestamp;
	time_point check_timeout_timestamp;

	internal_timer() {
		base_timer = nullptr;

		timestamp = time_point();
		check_timeout_timestamp = time_point();
	}

	~internal_timer()
	{}

	void check_timeout(
			time_point timestamp = clock::now(),
			clock::duration idle_timeout = std::chrono::duration_cast<clock::duration>(config::defaults::timer::idle_timeout)
			);

	void process_event_message(const events::event_message& message);

	void process_init_event(const events::event_message& message);
	void process_start_event(const events::event_message& message);
	void process_stop_event(const events::event_message& message);
	void process_discard_event(const events::event_message& message);
	void process_heartbeat_event(const events::event_message& message);
};

}} // namespace handystats::internal


#endif // HANDYSTATS_INTERNAL_TIMER_H_
