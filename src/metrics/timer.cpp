// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/timer.hpp>

namespace handystats { namespace metrics {

const timer::instance_id_type timer::DEFAULT_INSTANCE_ID = -1;

timer::timer(
		const config::metrics::timer& timer_opts
	)
	: m_idle_timeout(timer_opts.idle_timeout)
	, m_values(timer_opts.values)
	, m_idle_check_timestamp()
{
}

void timer::start(const instance_id_type& instance_id, const time_point& timestamp) {
	check_idle_timeout(timestamp);

	auto& instance = m_instances[instance_id];
	instance.start_timestamp = timestamp;
	instance.heartbeat_timestamp = timestamp;
}

void timer::stop(const instance_id_type& instance_id, const time_point& timestamp) {
	check_idle_timeout(timestamp);

	auto instance = m_instances.find(instance_id);
	if (instance == m_instances.end()) {
		return;
	}

	if (instance->second.expired(m_idle_timeout, timestamp)) {
		m_instances.erase(instance);
		return;
	}

	auto instance_value = timestamp - instance->second.start_timestamp;

	m_values.update(chrono::duration_cast<value_type>(instance_value).count(), timestamp);

	m_instances.erase(instance);
}

void timer::heartbeat(const instance_id_type& instance_id, const time_point& timestamp) {
	check_idle_timeout(timestamp);

	auto instance = m_instances.find(instance_id);
	if (instance == m_instances.end()) {
		return;
	}

	if (instance->second.expired(m_idle_timeout, timestamp)) {
		m_instances.erase(instance);
		return;
	}

	instance->second.heartbeat_timestamp = timestamp;
}

void timer::discard(const instance_id_type& instance_id, const time_point& timestamp) {
	check_idle_timeout(timestamp);

	m_instances.erase(instance_id);
}

void timer::set(const clock::duration& measurement, const time_point& timestamp) {
	m_values.update(chrono::duration_cast<value_type>(measurement).count(), timestamp);
}

void timer::check_idle_timeout(const time_point& timestamp, const bool& force) {
	if (!force) {
		if (timestamp < m_idle_check_timestamp + m_idle_timeout) {
			return;
		}
	}

	for (auto instance = m_instances.begin(); instance != m_instances.end();) {
		if (instance->second.expired(m_idle_timeout, timestamp)) {
			instance = m_instances.erase(instance);
		}
		else {
			++instance;
		}
	}

	if (m_idle_check_timestamp < timestamp) {
		m_idle_check_timestamp = timestamp;
	}
}

void timer::update_statistics(const time_point& timestamp) {
	m_values.update_time(timestamp);
}

const statistics& timer::values() const {
	return m_values;
}

}} // namespace handystats::metrics
