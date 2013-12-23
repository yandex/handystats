#ifndef HANDY_MONOTONIC_COUNTER_H_
#define HANDY_MONOTONIC_COUNTER_H_

#include <string>
#include <chrono>

#include "chrono.hpp"
#include "counter.hpp"

namespace handystats {

template<class Value = int, class Duration = std::chrono::microseconds>
class monotonic_counter : public counter<monotonic_counter<Value, Duration>, Value, Duration>
{
public:
	// XXX check Value type for some condition (integer, arithmetic, ...)
	// or leave the decision to the user
	//static_assert(std::numeric_limits<Value>::is_integer, "value type must be integer!");

	typedef counter<monotonic_counter<Value, Duration>, Value, Duration> base_counter;

	typedef typename base_counter::value_type value_type;
	typedef typename base_counter::duration_type duration_type;
	typedef typename base_counter::clock clock;
	typedef typename base_counter::time_point time_point;

	monotonic_counter()
		: base_counter()
	{}

	monotonic_counter(value_type value, time_point initial_timestamp)
		: base_counter(value, initial_timestamp)
	{}

	void reset(value_type value = 0, time_point timestamp = clock::now()) {
		base_counter::value = value;
		base_counter::first_timestamp = timestamp;
		base_counter::last_timestamp = timestamp;
	}

	void increment(value_type inc_value = 1, time_point timestamp = clock::now()) {
		base_counter::value += inc_value;
		base_counter::update_timestamp(timestamp);
	}

	void incident(time_point timestamp = clock::now()) {
		increment(1, timestamp);
	}

	// TODO to_string method should be rewritten
	std::string to_string() const {
		return std::string("[ counter")
			.append("  value: ")
			.append(std::to_string(base_counter::get_value()))
			.append("  active_time: ")
			.append(std::to_string(chrono::to_microseconds(base_counter::get_active_time()).count()))
			.append(" us ]");
	}
};

} // namespace handystats

#endif // HANDY_MONOTONIC_COUNTER_H_
