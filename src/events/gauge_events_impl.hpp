// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_GAUGE_EVENT_IMPL_H_
#define HANDYSTATS_GAUGE_EVENT_IMPL_H_

#include <string>
#include <memory>

#include <handystats/metrics/gauge.hpp>

#include "events/event_message_impl.hpp"


namespace handystats { namespace events {

namespace gauge_event {
enum {
	INIT,
	SET
};
}


event_message_ptr gauge_init_event(
		const std::string& gauge_name,
		const metrics::gauge::value_type& init_value,
		const metrics::gauge::time_point& timestamp
	);

event_message_ptr gauge_set_event(
		const std::string& gauge_name,
		const metrics::gauge::value_type& value,
		const metrics::gauge::time_point& timestamp
	);


void delete_gauge_event(event_message* message);

}} // namespace handystats::events


#endif // HANDYSTATS_GAUGE_EVENT_H_

