// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/config/metrics/timer.hpp>

#include "config/statistics_impl.hpp"
#include "config/metrics/timer_impl.hpp"

namespace handystats { namespace config { namespace metrics {

timer::timer()
	: idle_timeout(10, chrono::time_unit::SEC)
	, values(statistics())
{
}

}}} // namespace handystats::config::metrics


namespace handystats { namespace config {

void apply(const rapidjson::Value& config, metrics::timer& timer_opts) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("idle-timeout")) {
		const rapidjson::Value& idle_timeout = config["idle-timeout"];
		if (idle_timeout.IsUint64()) {
			timer_opts.idle_timeout = chrono::milliseconds(idle_timeout.GetUint64());
		}
	}

	apply(config, timer_opts.values);
}

}} // namespace handystats::config
