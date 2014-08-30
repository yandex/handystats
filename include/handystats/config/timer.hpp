// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_TIMER_HPP_
#define HANDYSTATS_CONFIG_TIMER_HPP_

#include <chrono>

namespace handystats { namespace config {

struct timer {
	struct defaults {
		static const std::chrono::milliseconds idle_timeout;
	};

	std::chrono::milliseconds idle_timeout;

	timer();
};

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_TIMER_HPP_
