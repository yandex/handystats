// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/config/metrics/counter.hpp>

namespace handystats { namespace config { namespace metrics {

const statistics counter::defaults::values = statistics();
const statistics counter::defaults::incr_deltas = statistics();
const statistics counter::defaults::decr_deltas = statistics();
const statistics counter::defaults::deltas = statistics();

counter::counter()
	: values(counter::defaults::values)
	, incr_deltas(counter::defaults::incr_deltas)
	, decr_deltas(counter::defaults::decr_deltas)
	, deltas(counter::defaults::deltas)
{
}

void counter::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("values")) {
		const rapidjson::Value& values = config["values"];
		this->values.configure(values);
	}
	if (config.HasMember("incr-deltas")) {
		const rapidjson::Value& incr_deltas = config["incr-deltas"];
		this->incr_deltas.configure(incr_deltas);
	}
	if (config.HasMember("decr-deltas")) {
		const rapidjson::Value& decr_deltas = config["decr-deltas"];
		this->decr_deltas.configure(decr_deltas);
	}
	if (config.HasMember("deltas")) {
		const rapidjson::Value& deltas = config["deltas"];
		this->deltas.configure(deltas);
	}
}

}}} // namespace handystats::config::metrics
