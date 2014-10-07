// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_METRICS_TIMER_HPP_
#define HANDYSTATS_CONFIG_METRICS_TIMER_HPP_

#include <handystats/chrono.hpp>
#include <handystats/config/statistics.hpp>

namespace handystats { namespace config { namespace metrics {

struct timer {
	chrono::duration idle_timeout;
	statistics values;

	timer();
};

}}} // namespace handystats::config::metrics

#endif // HANDYSTATS_CONFIG_METRICS_TIMER_HPP_
