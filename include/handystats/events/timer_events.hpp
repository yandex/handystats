#ifndef HANDYSTATS_TIMER_EVENT_H_
#define HANDYSTATS_TIMER_EVENT_H_

#include <cstdint>

#include "handystats/events/event_message.hpp"
#include "handystats/metrics/timer.hpp"

namespace handystats { namespace events {

enum class timer_event {
	INIT,
	START,
	FINISH,
	DISCARD,
	HEARTBEAT
};


event_message* timer_init_event(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		metrics::timer::time_point timestamp = metrics::timer::clock::now()
		);

event_message* timer_start_event(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		metrics::timer::time_point timestamp = metrics::timer::clock::now()
		);

event_message* timer_finish_event(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		metrics::timer::time_point timestamp = metrics::timer::clock::now()
		);

event_message* timer_discard_event(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		metrics::timer::time_point timestamp = metrics::timer::clock::now()
		);

event_message* timer_heartbeat_event(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		metrics::timer::time_point timestamp = metrics::timer::clock::now()
		);


void delete_timer_init_event(event_message*);
void delete_timer_start_event(event_message*);
void delete_timer_finish_event(event_message*);
void delete_timer_discard_event(event_message*);
void delete_timer_heartbeat_event(event_message*);

void delete_timer_event(event_message*);


}} // namespace handystats::events


#endif // HANDYSTATS_TIMER_EVENT_H_
