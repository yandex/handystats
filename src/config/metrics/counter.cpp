// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/config/metrics/counter.hpp>

#include "config/statistics_impl.hpp"
#include "config/metrics/counter_impl.hpp"

namespace handystats { namespace config { namespace metrics {

counter::counter()
	: values(statistics())
{
}

}}} // namespace handystats::config::metrics


namespace handystats { namespace config {

void apply(const rapidjson::Value& config, metrics::counter& counter_opts) {
	apply(config, counter_opts.values);
}

}} // namespace handystats::config
