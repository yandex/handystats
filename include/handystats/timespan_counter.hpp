#ifndef HANDY_TIMESPAN_COUNTER_H_
#define HANDY_TIMESPAN_COUNTER_H_

#include <string>
#include <chrono>

#include <handystats/chrono.hpp>
#include <handystats/counter.hpp>

namespace handystats {

template<class Duration = std::chrono::microseconds>
class timespan_counter : public counter<timespan_counter<Duration>, Duration, Duration>
{
public:
	// XXX check Duration type
	//static_assert(std::is_same<Duration, std::chrono::duration>::value, "duration type must be std::chrono::duration-compatible!");

	typedef counter<timespan_counter<Duration>, Duration, Duration> base_counter;

	typedef typename base_counter::duration_type duration_type;
	typedef typename base_counter::clock clock;
	typedef typename base_counter::time_point time_point;

	timespan_counter()
		: base_counter()
		, running(false)
	{}

	timespan_counter(duration_type value, time_point initial_timestamp = clock::now())
		: base_counter(value, initial_timestamp)
		, running(false)
	{}

	void reset(const duration_type& value = duration_type::zero(), const time_point& timestamp = clock::now()) {
		base_counter::reset(value, timestamp);

		running = false;
		span_start_time = time_point::min();
		last_span_duration = duration_type::zero();
	}

	bool is_running() const {
		return running;
	}

	time_point get_span_start_time() const {
		return span_start_time;
	}

	duration_type get_last_span_duration(const time_point& timestamp = clock::now()) const {
		if (!running) {
			return last_span_duration;
		}
		else {
			return timestamp - span_start_time;
		}
	}

	duration_type get_running_time(const time_point& timestamp = clock::now()) const {
		if (!running) {
			return this->value;
		}
		else {
			return this->value + get_last_span_duration(timestamp);
		}
	}

	duration_type get_value() const {
		return get_running_time();
	}

	void start(const time_point& timestamp = clock::now()) {
		this->update_timestamp(timestamp);

		if (!running) {
			running = true;
			span_start_time = timestamp;
		}
	}

	void stop(const time_point& timestamp = clock::now()) {
		this->update_timestamp(timestamp);

		if (running) {
			last_span_duration = timestamp - span_start_time;
			this->value += last_span_duration;

			span_start_time = time_point::min();
			running = false;
		}
	}

protected:
	bool running;
	time_point span_start_time;
	duration_type last_span_duration;
};

} // namespace handystats

#endif // HANDY_TIMESPAN_COUNTER_H_
