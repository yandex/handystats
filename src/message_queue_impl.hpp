/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#ifndef HANDYSTATS_MESSAGE_QUEUE_IMPL_HPP_
#define HANDYSTATS_MESSAGE_QUEUE_IMPL_HPP_

#include <handystats/atomic.hpp>
#include <handystats/chrono.hpp>
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

void update(const chrono::time_point&);

void initialize();
void finalize();

} // namespace stats


}} // namespace handystats::message_queue


#endif // HANDYSTATS_MESSAGE_QUEUE_IMPL_HPP_
