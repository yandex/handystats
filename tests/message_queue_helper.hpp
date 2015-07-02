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

#ifndef HANDYSTATS_TESTS_MESSAGE_QUEUE_HELPER_HPP_
#define HANDYSTATS_TESTS_MESSAGE_QUEUE_HELPER_HPP_

#include <thread>
#include <chrono>

#include "message_queue_impl.hpp"

namespace handystats { namespace message_queue {

void wait_until_empty() {
	while (!handystats::message_queue::empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

}} // namespace handystats::message_queue

#endif // HANDYSTATS_TESTS_MESSAGE_QUEUE_HELPER_HPP_
