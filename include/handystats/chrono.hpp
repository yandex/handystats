// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CHRONO_IMPL_H_
#define HANDYSTATS_CHRONO_IMPL_H_

#include <cstdint>
#include <cstdlib>
#include <chrono>

namespace handystats { namespace chrono {

/*
 * Concept 'clock'.
 *
 * Type names:
 * - clock::time_point - represents time point from clock's epoch
 * - clock::duration - represents duration between time points
 *
 * Member functions:
 * - static time_point now() noexcept - gets current time point
 *
 * Notes:
 * - time_point and duration can have no connection to std::chrono
 *   but there should be std::chrono::duration_cast specialization for clock::duration
 */


class tsc_clock {
public:
	typedef int64_t time_point;
	typedef int64_t duration;

	static time_point now() noexcept;
	static std::chrono::nanoseconds to_nanoseconds(const duration&) noexcept;
	static duration from_nanoseconds(const std::chrono::nanoseconds&) noexcept;
};

template<class time_duration>
class steady_clock {
public:
	typedef time_duration duration;
	typedef typename duration::rep rep;
	typedef typename duration::period period;
	typedef typename std::chrono::time_point<steady_clock> time_point;
	static const bool is_steady = true;

	static time_point now() noexcept;
};


typedef std::chrono::microseconds default_duration;
typedef tsc_clock default_clock;
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

namespace std { namespace chrono {

template <typename ToDuration>
ToDuration duration_cast(handystats::chrono::tsc_clock::duration duration) {
	return duration_cast<ToDuration>(handystats::chrono::tsc_clock::to_nanoseconds(duration));
}

template <typename, typename FromDuration>
handystats::chrono::tsc_clock::duration duration_cast(FromDuration duration) {
	return handystats::chrono::tsc_clock::from_nanoseconds(std::chrono::duration_cast<std::chrono::nanoseconds>(duration));
}

}} // namespace std::chrono

#endif // HANDYSTATS_CHRONO_IMPL_H_
