#ifndef HANDYSTATS_CHRONO_H_
#define HANDYSTATS_CHRONO_H_

#include <chrono>

namespace handystats { namespace chrono {


template<class time_duration>
class steady_clock {
public:
	typedef time_duration duration;
	typedef typename duration::rep rep;
	typedef typename duration::period period;
	typedef typename std::chrono::time_point<steady_clock> time_point;
	static const bool is_steady = true;

	static time_point now() noexcept {
		timespec ts;
		clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
		return time_point(std::chrono::duration_cast<duration>(std::chrono::seconds(ts.tv_sec) + std::chrono::nanoseconds(ts.tv_nsec)));
	}
};

typedef std::chrono::microseconds default_duration;
typedef steady_clock<default_duration> default_clock;
typedef default_clock::time_point default_time_point;


template<class time_duration>
std::chrono::nanoseconds to_nanoseconds(const time_duration& duration) {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
}

template<class time_duration>
std::chrono::microseconds to_microseconds(const time_duration& duration) {
	return std::chrono::duration_cast<std::chrono::microseconds>(duration);
}

template<class time_duration>
std::chrono::milliseconds to_milliseconds(const time_duration& duration) {
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration);
}

template<class time_duration>
std::chrono::seconds to_seconds(const time_duration& duration) {
	return std::chrono::duration_cast<std::chrono::seconds>(duration);
}

template<class time_duration>
std::chrono::minutes to_minutes(const time_duration& duration) {
	return std::chrono::duration_cast<std::chrono::minutes>(duration);
}

template<class time_duration>
std::chrono::hours to_hours(const time_duration& duration) {
	return std::chrono::duration_cast<std::chrono::hours>(duration);
}

}} // namespace handystats::chrono

#endif // HANDYSTATS_CHRONO_H_
