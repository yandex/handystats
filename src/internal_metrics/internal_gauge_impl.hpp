// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_INTERNAL_GAUGE_H_
#define HANDYSTATS_INTERNAL_GAUGE_H_

#include <handystats/metrics/gauge.hpp>

namespace handystats { namespace events {

struct event_message;

}} // namespace handystats::events


namespace handystats { namespace internal {

struct internal_gauge
{
	typedef typename metrics::gauge::clock clock;
	typedef typename clock::time_point time_point;

	metrics::gauge* base_gauge;
	time_point timestamp;

	internal_gauge() {
		base_gauge = nullptr;
	}

	~internal_gauge() {
		if (base_gauge) {
			delete base_gauge;
		}
	}

	void process_event_message(const events::event_message& message);

	void process_init_event(const events::event_message& message);
	void process_set_event(const events::event_message& message);
};

}} // namespace handystats::internal

#endif // HANDYSTATS_INTERNAL_GAUGE_H_
