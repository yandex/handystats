// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CHRONO_HPP_
#define HANDYSTATS_CHRONO_HPP_

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <algorithm>

namespace handystats { namespace chrono {

enum class time_unit {
	TICK,
	NSEC,
	USEC,
	MSEC,
	SEC,
	MIN,
	HOUR,
	DAY
};

enum class clock_type {
	TSC, // epoch - unspecified point in time (usually boot-time), machine-specific
	SYSTEM // epoch - 1970 00:00:00 UT
};

}} // namespace handystats::chrono


namespace std {

template <>
struct less<handystats::chrono::time_unit> {
	bool operator() (const handystats::chrono::time_unit& x, const handystats::chrono::time_unit& y) {
		return precision_class(x) < precision_class(y);
	}

private:
	static unsigned precision_class(const handystats::chrono::time_unit& unit) {
		switch (unit) {
		case handystats::chrono::time_unit::TICK:
			return 1 << 0;
		case handystats::chrono::time_unit::NSEC:
			return 1 << 1;
		case handystats::chrono::time_unit::USEC:
			return 1 << 2;
		case handystats::chrono::time_unit::MSEC:
			return 1 << 3;
		case handystats::chrono::time_unit::SEC:
			return 1 << 4;
		case handystats::chrono::time_unit::MIN:
			return 1 << 5;
		case handystats::chrono::time_unit::HOUR:
			return 1 << 6;
		case handystats::chrono::time_unit::DAY:
			return 1 << 7;
		}

		return 0;
	}
};

} // namespace std


namespace handystats { namespace chrono {

struct duration;
struct time_point;

struct tsc_clock {
	// will return time_point with TSC clock type and TICK time unit
	static time_point now();
};

struct system_clock {
	// will return time_point with SYSTEM clock type and NSEC time unit
	static time_point now();
};

struct duration {
	static duration convert_to(const time_unit&, const duration&);

	duration()
		: m_rep(0)
		, m_unit(time_unit::TICK)
	{}

	duration(const int64_t& d, const time_unit& u)
		: m_rep(d)
		, m_unit(u)
	{}

	duration(const duration&) = default;

	/* Assign operator */
	duration& operator= (const duration&) = default;

	/* Access ticks count and time unit */
	int64_t count() const {
		return m_rep;
	}
	time_unit unit() const {
		return m_unit;
	}

	/* Unary operators */
	duration operator+() const {
		return *this;
	}
	duration operator-() const {
		return duration(-m_rep, m_unit);
	}

	/* Increment/decrement operators */
	duration& operator++() {
		++m_rep;
		return *this;
	}
	duration operator++(int) {
		return duration(m_rep++, m_unit);
	}
	duration& operator--() {
		--m_rep;
		return *this;
	}
	duration operator--(int) {
		return duration(m_rep--, m_unit);
	}

	/* Compound assignments */
	duration& operator+=(const duration& d) {
		if (m_unit == d.m_unit) {
			m_rep += d.m_rep;
		}
		else {
			m_rep += convert_to(m_unit, d).m_rep;
		}
		return *this;
	}
	duration& operator-=(const duration& d) {
		if (m_unit == d.m_unit) {
			m_rep -= d.m_rep;
		}
		else {
			m_rep -= convert_to(m_unit, d).m_rep;
		}
		return *this;
	}
	duration& operator%=(const duration& d) {
		if (m_unit == d.m_unit) {
			m_rep %= d.m_rep;
		}
		else {
			m_rep %= convert_to(m_unit, d).m_rep;
		}
		return *this;
	}

	duration& operator+=(const int64_t& d) {
		m_rep += d;
		return *this;
	}
	duration& operator-=(const int64_t& d) {
		m_rep -= d;
		return *this;
	}
	duration& operator*=(const int64_t& d) {
		m_rep *= d;
		return *this;
	}
	duration& operator/=(const int64_t& d) {
		m_rep /= d;
		return *this;
	}
	duration& operator%=(const int64_t& d) {
		m_rep %= d;
		return *this;
	}

	/* Arithmetic operations with duration */
	duration operator+(const duration& d) const {
		if (m_unit == d.m_unit) {
			return duration(m_rep + d.m_rep, m_unit);
		}
		else {
			const auto& common_unit = std::min(m_unit, d.m_unit, std::less<time_unit>());
			return duration(convert_to(common_unit, *this).m_rep + convert_to(common_unit, d).m_rep, common_unit);
		}
	}
	duration operator-(const duration& d) const {
		if (m_unit == d.m_unit) {
			return duration(m_rep - d.m_rep, m_unit);
		}
		else {
			const auto& common_unit = std::min(m_unit, d.m_unit, std::less<time_unit>());
			return duration(convert_to(common_unit, *this).m_rep - convert_to(common_unit, d).m_rep, common_unit);
		}
	}

	duration operator+(const int64_t& d) const {
		return duration(m_rep + d, m_unit);
	}
	duration operator-(const int64_t& d) const {
		return duration(m_rep - d, m_unit);
	}
	duration operator*(const int64_t& d) const {
		return duration(m_rep * d, m_unit);
	}
	duration operator/(const int64_t& d) const {
		return duration(m_rep / d, m_unit);
	}
	duration operator%(const int64_t& d) const {
		return duration(m_rep % d, m_unit);
	}

	/* Comparison operators */
	bool operator==(const duration& d) const {
		if (m_unit == d.m_unit) {
			return m_rep == d.m_rep;
		}
		else {
			const auto& common_unit = std::min(m_unit, d.m_unit, std::less<time_unit>());
			return convert_to(common_unit, *this).m_rep == convert_to(common_unit, d).m_rep;
		}
	}
	bool operator!=(const duration& d) const {
		if (m_unit == d.m_unit) {
			return m_rep != d.m_rep;
		}
		else {
			const auto& common_unit = std::min(m_unit, d.m_unit, std::less<time_unit>());
			return convert_to(common_unit, *this).m_rep != convert_to(common_unit, d).m_rep;
		}
	}
	bool operator<(const duration& d) const {
		if (m_unit == d.m_unit) {
			return m_rep < d.m_rep;
		}
		else {
			const auto& common_unit = std::min(m_unit, d.m_unit, std::less<time_unit>());
			return convert_to(common_unit, *this).m_rep < convert_to(common_unit, d).m_rep;
		}
	}
	bool operator<=(const duration& d) const {
		if (m_unit == d.m_unit) {
			return m_rep <= d.m_rep;
		}
		else {
			const auto& common_unit = std::min(m_unit, d.m_unit, std::less<time_unit>());
			return convert_to(common_unit, *this).m_rep <= convert_to(common_unit, d).m_rep;
		}
	}
	bool operator>(const duration& d) const {
		if (m_unit == d.m_unit) {
			return m_rep > d.m_rep;
		}
		else {
			const auto& common_unit = std::min(m_unit, d.m_unit, std::less<time_unit>());
			return convert_to(common_unit, *this).m_rep > convert_to(common_unit, d).m_rep;
		}
	}
	bool operator>=(const duration& d) const {
		if (m_unit == d.m_unit) {
			return m_rep >= d.m_rep;
		}
		else {
			const auto& common_unit = std::min(m_unit, d.m_unit, std::less<time_unit>());
			return convert_to(common_unit, *this).m_rep >= convert_to(common_unit, d).m_rep;
		}
	}

	friend struct time_point;

private:
	int64_t m_rep;
	time_unit m_unit;
};

struct time_point {
	// if clock types are the same nothing will be done
	// if target clock type is TSC resulted duration will be TICK
	// if target clock type is SYSTEM resulted duration will be NSEC
	static time_point convert_to(const clock_type&, const time_point&);

	time_point()
		: m_since_epoch()
		, m_clock(clock_type::TSC)
	{}

	time_point(const duration& d, const clock_type& clock)
		: m_since_epoch(d)
		, m_clock(clock)
	{}

	duration time_since_epoch() const {
		return m_since_epoch;
	}

	/* Compound assignments */
	time_point& operator+=(const duration& d) {
		if (m_clock == clock_type::TSC) {
			m_since_epoch += d;
		}
		else {
			if (d.m_unit == time_unit::TICK) {
				m_since_epoch += duration::convert_to(m_since_epoch.m_unit, d);
			}
			else {
				m_since_epoch += d;
			}
		}
		return *this;
	}
	time_point& operator-=(const duration& d) {
		if (m_clock == clock_type::TSC) {
			m_since_epoch -= d;
		}
		else {
			if (d.m_unit == time_unit::TICK) {
				m_since_epoch -= duration::convert_to(m_since_epoch.m_unit, d);
			}
			else {
				m_since_epoch -= d;
			}
		}
		return *this;
	}

	/* Arithmetic operations with duration */
	time_point operator+(const duration& d) const {
		time_point ret(*this);
		ret += d;
		return ret;
	}
	time_point operator-(const duration& d) const {
		time_point ret(*this);
		ret -= d;
		return ret;
	}

	/* Arithmetic operations with time_point */
	duration operator-(const time_point& t) const {
		if (m_clock == t.m_clock) {
			return m_since_epoch - t.m_since_epoch;
		}
		else {
			if (m_clock == clock_type::TSC) {
				return convert_to(clock_type::SYSTEM, *this).m_since_epoch - t.m_since_epoch;
			}
			else {
				return m_since_epoch - convert_to(clock_type::SYSTEM, t).m_since_epoch;
			}
		}
	}

	/* Comparison operators */
	bool operator==(const time_point& t) const {
		if (m_clock == t.m_clock) {
			return m_since_epoch == t.m_since_epoch;
		}
		else {
			if (m_clock == clock_type::TSC) {
				return convert_to(clock_type::SYSTEM, *this) == t;
			}
			else {
				return *this == convert_to(clock_type::SYSTEM, t);
			}
		}
	}
	bool operator!=(const time_point& t) const {
		if (m_clock == t.m_clock) {
			return m_since_epoch != t.m_since_epoch;
		}
		else {
			if (m_clock == clock_type::TSC) {
				return convert_to(clock_type::SYSTEM, *this) != t;
			}
			else {
				return *this != convert_to(clock_type::SYSTEM, t);
			}
		}
	}
	bool operator<(const time_point& t) const {
		if (m_clock == t.m_clock) {
			return m_since_epoch < t.m_since_epoch;
		}
		else {
			if (m_clock == clock_type::TSC) {
				return convert_to(clock_type::SYSTEM, *this) < t;
			}
			else {
				return *this < convert_to(clock_type::SYSTEM, t);
			}
		}
	}
	bool operator<=(const time_point& t) const {
		if (m_clock == t.m_clock) {
			return m_since_epoch <= t.m_since_epoch;
		}
		else {
			if (m_clock == clock_type::TSC) {
				return convert_to(clock_type::SYSTEM, *this) <= t;
			}
			else {
				return *this <= convert_to(clock_type::SYSTEM, t);
			}
		}
	}
	bool operator>(const time_point& t) const {
		if (m_clock == t.m_clock) {
			return m_since_epoch > t.m_since_epoch;
		}
		else {
			if (m_clock == clock_type::TSC) {
				return convert_to(clock_type::SYSTEM, *this) > t;
			}
			else {
				return *this > convert_to(clock_type::SYSTEM, t);
			}
		}
	}
	bool operator>=(const time_point& t) const {
		if (m_clock == t.m_clock) {
			return m_since_epoch >= t.m_since_epoch;
		}
		else {
			if (m_clock == clock_type::TSC) {
				return convert_to(clock_type::SYSTEM, *this) >= t;
			}
			else {
				return *this >= convert_to(clock_type::SYSTEM, t);
			}
		}
	}

private:
	duration m_since_epoch;
	clock_type m_clock;
};

}} // namespace handystats::chrono

#endif // HANDYSTATS_CHRONO_HPP_
