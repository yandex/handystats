// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_ATTRIBUTE_MEASURING_POINTS_HPP_
#define HANDYSTATS_ATTRIBUTE_MEASURING_POINTS_HPP_

#include <cstdint>

#include <string>

#include <handystats/metrics/attribute.hpp>
#include <handystats/macros.h>


namespace handystats { namespace measuring_points {

template <typename ValueType>
void attribute_set(
		std::string&& attribute_name,
		const ValueType& value,
		const handystats::metrics::attribute::time_point& timestamp = handystats::metrics::attribute::clock::now()
	);

template <>
void attribute_set<handystats::metrics::attribute::value_type>(
		std::string&& attribute_name,
		const handystats::metrics::attribute::value_type& value,
		const handystats::metrics::attribute::time_point& timestamp
	);

template <>
void attribute_set<bool>(
		std::string&& attribute_name,
		const bool& b,
		const handystats::metrics::attribute::time_point& timestamp
	);

template <>
void attribute_set<int>(
		std::string&& attribute_name,
		const int& i,
		const handystats::metrics::attribute::time_point& timestamp
	);

template <>
void attribute_set<unsigned>(
		std::string&& attribute_name,
		const unsigned& u,
		const handystats::metrics::attribute::time_point& timestamp
	);

template <>
void attribute_set<int64_t>(
		std::string&& attribute_name,
		const int64_t& i64,
		const handystats::metrics::attribute::time_point& timestamp
	);

template <>
void attribute_set<uint64_t>(
		std::string&& attribute_name,
		const uint64_t& u64,
		const handystats::metrics::attribute::time_point& timestamp
	);

template <>
void attribute_set<double>(
		std::string&& attribute_name,
		const double& d,
		const handystats::metrics::attribute::time_point& timestamp
	);

template <>
void attribute_set<std::string>(
		std::string&& attribute_name,
		const std::string& s,
		const handystats::metrics::attribute::time_point& timestamp
	);

}} // namespace handystats::measuring_points


#ifndef HANDYSTATS_DISABLE

	#define HANDY_ATTRIBUTE_SET(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::attribute_set, __VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_BOOL(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::attribute_set<bool>, __VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_INT(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::attribute_set<int>, __VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_UINT(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::attribute_set<unsigned>, __VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_INT64(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::attribute_set<int64_t>, __VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_UINT64(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::attribute_set<uint64_t>, __VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_DOUBLE(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::attribute_set<double>, __VA_ARGS__)

	#define HANDY_ATTRIBUTE_SET_STRING(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::attribute_set<std::string>, __VA_ARGS__)

#else

	#define HANDY_ATTRIBUTE_SET(...)

	#define HANDY_ATTRIBUTE_SET_BOOL(...)

	#define HANDY_ATTRIBUTE_SET_INT(...)

	#define HANDY_ATTRIBUTE_SET_UINT(...)

	#define HANDY_ATTRIBUTE_SET_INT64(...)

	#define HANDY_ATTRIBUTE_SET_UINT64(...)

	#define HANDY_ATTRIBUTE_SET_DOUBLE(...)

	#define HANDY_ATTRIBUTE_SET_STRING(...)

#endif

#endif // HANDYSTATS_ATTRIBUTE_MEASURING_POINTS_HPP_
