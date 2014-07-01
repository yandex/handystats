// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_MESSAGE_QUEUE_IMPL_H_
#define HANDYSTATS_MESSAGE_QUEUE_IMPL_H_

#include <memory>

#include "events/event_message_impl.hpp"

namespace handystats { namespace message_queue {

void push_event_message(events::event_message_ptr&&);
events::event_message_ptr pop_event_message();

bool empty();
size_t size();

void initialize();
void finalize();

}} // namespace handystats::message_queue


#endif // HANDYSTATS_MESSAGE_QUEUE_IMPL_H_
