/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#include <handystats/metrics/timer.hpp>

namespace handystats { namespace metrics {

const timer::instance_id_type timer::DEFAULT_INSTANCE_ID = -1;
const chrono::time_unit timer::value_unit = chrono::time_unit::USEC;

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

	const auto& instance_value =
		chrono::duration::convert_to(value_unit, timestamp - instance->second.start_timestamp);

	m_values.update(instance_value.count(), timestamp);

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

void timer::set(const value_type& measurement, const time_point& timestamp) {
	m_values.update(chrono::duration::convert_to(value_unit, measurement).count(), timestamp);
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
