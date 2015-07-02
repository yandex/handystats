/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#ifndef HANDYSTATS_MEASURING_POINTS_ATTRIBUTE_PROXY_HPP_
#define HANDYSTATS_MEASURING_POINTS_ATTRIBUTE_PROXY_HPP_

#include <cstdint>

#include <string>

#include <handystats/metrics/attribute.hpp>

#include <handystats/measuring_points/attribute.hpp>

namespace handystats { namespace measuring_points {

class attribute_proxy {
public:
	attribute_proxy(const std::string& name)
		: name(name)
	{}

	attribute_proxy(const char* name)
		: name(name)
	{}

	void set(
			const metrics::attribute::value_type& value,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET(name.substr(), value, timestamp);
	}

	void set(
			const bool& b,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET_BOOL(name.substr(), b, timestamp);
	}

	void set(
			const int& i,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET_INT(name.substr(), i, timestamp);
	}

	void set(
			const unsigned& u,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET_UINT(name.substr(), u, timestamp);
	}

	void set(
			const int64_t& i64,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET_INT64(name.substr(), i64, timestamp);
	}

	void set(
			const uint64_t& u64,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET_UINT64(name.substr(), u64, timestamp);
	}

	void set(
			const double& d,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET_DOUBLE(name.substr(), d, timestamp);
	}

	void set(
			const std::string& s,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET_STRING(name.substr(), s, timestamp);
	}

private:
	const std::string name;
};

}} // namespace handystats::measuring_points

#endif // HANDYSTATS_MEASURING_POINTS_ATTRIBUTE_PROXY_HPP_
