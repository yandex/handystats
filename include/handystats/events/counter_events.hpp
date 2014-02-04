#ifndef HANDYSTATS_COUNTER_EVENT_H_
#define HANDYSTATS_COUNTER_EVENT_H_

#include "handystats/metrics/counter.hpp"
#include "handystats/events/event_message.hpp"

namespace handystats { namespace events {

enum class counter_event {
	INIT,
	INCREMENT,
	DECREMENT
};


event_message* counter_init_event(
		const std::string counter_name,
		metrics::counter::value_type init_value,
		metrics::counter::time_point timestamp = metrics::counter::clock::now()
		);

event_message* counter_increment_event(
		const std::string counter_name,
		metrics::counter::value_type value,
		metrics::counter::time_point timestamp = metrics::counter::clock::now()
		);

event_message* counter_decrement_event(
		const std::string counter_name,
		metrics::counter::value_type value,
		metrics::counter::time_point timestamp = metrics::counter::clock::now()
		);


void delete_counter_init_event(event_message*);
void delete_counter_increment_event(event_message*);
void delete_counter_decrement_event(event_message*);

void delete_counter_event(event_message*);


}} // namespace handystats::events


inline void HANDY_COUNTER_INIT(
		const std::string counter_name,
		handystats::metrics::counter::value_type init_value,
		handystats::metrics::counter::time_point timestamp = handystats::metrics::counter::clock::now()
		);

inline void HANDY_COUNTER_INCREMENT(
		const std::string counter_name,
		handystats::metrics::counter::value_type value,
		handystats::metrics::counter::time_point timestamp = handystats::metrics::counter::clock::now()
		);

inline void HANDY_COUNTER_DECREMENT(
		const std::string counter_name,
		handystats::metrics::counter::value_type value,
		handystats::metrics::counter::time_point timestamp = handystats::metrics::counter::clock::now()
		);

#endif // HANDYSTATS_COUNTER_EVENT_H_
