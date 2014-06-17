// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_TIMER_EVENT_H_
#define HANDYSTATS_TIMER_EVENT_H_

#include <string>
#include <memory>
#include <cstdint>

#include <handystats/metrics/timer.hpp>

#include "events/event_message_impl.hpp"


namespace handystats { namespace events {

namespace timer_event {
enum {
	INIT,
	START,
	STOP,
	DISCARD,
	HEARTBEAT
};
}


std::shared_ptr<event_message> timer_init_event(
		const std::string timer_name,
		const metrics::timer::instance_id_type instance_id,
		metrics::timer::time_point timestamp
		);

std::shared_ptr<event_message> timer_start_event(
		const std::string timer_name,
		const metrics::timer::instance_id_type instance_id,
		metrics::timer::time_point timestamp
		);

std::shared_ptr<event_message> timer_stop_event(
		const std::string timer_name,
		const metrics::timer::instance_id_type instance_id,
		metrics::timer::time_point timestamp
		);

std::shared_ptr<event_message> timer_discard_event(
		const std::string timer_name,
		const metrics::timer::instance_id_type instance_id,
		metrics::timer::time_point timestamp
		);

std::shared_ptr<event_message> timer_heartbeat_event(
		const std::string timer_name,
		const metrics::timer::instance_id_type instance_id,
		metrics::timer::time_point timestamp
		);


void delete_timer_event(event_message* message);

}} // namespace handystats::events


#endif // HANDYSTATS_TIMER_EVENT_H_
