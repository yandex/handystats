// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/config/metrics/gauge.hpp>

namespace handystats { namespace config { namespace metrics {

const statistics gauge::defaults::values = statistics();

gauge::gauge()
	: values(gauge::defaults::values)
{
}

void gauge::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("values")) {
		const rapidjson::Value& values = config["values"];
		this->values.configure(values);
	}
}

}}} // namespace handystats::config::metrics
