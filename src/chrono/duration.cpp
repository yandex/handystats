/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <handystats/chrono.hpp>

namespace handystats { namespace chrono {

struct time_unit_less {
	bool operator() (const handystats::chrono::time_unit& x, const handystats::chrono::time_unit& y) {
		return precision_class(x) < precision_class(y);
	}

private:
	static
	unsigned precision_class(const handystats::chrono::time_unit& unit) {
		switch (unit) {
		case handystats::chrono::time_unit::CYCLE:
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

extern long double cycles_per_nanosec;

duration::duration()
	: m_rep(0)
	, m_unit(time_unit::NSEC)
{}

duration::duration(const int64_t& d, const time_unit& u)
	: m_rep(d)
	, m_unit(u)
{}

int64_t duration::count() const {
	return m_rep;
}
time_unit duration::unit() const {
	return m_unit;
}

int64_t duration::count(const time_unit& unit) const {
	if (m_unit == unit) return m_rep;

	if (unit == time_unit::CYCLE) {
		return int64_t(cycles_per_nanosec * nsec_factor(m_unit) * m_rep);
	}

	if (m_unit == time_unit::CYCLE) {
		return int64_t(double(m_rep) / nsec_factor(unit) / cycles_per_nanosec);
	}

	return nsec_factor(m_unit) * m_rep / nsec_factor(unit);
}

duration duration::operator+() const {
	return *this;
}
duration duration::operator-() const {
	return duration(-m_rep, m_unit);
}

duration& duration::operator++() {
	++m_rep;
	return *this;
}
duration duration::operator++(int) {
	return duration(m_rep++, m_unit);
}
duration& duration::operator--() {
	--m_rep;
	return *this;
}
duration duration::operator--(int) {
	return duration(m_rep--, m_unit);
}

duration& duration::operator+=(const duration& d) {
	if (m_unit == d.m_unit) {
		m_rep += d.m_rep;
	}
	else {
		m_rep += d.count(m_unit);
	}
	return *this;
}
duration& duration::operator-=(const duration& d) {
	if (m_unit == d.m_unit) {
		m_rep -= d.m_rep;
	}
	else {
		m_rep -= d.count(m_unit);
	}
	return *this;
}
duration& duration::operator%=(const duration& d) {
	if (m_unit == d.m_unit) {
		m_rep %= d.m_rep;
	}
	else {
		m_rep %= d.count(m_unit);
	}
	return *this;
}

duration& duration::operator+=(const int64_t& d) {
	m_rep += d;
	return *this;
}
duration& duration::operator-=(const int64_t& d) {
	m_rep -= d;
	return *this;
}
duration& duration::operator*=(const int64_t& d) {
	m_rep *= d;
	return *this;
}
duration& duration::operator/=(const int64_t& d) {
	m_rep /= d;
	return *this;
}
duration& duration::operator%=(const int64_t& d) {
	m_rep %= d;
	return *this;
}

duration duration::operator+(const duration& d) const {
	if (m_unit == d.m_unit) {
		return duration(m_rep + d.m_rep, m_unit);
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return duration(count(common_unit) + d.count(common_unit), common_unit);
	}
}
duration duration::operator-(const duration& d) const {
	if (m_unit == d.m_unit) {
		return duration(m_rep - d.m_rep, m_unit);
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return duration(count(common_unit) - d.count(common_unit), common_unit);
	}
}

duration duration::operator+(const int64_t& d) const {
	return duration(m_rep + d, m_unit);
}
duration duration::operator-(const int64_t& d) const {
	return duration(m_rep - d, m_unit);
}
duration duration::operator*(const int64_t& d) const {
	return duration(m_rep * d, m_unit);
}
duration duration::operator/(const int64_t& d) const {
	return duration(m_rep / d, m_unit);
}
duration duration::operator%(const int64_t& d) const {
	return duration(m_rep % d, m_unit);
}

bool duration::operator==(const duration& d) const {
	if (m_unit == d.m_unit) {
		return m_rep == d.m_rep;
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return count(common_unit) == d.count(common_unit);
	}
}
bool duration::operator!=(const duration& d) const {
	if (m_unit == d.m_unit) {
		return m_rep != d.m_rep;
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return count(common_unit) != d.count(common_unit);
	}
}
bool duration::operator<(const duration& d) const {
	if (m_unit == d.m_unit) {
		return m_rep < d.m_rep;
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return count(common_unit) < d.count(common_unit);
	}
}
bool duration::operator<=(const duration& d) const {
	if (m_unit == d.m_unit) {
		return m_rep <= d.m_rep;
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return count(common_unit) <= d.count(common_unit);
	}
}
bool duration::operator>(const duration& d) const {
	if (m_unit == d.m_unit) {
		return m_rep > d.m_rep;
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return count(common_unit) > d.count(common_unit);
	}
}
bool duration::operator>=(const duration& d) const {
	if (m_unit == d.m_unit) {
		return m_rep >= d.m_rep;
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return count(common_unit) >= d.count(common_unit);
	}
}

duration duration::convert_to(const time_unit& to_unit, const duration& d) {
	return duration(d.count(to_unit), to_unit);
}

// duration helpers
duration nanoseconds(const int64_t& d) {
	return duration(d, time_unit::NSEC);
}

duration microseconds(const int64_t& d) {
	return duration(d, time_unit::USEC);
}

duration milliseconds(const int64_t& d) {
	return duration(d, time_unit::MSEC);
}

duration seconds(const int64_t& d) {
	return duration(d, time_unit::SEC);
}

duration minutes(const int64_t& d) {
	return duration(d, time_unit::MIN);
}

duration hours(const int64_t& d) {
	return duration(d, time_unit::HOUR);
}

duration days(const int64_t& d) {
	return duration(d, time_unit::DAY);
}

}} // namespace handystats::chrono
