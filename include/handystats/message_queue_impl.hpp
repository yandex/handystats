#ifndef HANDYSTATS_MESSAGE_QUEUE_IMPL_H_
#define HANDYSTATS_MESSAGE_QUEUE_IMPL_H_

#include <tbb/concurrent_queue.h>

#include "handystats/events/event_message.hpp"

namespace handystats { namespace message_queue {

extern tbb::concurrent_queue<events::event_message*>* event_message_queue;

void push_event_message(events::event_message* message);
events::event_message* pop_event_message();

void initialize();
void finalize();

}} // namespace handystats::message_queue

#endif // HANDYSTATS_MESSAGE_QUEUE_H_
