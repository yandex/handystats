// Copyright (c) 2014 Yandex LLC. All rights reserved.

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
		m_rep += convert_to(m_unit, d).m_rep;
	}
	return *this;
}
duration& duration::operator-=(const duration& d) {
	if (m_unit == d.m_unit) {
		m_rep -= d.m_rep;
	}
	else {
		m_rep -= convert_to(m_unit, d).m_rep;
	}
	return *this;
}
duration& duration::operator%=(const duration& d) {
	if (m_unit == d.m_unit) {
		m_rep %= d.m_rep;
	}
	else {
		m_rep %= convert_to(m_unit, d).m_rep;
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
		return duration(convert_to(common_unit, *this).m_rep + convert_to(common_unit, d).m_rep, common_unit);
	}
}
duration duration::operator-(const duration& d) const {
	if (m_unit == d.m_unit) {
		return duration(m_rep - d.m_rep, m_unit);
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return duration(convert_to(common_unit, *this).m_rep - convert_to(common_unit, d).m_rep, common_unit);
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
		return convert_to(common_unit, *this).m_rep == convert_to(common_unit, d).m_rep;
	}
}
bool duration::operator!=(const duration& d) const {
	if (m_unit == d.m_unit) {
		return m_rep != d.m_rep;
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return convert_to(common_unit, *this).m_rep != convert_to(common_unit, d).m_rep;
	}
}
bool duration::operator<(const duration& d) const {
	if (m_unit == d.m_unit) {
		return m_rep < d.m_rep;
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return convert_to(common_unit, *this).m_rep < convert_to(common_unit, d).m_rep;
	}
}
bool duration::operator<=(const duration& d) const {
	if (m_unit == d.m_unit) {
		return m_rep <= d.m_rep;
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return convert_to(common_unit, *this).m_rep <= convert_to(common_unit, d).m_rep;
	}
}
bool duration::operator>(const duration& d) const {
	if (m_unit == d.m_unit) {
		return m_rep > d.m_rep;
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return convert_to(common_unit, *this).m_rep > convert_to(common_unit, d).m_rep;
	}
}
bool duration::operator>=(const duration& d) const {
	if (m_unit == d.m_unit) {
		return m_rep >= d.m_rep;
	}
	else {
		const auto& common_unit = std::min(m_unit, d.m_unit, time_unit_less());
		return convert_to(common_unit, *this).m_rep >= convert_to(common_unit, d).m_rep;
	}
}

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

duration duration::convert_to(const time_unit& to_unit, const duration& d) {
	if (d.m_unit == to_unit) return d;

	if (to_unit == time_unit::CYCLE) {
		return duration(int64_t(cycles_per_nanosec * nsec_factor(d.m_unit) * d.m_rep), to_unit);
	}

	if (d.m_unit == time_unit::CYCLE) {
		return duration(int64_t(double(d.m_rep) / nsec_factor(to_unit) / cycles_per_nanosec), to_unit);
	}

	return duration(nsec_factor(d.m_unit) * d.m_rep / nsec_factor(to_unit), to_unit);
}

}} // namespace handystats::chrono
