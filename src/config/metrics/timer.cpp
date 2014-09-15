// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <chrono>

#include <handystats/config/metrics/timer.hpp>

namespace handystats { namespace config { namespace metrics {

const chrono::clock::duration timer::defaults::idle_timeout =
		chrono::duration_cast<chrono::clock::duration>(
			std::chrono::seconds(10)
		);
const statistics timer::defaults::values = statistics();

timer::timer()
	: idle_timeout(timer::defaults::idle_timeout)
	, values(timer::defaults::values)
{
}

void timer::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("idle-timeout")) {
		const rapidjson::Value& idle_timeout = config["idle-timeout"];
		if (idle_timeout.IsUint64()) {
			this->idle_timeout =
				chrono::duration_cast<chrono::clock::duration>(
					std::chrono::milliseconds(idle_timeout.GetUint64())
				);
		}
	}

	if (config.HasMember("values")) {
		const rapidjson::Value& values = config["values"];
		this->values.configure(values);
	}
}

}}} // namespace handystats::config::metrics