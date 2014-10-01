// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <chrono>

#include <handystats/config/metrics/timer.hpp>

namespace handystats { namespace config { namespace metrics {

timer::timer()
	: idle_timeout(10, chrono::time_unit::SEC)
	, values(statistics())
{
}

void timer::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("idle-timeout")) {
		const rapidjson::Value& idle_timeout = config["idle-timeout"];
		if (idle_timeout.IsUint64()) {
			this->idle_timeout = chrono::duration(idle_timeout.GetUint64(), chrono::time_unit::MSEC);
		}
	}

	this->values.configure(config);
}

}}} // namespace handystats::config::metrics
