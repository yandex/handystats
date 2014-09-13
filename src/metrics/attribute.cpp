// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/attribute.hpp>


namespace handystats { namespace metrics {

attribute::attribute()
	: m_value()
{
}

void attribute::set(const attribute::value_type& value) {
	m_value = value;
}

void attribute::set(const bool& b) {
	m_value = b;
}

void attribute::set(const int& i) {
	m_value = i;
}

void attribute::set(const unsigned& u) {
	m_value = u;
}

void attribute::set(const int64_t& i64) {
	m_value = i64;
}

void attribute::set(const uint64_t& u64) {
	m_value = u64;
}

void attribute::set(const double& d) {
	m_value = d;
}

void attribute::set(const char* s) {
	set(std::string(s));
}

void attribute::set(const std::string& s) {
	m_value = s;
}

const attribute::value_type& attribute::value() const {
	return m_value;
}

}} // namespace handystats::metrics
