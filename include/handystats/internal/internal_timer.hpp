#ifndef HANDYSTATS_INTERNAL_TIMER_H_
#define HANDYSTATS_INTERNAL_TIMER_H_

#include <cstdint>
#include <string>
#include <utility>
#include <unordered_map>

#include <handystats/chrono.hpp>
#include <handystats/metrics/timer.hpp>
#include <handystats/metrics/gauge.hpp>
#include <handystats/events/event_message.hpp>

namespace handystats { namespace internal {

struct internal_timer
{
	typedef typename metrics::timer::time_duration time_duration;
	typedef typename metrics::timer::clock clock;
	typedef typename clock::time_point time_point;

	struct timer_instance {
		metrics::timer* timer;
		time_point timestamp;

		timer_instance() {
			timer = nullptr;
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

	const time_duration TIMEOUT = std::chrono::duration_cast<time_duration>(std::chrono::seconds(5));
	time_point check_timestamp;

	internal_timer()
	{}

	~internal_timer()
	{}

	void check_on_timeout(time_point timestamp);

	void process_event_message(events::event_message* message);

	void process_init_event(events::event_message* message);
	void process_start_event(events::event_message* message);
	void process_stop_event(events::event_message* message);
	void process_discard_event(events::event_message* message);
	void process_heartbeat_event(events::event_message* message);

	std::string to_string() const;
};

}} // namespace handystats::internal


#endif // HANDYSTATS_INTERNAL_TIMER_H_
