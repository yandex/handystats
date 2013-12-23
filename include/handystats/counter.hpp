#ifndef HANDY_COUNTER_H_
#define HANDY_COUNTER_H_

#include <chrono>
#include <string>

namespace handystats {

template<class Counter_impl, class Value = int, class Duration = std::chrono::microseconds>
class counter
{
public:
	typedef Value value_type;
	typedef Duration duration_type;
	typedef typename chrono::steady_clock<duration_type> clock;
	typedef typename clock::time_point time_point;

	counter()
		: value(0)
		, first_timestamp(time_point::min())
		, last_timestamp(time_point::min())
	{}

	counter(value_type value, time_point initial_timestamp)
		: value(value)
		, first_timestamp(initial_timestamp)
		, last_timestamp(initial_timestamp)
	{}

	void update_timestamp(const time_point& timestamp) {
		last_timestamp = timestamp;
		if (first_timestamp == time_point::min()) {
			first_timestamp = timestamp;
		}
	}

	void reset(value_type value, time_point timestamp = clock::now()) {
		this->value = value;
		first_timestamp = timestamp;
		last_timestamp = timestamp;
	}

	value_type get_value() const {
		return value;
	}

	time_point get_first_timestamp() const {
		return first_timestamp;
	}

	time_point get_last_timestamp() const {
		return last_timestamp;
	}

	duration_type get_active_time() const {
		return last_timestamp - first_timestamp;
	}

	std::string to_string() const {
		return std::string("NOT IMPLEMENTED");
	}

protected:
	value_type value;

	time_point first_timestamp;
	time_point last_timestamp;
};

} // namespace handystats

#endif // HANDY_COUNTER_H_
