// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/atomic.hpp>
#include <type_traits>

#include <handystats/common.h>

#include <handystats/chrono.hpp>
#include <handystats/chrono/tsc_clock.hpp>

#include "chrono/tsc_impl.hpp"

namespace handystats { namespace chrono {

////////////////////////
// tsc_clock::duration
////////////////////////

/* Get duration's tick count */

tsc_clock::duration::rep tsc_clock::duration::count() const {
	return tick_count;
}

/* Special duration values */

tsc_clock::duration tsc_clock::duration::zero() {
	return duration(std::chrono::duration_values<rep>::zero());
}

tsc_clock::duration tsc_clock::duration::min() {
	return duration(std::chrono::duration_values<rep>::min());
}

tsc_clock::duration tsc_clock::duration::max() {
	return duration(std::chrono::duration_values<rep>::max());
}

/* Unary operators */

tsc_clock::duration tsc_clock::duration::operator+() const {
	return *this;
}

tsc_clock::duration tsc_clock::duration::operator-() const {
	return duration(-count());
}

/* Increment/decrement operators */

tsc_clock::duration& tsc_clock::duration::operator++() {
	++tick_count;
	return *this;
}

tsc_clock::duration tsc_clock::duration::operator++(int) {
	return duration(tick_count++);
}

tsc_clock::duration& tsc_clock::duration::operator--() {
	--tick_count;
	return *this;
}

tsc_clock::duration tsc_clock::duration::operator--(int) {
	return duration(tick_count--);
}

/* Compund assignments */

tsc_clock::duration& tsc_clock::duration::operator+=(const duration& d) {
	tick_count += d.count();
	return *this;
}

tsc_clock::duration& tsc_clock::duration::operator-=(const duration& d) {
	tick_count -= d.count();
	return *this;
}

tsc_clock::duration& tsc_clock::duration::operator*=(const rep& r) {
	tick_count *= r;
	return *this;
}

tsc_clock::duration& tsc_clock::duration::operator/=(const rep& r) {
	tick_count /= r;
	return *this;
}

tsc_clock::duration& tsc_clock::duration::operator%=(const rep& r) {
	tick_count %= r;
	return *this;
}

tsc_clock::duration& tsc_clock::duration::operator%=(const duration& d) {
	tick_count %= d.count();
	return *this;
}

/* Arithmetic operations with duration */

tsc_clock::duration tsc_clock::duration::operator+(const duration& d) const {
	return duration(count() + d.count());
}

tsc_clock::duration tsc_clock::duration::operator-(const duration& d) const {
	return duration(count() - d.count());
}

tsc_clock::duration tsc_clock::duration::operator*(const rep& r) const {
	return duration(count() * r);
}

tsc_clock::duration tsc_clock::duration::operator/(const rep& r) const {
	return duration(count() / r);
}

tsc_clock::duration tsc_clock::duration::operator%(const rep& r) const {
	return duration(count() % r);
}

/* Comparison operators */

bool tsc_clock::duration::operator==(const duration& d) const {
	return count() == d.count();
}

bool tsc_clock::duration::operator!=(const duration& d) const {
	return count() != d.count();
}

bool tsc_clock::duration::operator<(const duration& d) const {
	return count() < d.count();
}

bool tsc_clock::duration::operator<=(const duration& d) const {
	return count() <= d.count();
}

bool tsc_clock::duration::operator>(const duration& d) const {
	return count() > d.count();
}

bool tsc_clock::duration::operator>=(const duration& d) const {
	return count() >= d.count();
}


//////////////////////////
// tsc_clock::time_point
//////////////////////////

/* Constructors */

tsc_clock::time_point::time_point(const duration& d) {
	since_epoch = d;
}

/* Special time_point values */

tsc_clock::time_point tsc_clock::time_point::min() {
	return time_point(duration::min());
}

tsc_clock::time_point tsc_clock::time_point::max() {
	return time_point(duration::max());
}

/* Get duration between time_point and clock's epoch */

tsc_clock::time_point::duration tsc_clock::time_point::time_since_epoch() const {
	return since_epoch;
}

/* Compound assignments */

tsc_clock::time_point& tsc_clock::time_point::operator+=(const duration& d) {
	since_epoch += d;
	return *this;
}

tsc_clock::time_point& tsc_clock::time_point::operator-=(const duration& d) {
	since_epoch -= d;
	return *this;
}

/* Arithmetic operations with duration */

tsc_clock::time_point tsc_clock::time_point::operator+(const duration& d) const {
	time_point result(*this);
	result += d;
	return result;
}

tsc_clock::time_point tsc_clock::time_point::operator-(const duration& d) const {
	time_point result(*this);
	result -= d;
	return result;
}

/* Arithmetic operations with duration */

tsc_clock::duration tsc_clock::time_point::operator-(const time_point& t) const {
	return time_since_epoch() - t.time_since_epoch();
}

/* Comparison operators */

bool tsc_clock::time_point::operator==(const time_point& t) const {
	return time_since_epoch() == t.time_since_epoch();
}

bool tsc_clock::time_point::operator!=(const time_point& t) const {
	return time_since_epoch() != t.time_since_epoch();
}

bool tsc_clock::time_point::operator<(const time_point& t) const {
	return time_since_epoch() < t.time_since_epoch();
}

bool tsc_clock::time_point::operator<=(const time_point& t) const {
	return time_since_epoch() <= t.time_since_epoch();
}

bool tsc_clock::time_point::operator>(const time_point& t) const {
	return time_since_epoch() > t.time_since_epoch();
}

bool tsc_clock::time_point::operator>=(const time_point& t) const {
	return time_since_epoch() > t.time_since_epoch();
}


//////////////
// tsc_clock
//////////////

tsc_clock::time_point tsc_clock::now() HANDYSTATS_NOEXCEPT {
	return
		time_point(
			duration(
				duration::rep(rdtsc())
			)
		);
}


/* Conversion to system time */
std::chrono::system_clock::time_point to_system_time(const tsc_clock::time_point& t) {
	static std::atomic<int64_t> ns_offset(0);
	static std::atomic<tsc_clock::time_point::rep> offset_timestamp(0);
	static std::atomic_flag lock = ATOMIC_FLAG_INIT;

	static const tsc_clock::duration OFFSET_TIMEOUT (15 * (int64_t)1E9);
	static const tsc_clock::duration CLOSE_DISTANCE (15 * (int64_t)1E3);
	static const uint64_t MAX_UPDATE_TRIES (100);

	tsc_clock::time_point current_tsc_time = tsc_clock::now();

	tsc_clock::time_point::rep offset_ts = offset_timestamp.load(std::memory_order_acquire);

	if (offset_ts == 0 || (current_tsc_time.time_since_epoch() - tsc_clock::duration(offset_ts)) > OFFSET_TIMEOUT) {
		if (!lock.test_and_set(std::memory_order_acquire)) {
			tsc_clock::time_point cycles_start, cycles_end;
			std::chrono::system_clock::time_point current_time;

			bool close_pair_found = false;
			for (uint64_t update_try = 0; update_try < MAX_UPDATE_TRIES; ++update_try) {
				cycles_start = tsc_clock::now();
				current_time = std::chrono::system_clock::now();
				cycles_end = tsc_clock::now();

				if (cycles_end - cycles_start < CLOSE_DISTANCE) {
					close_pair_found = true;
					break;
				}
			}

			if (close_pair_found) {
				tsc_clock::time_point cycles_middle = cycles_start + (cycles_end - cycles_start) / 2;
				int64_t new_offset =
					std::chrono::nanoseconds(
							duration_cast<std::chrono::nanoseconds>(current_time.time_since_epoch()) -
							duration_cast<std::chrono::nanoseconds>(cycles_middle.time_since_epoch())
							)
					.count();

				ns_offset.store(new_offset, std::memory_order_release);
				offset_timestamp.store(cycles_middle.time_since_epoch().count(), std::memory_order_release);
			}

			lock.clear(std::memory_order_release);
		}
	}

	return std::chrono::system_clock::time_point(
			duration_cast<std::chrono::system_clock::duration>(t.time_since_epoch()) +
			std::chrono::system_clock::duration(ns_offset.load(std::memory_order_acquire))
		);
}


/*
 * Duration cast specializations for tsc_clock::duration and std::chrono::nanoseconds
 */
template <>
std::chrono::nanoseconds duration_cast<std::chrono::nanoseconds, tsc_clock::duration>(const tsc_clock::duration& d) {
	return std::chrono::nanoseconds(
			std::chrono::nanoseconds::rep(d.count() / cycles_per_nanosec)
			);
}

template <>
tsc_clock::duration duration_cast<tsc_clock::duration, std::chrono::nanoseconds>(const std::chrono::nanoseconds& d) {
	return tsc_clock::duration(
			tsc_clock::duration::rep(d.count() * cycles_per_nanosec)
			);
}

}} // namespace handystats::chrono
