#ifndef HANDY_GENERALIZED_COUNTER_H_
#define HANDY_GENERALIZED_COUNTER_H_

#include <string>
#include <chrono>

#include <handystats/chrono.hpp>
#include <handystats/counter.hpp>

namespace handystats {

template<class Value = int, class Duration = std::chrono::microseconds>
class generalized_counter : public counter<generalized_counter<Value, Duration>, Value, Duration>
{
public:
	// XXX check Value type for some condition (integer, arithmetic, ...)
	// or leave the decision to the user
	static_assert(std::is_arithmetic<Value>::value, "Value type must be arithmetic!");

	typedef counter<generalized_counter<Value, Duration>, Value, Duration> base_counter;

	typedef typename base_counter::value_type value_type;
	typedef typename base_counter::duration_type duration_type;
	typedef typename base_counter::clock clock;
	typedef typename base_counter::time_point time_point;

	generalized_counter()
		: base_counter()
	{}

	generalized_counter(value_type value, time_point initial_timestamp)
		: base_counter(value, initial_timestamp)
	{}

	void reset(value_type value = 0, time_point timestamp = clock::now()) {
		base_counter::reset(value, timestamp);
	}

	void set_value(value_type value, time_point timestamp = clock::now()) {
		this->update_timestamp(timestamp);
		this->value = value;
	}

	void increment(value_type inc_value = 1, time_point timestamp = clock::now()) {
		this->update_timestamp(timestamp);
		this->value += inc_value;
	}

	void decrement(value_type dec_value = 1, time_point timestamp = clock::now()) {
		this->update_timestamp(timestamp);
		this->value -= dec_value;
	}
};

} // namespace handystats

#endif // HANDY_GENERALIZED_COUNTER_H_
