// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_METRICS_COUNTER_HPP_
#define HANDYSTATS_CONFIG_METRICS_COUNTER_HPP_

#include <handystats/rapidjson/document.h>

#include <handystats/config/statistics.hpp>

namespace handystats { namespace config { namespace metrics {

struct counter {
	statistics values;
	statistics incr_deltas;
	statistics decr_deltas;
	statistics deltas;

	counter();
	void configure(const rapidjson::Value& config);
};

}}} // namespace handystats::config::metrics

#endif // HANDYSTATS_CONFIG_METRICS_COUNTER_HPP_
