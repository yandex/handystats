// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_MESSAGE_QUEUE_IMPL_HPP_
#define HANDYSTATS_MESSAGE_QUEUE_IMPL_HPP_

#include <handystats/atomic.hpp>
#include <handystats/metrics/gauge.hpp>
#include <handystats/metrics/counter.hpp>

namespace handystats { namespace events {

struct event_message;

}} // namespace handystats::events

namespace handystats { namespace message_queue {

struct node {
	std::atomic<events::event_message*> next;
};

void push(node*);
events::event_message* pop();

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
