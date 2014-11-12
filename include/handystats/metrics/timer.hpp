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

#ifndef HANDYSTATS_METRICS_TIMER_HPP_
#define HANDYSTATS_METRICS_TIMER_HPP_

#include <cstdint>

#include <utility>
#include <unordered_map>

#include <handystats/chrono.hpp>
#include <handystats/statistics.hpp>

#include <handystats/config/metrics/timer.hpp>

namespace handystats { namespace metrics {

struct timer
{
	typedef chrono::duration value_type;
	static const chrono::time_unit value_unit;

	typedef chrono::internal_clock clock;
	typedef chrono::time_point time_point;

	typedef uint64_t instance_id_type;

	static const instance_id_type DEFAULT_INSTANCE_ID;

	struct instance_state {
		time_point start_timestamp;
		time_point heartbeat_timestamp;

		instance_state()
			: start_timestamp()
			, heartbeat_timestamp()
		{
		}

		bool expired(const chrono::duration& idle_timeout, const time_point& timestamp = clock::now()) {
			return (timestamp > heartbeat_timestamp) && (timestamp - heartbeat_timestamp > idle_timeout);
		}
	};

	timer(const config::metrics::timer& timer_opts = config::metrics::timer());
	void reset();

	void start(
			const instance_id_type& instance_id = DEFAULT_INSTANCE_ID,
			const time_point& timestamp = clock::now()
		);

	void stop(
			const instance_id_type& instance_id = DEFAULT_INSTANCE_ID,
			const time_point& timestamp = clock::now()
		);

	void heartbeat(
			const instance_id_type& instance_id = DEFAULT_INSTANCE_ID,
			const time_point& timestamp = clock::now()
		);

	void discard(
			const instance_id_type& instance_id = DEFAULT_INSTANCE_ID,
			const time_point& timestamp = clock::now()
		);

	void set(
			const value_type& measurement,
			const time_point& timestamp = clock::now()
		);

	void check_idle_timeout(
			const time_point& timestamp = clock::now(),
			const bool& force = false
		);

	void update_statistics(const time_point& timestamp = clock::now());

	const statistics& values() const;

private:
	chrono::duration m_idle_timeout;

	statistics m_values;

	std::unordered_map<instance_id_type, instance_state> m_instances;

	time_point m_idle_check_timestamp;

}; // struct timer

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_TIMER_HPP_
