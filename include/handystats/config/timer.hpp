// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_TIMER_HPP_
#define HANDYSTATS_CONFIG_TIMER_HPP_

#include <chrono>

#include <handystats/rapidjson/document.h>

namespace handystats { namespace config {

struct timer {
	struct defaults {
		static const std::chrono::milliseconds idle_timeout;
	};

	std::chrono::milliseconds idle_timeout;

	timer();
	void configure(const rapidjson::Value& config);
};

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_TIMER_HPP_
