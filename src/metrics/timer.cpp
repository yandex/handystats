// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/timer.hpp>

namespace handystats { namespace metrics {

timer::timer() {
	this->timestamp = time_point();
	this->value = value_type();
}

void timer::start(instance_id_type instance_id, time_point timestamp) {
	auto& instance = instances[instance_id];
	instance.start_timestamp = timestamp;
	instance.heartbeat_timestamp = timestamp;
}

void timer::stop(instance_id_type instance_id, time_point timestamp) {
	auto instance = instances.find(instance_id);
	if (instance == instances.end()) {
		return;
	}

	auto instance_value = timestamp - instance->second.start_timestamp;

	this->timestamp = timestamp;
	value = chrono::duration_cast<value_type>(instance_value);

	values(value.count(), timestamp);

	instances.erase(instance);
}

void timer::heartbeat(instance_id_type instance_id, time_point timestamp) {
	auto instance = instances.find(instance_id);
	if (instance == instances.end()) {
		return;
	}

	instance->second.heartbeat_timestamp = timestamp;
}

void timer::discard(instance_id_type instance_id, time_point timestamp) {
	instances.erase(instance_id);
}

void timer::check_timeout(time_point timestamp, clock::duration idle_timeout) {
	for (auto instance = instances.begin(); instance != instances.end();) {
		if (instance->second.heartbeat_timestamp == time_point()) {
			++instance;
			continue;
		}

		if ((timestamp > instance->second.heartbeat_timestamp) && (timestamp - instance->second.heartbeat_timestamp > idle_timeout)) {
			instance = instances.erase(instance);
		}
		else {
			++instance;
		}
	}
}

}} // namespace handystats::metrics
