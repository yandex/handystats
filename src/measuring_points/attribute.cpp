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
void attribute_set<handystats::metrics::attribute::value_type>(
		std::string&& attribute_name,
		const handystats::metrics::attribute::value_type& value,
		const handystats::metrics::attribute::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::attribute::create_set_event(std::move(attribute_name), value, timestamp)
			);
	}
}

template <>
void attribute_set<bool>(
		std::string&& attribute_name,
		const bool& b,
		const handystats::metrics::attribute::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::attribute::create_set_event(
					std::move(attribute_name),
					handystats::metrics::attribute::value_type(b),
					timestamp
				)
			);
	}
}

template <>
void attribute_set<int>(
		std::string&& attribute_name,
		const int& i,
		const handystats::metrics::attribute::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::attribute::create_set_event(
					std::move(attribute_name),
					handystats::metrics::attribute::value_type(i),
					timestamp
				)
			);
	}
}

template <>
void attribute_set<unsigned>(
		std::string&& attribute_name,
		const unsigned& u,
		const handystats::metrics::attribute::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::attribute::create_set_event(
					std::move(attribute_name),
					handystats::metrics::attribute::value_type(u),
					timestamp
				)
			);
	}
}

template <>
void attribute_set<int64_t>(
		std::string&& attribute_name,
		const int64_t& i64,
		const handystats::metrics::attribute::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::attribute::create_set_event(
					std::move(attribute_name),
					handystats::metrics::attribute::value_type(i64),
					timestamp
				)
			);
	}
}

template <>
void attribute_set<uint64_t>(
		std::string&& attribute_name,
		const uint64_t& u64,
		const handystats::metrics::attribute::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::attribute::create_set_event(
					std::move(attribute_name),
					handystats::metrics::attribute::value_type(u64),
					timestamp
				)
			);
	}
}

template <>
void attribute_set<double>(
		std::string&& attribute_name,
		const double& d,
		const handystats::metrics::attribute::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::attribute::create_set_event(
					std::move(attribute_name),
					handystats::metrics::attribute::value_type(d),
					timestamp
				)
			);
	}
}

template <>
void attribute_set<std::string>(
		std::string&& attribute_name,
		const std::string& s,
		const handystats::metrics::attribute::time_point& timestamp
	)
{
	if (handystats::is_enabled()) {
		handystats::message_queue::push(
				handystats::events::attribute::create_set_event(
					std::move(attribute_name),
					handystats::metrics::attribute::value_type(s),
					timestamp
				)
			);
	}
}

}} // namespace handystats::measuring_points


extern "C" {

void handystats_attribute_set_bool(
		const char* attribute_name,
		const char b
	)
{
	handystats::measuring_points::attribute_set<bool>(attribute_name, (bool)b);
}

void handystats_attribute_set_int(
		const char* attribute_name,
		const int i
	)
{
	handystats::measuring_points::attribute_set<int>(attribute_name, i);
}

void handystats_attribute_set_uint(
		const char* attribute_name,
		const unsigned u
	)
{
	handystats::measuring_points::attribute_set<unsigned>(attribute_name, u);
}

void handystats_attribute_set_int64(
		const char* attribute_name,
		const int64_t i64
	)
{
	handystats::measuring_points::attribute_set<int64_t>(attribute_name, i64);
}

void handystats_attribute_set_uint64(
		const char* attribute_name,
		const uint64_t u64
	)
{
	handystats::measuring_points::attribute_set<uint64_t>(attribute_name, u64);
}

void handystats_attribute_set_double(
		const char* attribute_name,
		const double d
	)
{
	handystats::measuring_points::attribute_set<double>(attribute_name, d);
}

void handystats_attribute_set_string(
		const char* attribute_name,
		const char* s
	)
{
	handystats::measuring_points::attribute_set<std::string>(attribute_name, s);
}

} // extern "C"
