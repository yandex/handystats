// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/atomic.hpp>
#include <stdexcept>
#include <ctime>

#include <handystats/chrono.h>
#include <handystats/chrono.hpp>

#include "chrono/tsc_impl.hpp"

namespace handystats { namespace chrono {

static
inline uint64_t nsec_factor(const time_unit& unit) {
	switch (unit) {
	case time_unit::NSEC:
		return 1ull;
	case time_unit::USEC:
		return 1000ull;
	case time_unit::MSEC:
		return 1000ull * 1000ull;
	case time_unit::SEC:
		return 1000ull * 1000ull * 1000ull;
	case time_unit::MIN:
		return 60ull * 1000ull * 1000ull * 1000ull;
	case time_unit::HOUR:
		return 60ull * 60ull * 1000ull * 1000ull * 1000ull;
	case time_unit::DAY:
		return 24ull * 60ull * 60ull * 1000ull * 1000ull * 1000ull;
	}

	return 0ull;
}

duration duration::convert_to(const time_unit& to_unit, const duration& d) {
	if (d.m_unit == to_unit) return d;

	if (to_unit == time_unit::TICK) {
		return duration(int64_t(cycles_per_nanosec * nsec_factor(d.m_unit) * d.m_rep), to_unit);
	}

	if (d.m_unit == time_unit::TICK) {
		return duration(int64_t(double(d.m_rep) / nsec_factor(to_unit) / cycles_per_nanosec), to_unit);
	}

	return duration(nsec_factor(d.m_unit) * d.m_rep / nsec_factor(to_unit), to_unit);
}

/* Conversion to system time */
static
time_point to_system_time(const time_point& t) {
	static std::atomic<int64_t> ns_offset(0);
	static std::atomic<int64_t> offset_timestamp(0);
	static std::atomic_flag lock = ATOMIC_FLAG_INIT;

	static const duration OFFSET_TIMEOUT (15 * (int64_t)1E9, time_unit::TICK);
	static const duration CLOSE_DISTANCE (15 * (int64_t)1E3, time_unit::TICK);
	static const uint64_t MAX_UPDATE_TRIES (100);

	time_point current_tsc_time = tsc_clock::now();

	int64_t offset_ts = offset_timestamp.load(std::memory_order_acquire);

	if (offset_ts == 0 ||
			current_tsc_time.time_since_epoch() - duration(offset_ts, time_unit::TICK) > OFFSET_TIMEOUT)
	{
		if (!lock.test_and_set(std::memory_order_acquire)) {
			time_point cycles_start, cycles_end;
			time_point current_system_time;

			bool close_pair_found = false;
			for (uint64_t update_try = 0; update_try < MAX_UPDATE_TRIES; ++update_try) {
				cycles_start = tsc_clock::now();
				current_system_time = system_clock::now();
				cycles_end = tsc_clock::now();

				if (cycles_end - cycles_start < CLOSE_DISTANCE) {
					close_pair_found = true;
					break;
				}
			}

			if (close_pair_found) {
				time_point cycles_middle = cycles_start + (cycles_end - cycles_start) / 2;
				int64_t new_offset =
					duration::convert_to(
						time_unit::NSEC,
						current_system_time.time_since_epoch() - cycles_middle.time_since_epoch()
					)
					.count();

				ns_offset.store(new_offset, std::memory_order_release);
				offset_timestamp.store(cycles_middle.time_since_epoch().count(), std::memory_order_release);
			}

			lock.clear(std::memory_order_release);
		}
	}

	return
		time_point(
			duration::convert_to(
				time_unit::NSEC,
				t.time_since_epoch() + duration(ns_offset.load(std::memory_order_acquire), time_unit::NSEC)
			),
			clock_type::SYSTEM
		);
}

time_point time_point::convert_to(const clock_type& to_clock, const time_point& t) {
	if (t.m_clock == to_clock) return t;

	if (to_clock == clock_type::SYSTEM) {
		return to_system_time(t);
	}
	else {
		// to_clock == clock_type::TSC
		throw std::logic_error("SYSTEM to TSC clock conversion is not implemented");
	}
}

time_point tsc_clock::now() {
	return time_point(duration(rdtsc(), time_unit::TICK), clock_type::TSC);
}

time_point system_clock::now() {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	return time_point(duration(ts.tv_sec * 1000ull * 1000ull * 1000ull + ts.tv_nsec, time_unit::NSEC), clock_type::SYSTEM);
}

}} // namespace handystats::chrono

HANDYSTATS_EXTERN_C
int64_t handystats_now(void) {
	return (int64_t)handystats::chrono::rdtsc();
}

HANDYSTATS_EXTERN_C
double handystats_difftime(int64_t end, int64_t start) {
	return double(end - start) / handystats::chrono::cycles_per_nanosec / 1E9;
}
