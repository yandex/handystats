// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CHRONO_HPP_
#define HANDYSTATS_CHRONO_HPP_

#include <cstdint>
#include <cstdlib>
#include <chrono>

#include <handystats/chrono/tsc_clock.hpp>

namespace handystats { namespace chrono {

/*
 * Concept 'clock' (follows std::chrono::clock).
 *
 * Types:
 * - clock::time_point - represents time point from clock's epoch (follows std::chrono::time_point)
 * - clock::duration - represents duration between time points (follows std::chrono::duration)
 *
 * Member functions:
 * - static time_point now() noexcept
 *       Gets current time point
 *
 * Non-member functions:
 * - std::chrono::system_clock::time_point to_system_time(const clock::time_point&)
 *       Converts clock's time_point to system-wide clock's time represented by std::chrono::system_clock
 *
 * - duration_cast from and to clock::duration
 *   'clock' class should provide handystats::chrono::duration_cast specializations between clock::duration and std::chrono::nanoseconds
 *
 * Notes:
 * - time_point and duration can have no connection to std::chrono
 *   but overall public interface is aimed to follow std::chrono's one.
 *
 * - duration_cast in handystats::chrono namespace is not specialization for std::chrono::duration_cast
 *   but overload for clock::duration parameter.
 *
 *   Also, if handystats::chrono::duration_cast is called without clock::duration as parameter (from or to duration)
 *   call falls back to std::chrono::duration_cast.
 */

/*
 * Library-wide internal clock
 */
typedef tsc_clock clock;

/*
 * Library-wide time duration.
 * Also determines interval time measurement precision
 */
typedef std::chrono::microseconds time_duration;


// Convert from handystats internal clock's time_point to standard system_clock's time_point
std::chrono::system_clock::time_point to_system_time(const clock::time_point&);


// Fall back to std::chrono::duration_cast if not from and not to clock::duration
template <typename ToDuration, typename FromDuration,
			typename std::enable_if<
						!std::is_same<ToDuration, clock::duration>::value &&
						!std::is_same<FromDuration, clock::duration>::value,
					int>::type = 0
		>
ToDuration duration_cast(const FromDuration& d) {
	return std::chrono::duration_cast<ToDuration>(d);
}

// Cast from clock::duration through std::chrono::nanoseconds
template <typename ToDuration, typename FromDuration,
			typename std::enable_if<
						!std::is_same<ToDuration, clock::duration>::value &&
						std::is_same<FromDuration, clock::duration>::value,
					int>::type = 0
		>
ToDuration duration_cast(const FromDuration& d) {
	return std::chrono::duration_cast<ToDuration>(duration_cast<std::chrono::nanoseconds>(d));
}

// Cast to clock::duration through std::chrono::nanoseconds
template <typename ToDuration, typename FromDuration,
			typename std::enable_if<
						std::is_same<ToDuration, clock::duration>::value &&
						!std::is_same<FromDuration, clock::duration>::value,
					int>::type = 0
		>
ToDuration duration_cast(const FromDuration& d) {
	return duration_cast<clock::duration>(std::chrono::duration_cast<std::chrono::nanoseconds>(d));
}

// Trivial cast with the same clock::duration
template <typename ToDuration, typename FromDuration,
			typename std::enable_if<
						std::is_same<ToDuration, clock::duration>::value &&
						std::is_same<FromDuration, clock::duration>::value,
					int>::type = 0
		>
FromDuration duration_cast(const FromDuration& d) {
	return d;
}


// duration_cast specializations through std::chrono::nanoseconds that should support clock::duration
template <>
clock::duration duration_cast<clock::duration, std::chrono::nanoseconds>(const std::chrono::nanoseconds&);

template <>
std::chrono::nanoseconds duration_cast<std::chrono::nanoseconds, clock::duration>(const clock::duration&);

}} // namespace handystats::chrono

#endif // HANDYSTATS_CHRONO_HPP_
