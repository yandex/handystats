// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_INTERNAL_COUNTER_H_
#define HANDYSTATS_INTERNAL_COUNTER_H_

#include <handystats/metrics/counter.hpp>

namespace handystats { namespace events {

struct event_message;

}} // namespace handystats::events


namespace handystats { namespace internal {

struct internal_counter
{
	typedef typename metrics::counter::clock clock;
	typedef typename clock::time_point time_point;

	metrics::counter* base_counter;
	time_point timestamp;

	internal_counter() {
		base_counter = nullptr;
	}

	~internal_counter() {
		if (base_counter) {
			delete base_counter;
		}
	}

	void process_event_message(const events::event_message& message);

	void process_init_event(const events::event_message& message);
	void process_increment_event(const events::event_message& message);
	void process_decrement_event(const events::event_message& message);
};

}} // namespace handystats::internal

#endif // HANDYSTATS_INTERNAL_COUNTER_H_
