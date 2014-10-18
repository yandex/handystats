// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <stdexcept>

#include <handystats/atomic.hpp>
#include <handystats/chrono.hpp>

namespace handystats { namespace chrono {

time_point::time_point()
	: m_since_epoch()
	, m_clock(clock_type::INTERNAL)
{}

time_point::time_point(const duration& d, const clock_type& clock)
	: m_since_epoch(d)
	, m_clock(clock)
{}

duration time_point::time_since_epoch() const {
	return m_since_epoch;
}

time_point& time_point::operator+=(const duration& d) {
	if (m_clock == clock_type::INTERNAL) {
		m_since_epoch += d;
	}
	else {
		if (d.m_unit == time_unit::CYCLE) {
			m_since_epoch += duration::convert_to(m_since_epoch.m_unit, d);
		}
		else {
			m_since_epoch += d;
		}
	}
	return *this;
}
time_point& time_point::operator-=(const duration& d) {
	if (m_clock == clock_type::INTERNAL) {
		m_since_epoch -= d;
	}
	else {
		if (d.m_unit == time_unit::CYCLE) {
			m_since_epoch -= duration::convert_to(m_since_epoch.m_unit, d);
		}
		else {
			m_since_epoch -= d;
		}
	}
	return *this;
}

time_point time_point::operator+(const duration& d) const {
	time_point ret(*this);
	ret += d;
	return ret;
}
time_point time_point::operator-(const duration& d) const {
	time_point ret(*this);
	ret -= d;
	return ret;
}

duration time_point::operator-(const time_point& t) const {
	if (m_clock == t.m_clock) {
		return m_since_epoch - t.m_since_epoch;
	}
	else {
		if (m_clock == clock_type::INTERNAL) {
			return convert_to(clock_type::SYSTEM, *this).m_since_epoch - t.m_since_epoch;
		}
		else {
			return m_since_epoch - convert_to(clock_type::SYSTEM, t).m_since_epoch;
		}
	}
}

bool time_point::operator==(const time_point& t) const {
	if (m_clock == t.m_clock) {
		return m_since_epoch == t.m_since_epoch;
	}
	else {
		if (m_clock == clock_type::INTERNAL) {
			return convert_to(clock_type::SYSTEM, *this) == t;
		}
		else {
			return *this == convert_to(clock_type::SYSTEM, t);
		}
	}
}
bool time_point::operator!=(const time_point& t) const {
	if (m_clock == t.m_clock) {
		return m_since_epoch != t.m_since_epoch;
	}
	else {
		if (m_clock == clock_type::INTERNAL) {
			return convert_to(clock_type::SYSTEM, *this) != t;
		}
		else {
			return *this != convert_to(clock_type::SYSTEM, t);
		}
	}
}
bool time_point::operator<(const time_point& t) const {
	if (m_clock == t.m_clock) {
		return m_since_epoch < t.m_since_epoch;
	}
	else {
		if (m_clock == clock_type::INTERNAL) {
			return convert_to(clock_type::SYSTEM, *this) < t;
		}
		else {
			return *this < convert_to(clock_type::SYSTEM, t);
		}
	}
}
bool time_point::operator<=(const time_point& t) const {
	if (m_clock == t.m_clock) {
		return m_since_epoch <= t.m_since_epoch;
	}
	else {
		if (m_clock == clock_type::INTERNAL) {
			return convert_to(clock_type::SYSTEM, *this) <= t;
		}
		else {
			return *this <= convert_to(clock_type::SYSTEM, t);
		}
	}
}
bool time_point::operator>(const time_point& t) const {
	if (m_clock == t.m_clock) {
		return m_since_epoch > t.m_since_epoch;
	}
	else {
		if (m_clock == clock_type::INTERNAL) {
			return convert_to(clock_type::SYSTEM, *this) > t;
		}
		else {
			return *this > convert_to(clock_type::SYSTEM, t);
		}
	}
}
bool time_point::operator>=(const time_point& t) const {
	if (m_clock == t.m_clock) {
		return m_since_epoch >= t.m_since_epoch;
	}
	else {
		if (m_clock == clock_type::INTERNAL) {
			return convert_to(clock_type::SYSTEM, *this) >= t;
		}
		else {
			return *this >= convert_to(clock_type::SYSTEM, t);
		}
	}
}

/* Conversion to system time */
static
time_point to_system_time(const time_point& t) {
	static std::atomic<int64_t> ns_offset(0);
	static std::atomic<int64_t> offset_timestamp(0);
	static std::atomic_flag lock = ATOMIC_FLAG_INIT;

	static const duration OFFSET_TIMEOUT (15 * (int64_t)1E9, time_unit::NSEC);
	static const duration CLOSE_DISTANCE (15 * (int64_t)1E3, time_unit::NSEC);
	static const uint64_t MAX_UPDATE_TRIES (100);

	time_point current_tsc_time = internal_clock::now();
	time_unit tsc_unit = current_tsc_time.time_since_epoch().unit();

	int64_t offset_ts = offset_timestamp.load(std::memory_order_acquire);

	if (offset_ts == 0 ||
			current_tsc_time.time_since_epoch() - duration(offset_ts, tsc_unit) > OFFSET_TIMEOUT
		)
	{
		if (!lock.test_and_set(std::memory_order_acquire)) {
			time_point cycles_start, cycles_end;
			time_point current_system_time;

			bool close_pair_found = false;
			for (uint64_t update_try = 0; update_try < MAX_UPDATE_TRIES; ++update_try) {
				cycles_start = internal_clock::now();
				current_system_time = system_clock::now();
				cycles_end = internal_clock::now();

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
		// to_clock == clock_type::INTERNAL
		throw std::logic_error("SYSTEM to INTERNAL clock conversion is not implemented");
	}
}

}} // namespace handystats::chrono
