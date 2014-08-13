// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_ATTRIBUTE_MEASURING_POINTS_HPP_
#define HANDYSTATS_ATTRIBUTE_MEASURING_POINTS_HPP_

#include <cstdint>

#include <string>

#include <handystats/metrics/attribute.hpp>


namespace handystats { namespace measuring_points {

void attribute_set(
		const std::string& attribute_name,
		const handystats::metrics::attribute::value_type& value,
		const handystats::metrics::attribute::time_point& timestamp = handystats::metrics::attribute::clock::now()
	);

void attribute_set(
		const std::string& attribute_name,
		const bool& b,
		const handystats::metrics::attribute::time_point& timestamp = handystats::metrics::attribute::clock::now()
	);

void attribute_set(
		const std::string& attribute_name,
		const int& i,
		const handystats::metrics::attribute::time_point& timestamp = handystats::metrics::attribute::clock::now()
	);

void attribute_set(
		const std::string& attribute_name,
		const unsigned& u,
		const handystats::metrics::attribute::time_point& timestamp = handystats::metrics::attribute::clock::now()
	);

void attribute_set(
		const std::string& attribute_name,
		const int64_t& i64,
		const handystats::metrics::attribute::time_point& timestamp = handystats::metrics::attribute::clock::now()
	);

void attribute_set(
		const std::string& attribute_name,
		const uint64_t& u64,
		const handystats::metrics::attribute::time_point& timestamp = handystats::metrics::attribute::clock::now()
	);

void attribute_set(
		const std::string& attribute_name,
		const double& d,
		const handystats::metrics::attribute::time_point& timestamp = handystats::metrics::attribute::clock::now()
	);

void attribute_set(
		const std::string& attribute_name,
		const char* s,
		const handystats::metrics::attribute::time_point& timestamp = handystats::metrics::attribute::clock::now()
	);

void attribute_set(
		const std::string& attribute_name,
		const std::string& s,
		const handystats::metrics::attribute::time_point& timestamp = handystats::metrics::attribute::clock::now()
	);

}} // namespace handystats::measuring_points


#ifndef HANDYSTATS_DISABLE

	#define HANDY_ATTRIBUTE_SET(...) handystats::measuring_points::attribute_set(__VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_JSON(...) handystats::measuring_points::attribute_set(__VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_BOOL(...) handystats::measuring_points::attribute_set(__VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_INT(...) handystats::measuring_points::attribute_set(__VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_UINT(...) handystats::measuring_points::attribute_set(__VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_INT64(...) handystats::measuring_points::attribute_set(__VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_UINT64(...) handystats::measuring_points::attribute_set(__VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_DOUBLE(...) handystats::measuring_points::attribute_set(__VA_ARGS__)

	#define HANDY_ATTRIBURE_SET_STRING(...) handystats::measuring_point::attribute_set(__VA_ARGS__)

#else

	#define HANDY_ATTRIBUTE_SET(...)

	#define HANDY_ATTRIBUTE_SET_JSON(...)

	#define HANDY_ATTRIBUTE_SET_BOOL(...)

	#define HANDY_ATTRIBUTE_SET_INT(...)

	#define HANDY_ATTRIBUTE_SET_UINT(...)

	#define HANDY_ATTRIBUTE_SET_INT64(...)

	#define HANDY_ATTRIBUTE_SET_UINT64(...)

	#define HANDY_ATTRIBUTE_SET_DOUBLE(...)

	#define HANDY_ATTRIBURE_SET_STRING(...)

#endif

#endif // HANDYSTATS_ATTRIBUTE_MEASURING_POINTS_HPP_
