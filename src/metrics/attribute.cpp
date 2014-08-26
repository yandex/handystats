// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/attribute.hpp>


namespace handystats { namespace metrics {

attribute::attribute()
	: value()
	, timestamp()
{
}

void attribute::set(const value_type& value, const time_point& timestamp)
{
	this->value = value;
	this->timestamp = timestamp;
}

void attribute::set(const bool& b, const time_point& timestamp) {
	this->value = b;
	this->timestamp = timestamp;
}

void attribute::set(const int& i, const time_point& timestamp) {
	this->value = i;
	this->timestamp = timestamp;
}

void attribute::set(const unsigned& u, const time_point& timestamp) {
	this->value = u;
	this->timestamp = timestamp;
}

void attribute::set(const int64_t& i64, const time_point& timestamp) {
	this->value = i64;
	this->timestamp = timestamp;
}

void attribute::set(const uint64_t& u64, const time_point& timestamp) {
	this->value = u64;
	this->timestamp = timestamp;
}

void attribute::set(const double& d, const time_point& timestamp) {
	this->value = d;
	this->timestamp = timestamp;
}

void attribute::set(const char* s, const time_point& timestamp) {
	set(std::string(s), timestamp);
}

void attribute::set(const std::string& s, const time_point& timestamp) {
	this->value = s;
	this->timestamp = timestamp;
}

}} // namespace handystats::metrics
