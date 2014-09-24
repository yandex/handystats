// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/config/metrics/gauge.hpp>

namespace handystats { namespace config { namespace metrics {

gauge::gauge()
	: values(statistics())
{
}

void gauge::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	this->values.configure(config);
}

}}} // namespace handystats::config::metrics
