#ifndef HANDYSTATS_MESSAGE_QUEUE_IMPL_H_
#define HANDYSTATS_MESSAGE_QUEUE_IMPL_H_

#include <memory>


namespace handystats { namespace events {

struct event_message;

}} // namespace handystats::events


namespace handystats { namespace message_queue {

void push_event_message(std::shared_ptr<events::event_message>);
std::shared_ptr<events::event_message> pop_event_message();

bool empty();
size_t size();

void initialize();
void finalize();

}} // namespace handystats::message_queue


#endif // HANDYSTATS_MESSAGE_QUEUE_IMPL_H_
