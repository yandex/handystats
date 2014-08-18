// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CHRONO_TSC_CLOCK_HPP_
#define HANDYSTATS_CHRONO_TSC_CLOCK_HPP_

#include <cstdint>

#include <chrono>

#include <handystats/common.h>

namespace handystats { namespace chrono {

// Follows 'Clock' concept
class tsc_clock {
public:

	/*
	 * Duration
	 */
	class duration {
	public:
		typedef int64_t rep;

		/* Constructors */
		duration()
			: tick_count()
		{
		}

		duration(const duration& d)
			: tick_count(d.tick_count)
		{
		}

		template <class Rep2>
		explicit duration(const Rep2& r) {
			static_assert(!std::chrono::treat_as_floating_point<Rep2>::value, "Only integral types are accepted for tsc_clock::duration construction");
			tick_count = r;
		}

		/* Assign operator */
		duration& operator=(const duration& d) {
			tick_count = d.tick_count;
			return *this;
		}

		/* Get duration's tick count */
		rep count() const;

		/* Special duration values */
		static duration zero();
		static duration min();
		static duration max();

		/* Unary operators */
		duration operator+() const;
		duration operator-() const;

		/* Increment/decrement operators */
		duration& operator++();
		duration operator++(int);
		duration& operator--();
		duration operator--(int);

		/* Compound assignments */
		duration& operator+=(const duration&);
		duration& operator-=(const duration&);
		duration& operator*=(const rep&);
		duration& operator/=(const rep&);
		duration& operator%=(const rep&);
		duration& operator%=(const duration&);

		/* Arithmetic operations with duration */
		duration operator+(const duration&) const;
		duration operator-(const duration&) const;
		duration operator*(const rep&) const;
		duration operator/(const rep&) const;
		duration operator%(const rep&) const;

		/* Comparison operators */
		bool operator==(const duration&) const;
		bool operator!=(const duration&) const;
		bool operator<(const duration&) const;
		bool operator<=(const duration&) const;
		bool operator>(const duration&) const;
		bool operator>=(const duration&) const;

	private:
		rep tick_count;
	};

	/*
	 * Time Point
	 */
	class time_point {
	public:
		typedef tsc_clock clock;
		typedef clock::duration duration;
		typedef duration::rep rep;

		/* Constructors */
		time_point()
			: since_epoch()
		{
		}

		explicit time_point(const duration&);

		/* Special time_point values */
		static time_point min();
		static time_point max();

		/* Get duration between time_point and clock's epoch */
		duration time_since_epoch() const;

		/* Compound assignments */
		time_point& operator+=(const duration&);
		time_point& operator-=(const duration&);

		/* Arithmetic operations with duration */
		time_point operator+(const duration&) const;
		time_point operator-(const duration&) const;

		/* Arithmetic operations with time_point */
		duration operator-(const time_point&) const;

		/* Comparison operators */
		bool operator==(const time_point&) const;
		bool operator!=(const time_point&) const;
		bool operator<(const time_point&) const;
		bool operator<=(const time_point&) const;
		bool operator>(const time_point&) const;
		bool operator>=(const time_point&) const;

	private:
		duration since_epoch;
	};

	static time_point now() HANDYSTATS_NOEXCEPT;
};


// Convert from tsc_clock::time_point to std::chrono::system_clock::time_point
std::chrono::system_clock::time_point to_system_time(const tsc_clock::time_point&);

}} // namespace handystats::chrono

#endif // HANDYSTATS_CHRONO_TSC_CLOCK_HPP_
