// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_METRICS_TIMER_HPP_
#define HANDYSTATS_METRICS_TIMER_HPP_

#include <cstdint>

#include <utility>
#include <unordered_map>

#include <handystats/chrono.hpp>
#include <handystats/statistics.hpp>

#include <handystats/config/timer.hpp>
#include <handystats/config/statistics.hpp>

namespace handystats { namespace metrics {

struct timer
{
	typedef chrono::time_duration value_type;
	typedef chrono::clock clock;
	typedef clock::time_point time_point;

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

		bool expired(const clock::duration& idle_timeout, const time_point& timestamp = clock::now()) {
			return (timestamp > heartbeat_timestamp) && (timestamp - heartbeat_timestamp > idle_timeout);
		}
	};

	timer(
			const config::timer& timer_opts = config::timer(),
			const config::statistics& statistics_opts = config::statistics()
		);

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

	void check_idle_timeout(
			const time_point& timestamp = clock::now(),
			const bool& force = false
		);

	void update_statistics(const time_point& timestamp = clock::now());

	const statistics& values() const;

private:
	clock::duration m_idle_timeout;

	statistics m_values;

	std::unordered_map<instance_id_type, instance_state> m_instances;

	time_point m_idle_check_timestamp;

}; // struct timer

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_TIMER_HPP_
