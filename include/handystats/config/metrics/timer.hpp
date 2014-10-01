// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_METRICS_TIMER_HPP_
#define HANDYSTATS_CONFIG_METRICS_TIMER_HPP_

#include <handystats/chrono.hpp>
#include <handystats/rapidjson/document.h>
#include <handystats/config/statistics.hpp>

namespace handystats { namespace config { namespace metrics {

struct timer {
	chrono::duration idle_timeout;
	statistics values;

	timer();
	void configure(const rapidjson::Value& config);
};

}}} // namespace handystats::config::metrics

#endif // HANDYSTATS_CONFIG_METRICS_TIMER_HPP_
