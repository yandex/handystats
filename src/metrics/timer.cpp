// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/timer.hpp>

#include "configuration_impl.hpp"

namespace handystats { namespace metrics {

const timer::instance_id_type timer::DEFAULT_INSTANCE_ID = -1;

timer::timer()
	: timer(chrono::duration_cast<clock::duration>(config::timer.idle_timeout))
{}

timer::timer(const clock::duration& idle_timeout)
	: idle_timeout(idle_timeout)
{
	this->timestamp = time_point();
	this->value = value_type();
	this->idle_check_timestamp = time_point();
}

void timer::start(const instance_id_type& instance_id, const time_point& timestamp) {
	check_idle_timeout(timestamp);

	auto& instance = instances[instance_id];
	instance.start_timestamp = timestamp;
	instance.heartbeat_timestamp = timestamp;
}

void timer::stop(const instance_id_type& instance_id, const time_point& timestamp) {
	check_idle_timeout(timestamp);

	auto instance = instances.find(instance_id);
	if (instance == instances.end()) {
		return;
	}

	if (instance->second.expired(idle_timeout, timestamp)) {
		instances.erase(instance);
		return;
	}

	auto instance_value = timestamp - instance->second.start_timestamp;

	this->timestamp = timestamp;
	value = chrono::duration_cast<value_type>(instance_value);

	values(value.count(), timestamp);

	instances.erase(instance);
}

void timer::heartbeat(const instance_id_type& instance_id, const time_point& timestamp) {
	check_idle_timeout(timestamp);

	auto instance = instances.find(instance_id);
	if (instance == instances.end()) {
		return;
	}

	if (instance->second.expired(idle_timeout, timestamp)) {
		instances.erase(instance);
		return;
	}

	instance->second.heartbeat_timestamp = timestamp;
}

void timer::discard(const instance_id_type& instance_id, const time_point& timestamp) {
	check_idle_timeout(timestamp);

	instances.erase(instance_id);
}

void timer::check_idle_timeout(const time_point& timestamp, const bool& force) {
	if (!force) {
		if (timestamp < idle_check_timestamp + idle_timeout) {
			return;
		}
	}

	for (auto instance = instances.begin(); instance != instances.end();) {
		if (instance->second.expired(idle_timeout, timestamp)) {
			instance = instances.erase(instance);
		}
		else {
			++instance;
		}
	}

	if (idle_check_timestamp < timestamp) {
		idle_check_timestamp = timestamp;
	}
}

}} // namespace handystats::metrics
