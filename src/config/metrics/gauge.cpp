// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/config/metrics/gauge.hpp>

#include "config/statistics_impl.hpp"
#include "config/metrics/gauge_impl.hpp"

namespace handystats { namespace config { namespace metrics {

gauge::gauge()
	: values(statistics())
{
}

}}} // namespace handystats::config::metrics


namespace handystats { namespace config {

void apply(const rapidjson::Value& config, metrics::gauge& gauge_opts) {
	apply(config, gauge_opts.values);
}

}} // namespace handystats::config
