// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/timer.hpp>

namespace handystats { namespace metrics {

const timer::instance_id_type timer::DEFAULT_INSTANCE_ID = -1;

timer::timer() {
	this->timestamp = time_point();
	this->value = value_type();
}

void timer::start(const instance_id_type& instance_id, const time_point& timestamp) {
	auto& instance = instances[instance_id];
	instance.start_timestamp = timestamp;
	instance.heartbeat_timestamp = timestamp;
}

void timer::stop(const instance_id_type& instance_id, const time_point& timestamp) {
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

void timer::heartbeat(const instance_id_type& instance_id, const time_point& timestamp) {
	auto instance = instances.find(instance_id);
	if (instance == instances.end()) {
		return;
	}

	instance->second.heartbeat_timestamp = timestamp;
}

void timer::discard(const instance_id_type& instance_id, const time_point& timestamp) {
	instances.erase(instance_id);
}

void timer::check_timeout(const time_point& timestamp, const clock::duration& idle_timeout) {
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
