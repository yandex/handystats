// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_TESTS_MESSAGE_QUEUE_HELPER_HPP_
#define HANDYSTATS_TESTS_MESSAGE_QUEUE_HELPER_HPP_

#include <thread>
#include <chrono>

#include "core_impl.hpp"

namespace handystats {

void wait_until_empty() {
	while (core && core->m_channel.size() > 0) {
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

} // namespace handystats

#endif // HANDYSTATS_TESTS_MESSAGE_QUEUE_HELPER_HPP_
