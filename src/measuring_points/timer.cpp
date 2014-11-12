/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <memory>
#include <algorithm>

#include "events/timer_impl.hpp"
#include "core_impl.hpp"

#include <handystats/measuring_points/timer.hpp>
#include <handystats/measuring_points/timer.h>


namespace handystats { namespace measuring_points {

void timer_init(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	if (core) {
		core->m_channel.push(
				events::timer::create_init_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_start(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	if (core) {
		core->m_channel.push(
				events::timer::create_start_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_stop(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	if (core) {
		core->m_channel.push(
				events::timer::create_stop_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_discard(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	if (core) {
		core->m_channel.push(
				events::timer::create_discard_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_heartbeat(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	)
{
	if (core) {
		core->m_channel.push(
				events::timer::create_heartbeat_event(std::move(timer_name), instance_id, timestamp)
			);
	}
}

void timer_set(
		std::string&& timer_name,
		const metrics::timer::value_type& measurement,
		const metrics::timer::time_point& timestamp
	)
{
	if (core) {
		core->m_channel.push(
				events::timer::create_set_event(std::move(timer_name), measurement, timestamp)
			);
	}
}

}} // namespace measuring_points


extern "C" {

void handystats_timer_init(
		const char* timer_name,
		const uint64_t instance_id
	)
{
	handystats::measuring_points::timer_init(timer_name, instance_id);
}

void handystats_timer_start(
		const char* timer_name,
		const uint64_t instance_id
	)
{
	handystats::measuring_points::timer_start(timer_name, instance_id);
}

void handystats_timer_stop(
		const char* timer_name,
		const uint64_t instance_id
	)
{
	handystats::measuring_points::timer_stop(timer_name, instance_id);
}

void handystats_timer_discard(
		const char* timer_name,
		const uint64_t instance_id
	)
{
	handystats::measuring_points::timer_discard(timer_name, instance_id);
}

void handystats_timer_heartbeat(
		const char* timer_name,
		const uint64_t instance_id
	)
{
	handystats::measuring_points::timer_heartbeat(timer_name, instance_id);
}

void handystats_timer_set(
		const char* timer_name,
		const int64_t measurement
	)
{
	handystats::measuring_points::timer_set(
			timer_name,
			handystats::chrono::duration(measurement, handystats::metrics::timer::value_unit)
		);
}

} // extern "C"
