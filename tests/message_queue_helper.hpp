// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_TESTS_MESSAGE_QUEUE_HELPER_H_
#define HANDYSTATS_TESTS_MESSAGE_QUEUE_HELPER_H_

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

#endif // HANDYSTATS_TESTS_MESSAGE_QUEUE_HELPER_H_
