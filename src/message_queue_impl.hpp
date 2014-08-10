// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_MESSAGE_QUEUE_IMPL_HPP_
#define HANDYSTATS_MESSAGE_QUEUE_IMPL_HPP_

#include <handystats/metrics/gauge.hpp>
#include <handystats/metrics/counter.hpp>

#include "events/event_message_impl.hpp"

namespace handystats { namespace message_queue {

void push_event_message(events::event_message_ptr&&);
events::event_message_ptr pop_event_message();

bool empty();
size_t size();

void initialize();
void finalize();


namespace stats {

extern metrics::gauge size;
extern metrics::gauge message_wait_time;
extern metrics::counter pop_count;

void initialize();
void finalize();

} // namespace stats


}} // namespace handystats::message_queue


#endif // HANDYSTATS_MESSAGE_QUEUE_IMPL_HPP_
