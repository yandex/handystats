// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <memory>
#include <algorithm>

#include "events/attribute_impl.hpp"
#include "message_queue_impl.hpp"
#include "core_impl.hpp"

#include <handystats/measuring_points/attribute.hpp>
#include <handystats/measuring_points/attribute.h>


namespace handystats { namespace measuring_points {

template <>
void attribute_set<attribute::value_type>(
		std::string&& attr_name,
		const attribute::value_type& value,
		const attribute::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::attribute::create_set_event(std::move(attr_name), value, timestamp)
			);
	}
}

template <>
void attribute_set<bool>(
		std::string&& attr_name,
		const bool& b,
		const attribute::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::attribute::create_set_event(
					std::move(attr_name),
					attribute::value_type(b),
					timestamp
				)
			);
	}
}

template <>
void attribute_set<int>(
		std::string&& attr_name,
		const int& i,
		const attribute::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::attribute::create_set_event(
					std::move(attr_name),
					attribute::value_type(i),
					timestamp
				)
			);
	}
}

template <>
void attribute_set<unsigned>(
		std::string&& attr_name,
		const unsigned& u,
		const attribute::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::attribute::create_set_event(
					std::move(attr_name),
					attribute::value_type(u),
					timestamp
				)
			);
	}
}

template <>
void attribute_set<int64_t>(
		std::string&& attr_name,
		const int64_t& i64,
		const attribute::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::attribute::create_set_event(
					std::move(attr_name),
					attribute::value_type(i64),
					timestamp
				)
			);
	}
}

template <>
void attribute_set<uint64_t>(
		std::string&& attr_name,
		const uint64_t& u64,
		const attribute::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::attribute::create_set_event(
					std::move(attr_name),
					attribute::value_type(u64),
					timestamp
				)
			);
	}
}

template <>
void attribute_set<double>(
		std::string&& attr_name,
		const double& d,
		const attribute::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::attribute::create_set_event(
					std::move(attr_name),
					attribute::value_type(d),
					timestamp
				)
			);
	}
}

template <>
void attribute_set<std::string>(
		std::string&& attr_name,
		const std::string& s,
		const attribute::time_point& timestamp
	)
{
	if (is_enabled()) {
		message_queue::push(
				events::attribute::create_set_event(
					std::move(attr_name),
					attribute::value_type(s),
					timestamp
				)
			);
	}
}

}} // namespace handystats::measuring_points


extern "C" {

void handystats_attribute_set_bool(
		const char* attr_name,
		const char b
	)
{
	handystats::measuring_points::attribute_set<bool>(attr_name, (bool)b);
}

void handystats_attribute_set_int(
		const char* attr_name,
		const int i
	)
{
	handystats::measuring_points::attribute_set<int>(attr_name, i);
}

void handystats_attribute_set_uint(
		const char* attr_name,
		const unsigned u
	)
{
	handystats::measuring_points::attribute_set<unsigned>(attr_name, u);
}

void handystats_attribute_set_int64(
		const char* attr_name,
		const int64_t i64
	)
{
	handystats::measuring_points::attribute_set<int64_t>(attr_name, i64);
}

void handystats_attribute_set_uint64(
		const char* attr_name,
		const uint64_t u64
	)
{
	handystats::measuring_points::attribute_set<uint64_t>(attr_name, u64);
}

void handystats_attribute_set_double(
		const char* attr_name,
		const double d
	)
{
	handystats::measuring_points::attribute_set<double>(attr_name, d);
}

void handystats_attribute_set_string(
		const char* attr_name,
		const char* s
	)
{
	handystats::measuring_points::attribute_set<std::string>(attr_name, s);
}

} // extern "C"
