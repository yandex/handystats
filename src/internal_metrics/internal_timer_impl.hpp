#ifndef HANDYSTATS_INTERNAL_TIMER_H_
#define HANDYSTATS_INTERNAL_TIMER_H_

#include <cstdint>
#include <unordered_map>

#include <handystats/metrics/timer.hpp>
#include <handystats/metrics/gauge.hpp>

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

	struct timer_instance {
		metrics::timer* timer;
		time_point timestamp;

		timer_instance() {
			timer = nullptr;
			timestamp = time_point();
		}

		~timer_instance() {
			if (timer) {
				delete timer;
			}
		}
	};

	std::unordered_map<uint64_t, timer_instance> instances;
	metrics::gauge aggregator;
	time_point timestamp;

	time_point check_timestamp;

	internal_timer() {
		timestamp = time_point();
		check_timestamp = time_point();
	}

	~internal_timer()
	{}

	void check_on_timeout(time_point timestamp);

	void process_event_message(const events::event_message& message);

	void process_init_event(const events::event_message& message);
	void process_start_event(const events::event_message& message);
	void process_stop_event(const events::event_message& message);
	void process_discard_event(const events::event_message& message);
	void process_heartbeat_event(const events::event_message& message);
};

}} // namespace handystats::internal


#endif // HANDYSTATS_INTERNAL_TIMER_H_
