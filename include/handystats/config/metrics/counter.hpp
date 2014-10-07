// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_METRICS_COUNTER_HPP_
#define HANDYSTATS_CONFIG_METRICS_COUNTER_HPP_

#include <handystats/config/statistics.hpp>

namespace handystats { namespace config { namespace metrics {

struct counter {
	statistics values;

	counter();
};

}}} // namespace handystats::config::metrics

#endif // HANDYSTATS_CONFIG_METRICS_COUNTER_HPP_
