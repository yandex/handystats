#ifndef HANDYSTATS_METRICS_TIMER_H_
#define HANDYSTATS_METRICS_TIMER_H_

#include <utility>
#include <unordered_map>

#include <handystats/chrono.hpp>
#include <handystats/configuration/defaults.hpp>
#include <handystats/incremental_statistics.hpp>

namespace handystats { namespace metrics {

struct timer
{
	typedef chrono::default_clock clock;
	typedef typename chrono::default_duration value_type;
	typedef chrono::default_time_point time_point;
	typedef uint64_t instance_id_type;

	static const instance_id_type DEFAULT_INSTANCE_ID = -1;

	struct instance_state {
		time_point start_timestamp;
		time_point heartbeat_timestamp;

		instance_state()
			: start_timestamp()
			, heartbeat_timestamp()
		{
		}
	};

	timer();

	void start(time_point timestamp = clock::now(), instance_id_type instance_id = DEFAULT_INSTANCE_ID);
	void stop(time_point timestamp = clock::now(), instance_id_type instance_id = DEFAULT_INSTANCE_ID);
	void heartbeat(time_point timestamp = clock::now(), instance_id_type instance_id = DEFAULT_INSTANCE_ID);
	void discard(time_point timestamp = clock::now(), instance_id_type instance_id = DEFAULT_INSTANCE_ID);

	void check_timeout(
			time_point timestamp = clock::now(),
			clock::duration idle_timeout = std::chrono::duration_cast<clock::duration>(config::defaults::timer::idle_timeout)
			);

	time_point timestamp;
	value_type value;

	incremental_statistics values;

	std::unordered_map<instance_id_type, instance_state> instances;

}; // struct timer

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_TIMER_H_
