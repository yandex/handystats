// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/config/metrics/counter.hpp>

namespace handystats { namespace config { namespace metrics {

counter::counter()
	: values(statistics())
{
}

void counter::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	this->values.configure(config);
}

}}} // namespace handystats::config::metrics
