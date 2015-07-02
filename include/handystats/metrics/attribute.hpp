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

#ifndef HANDYSTATS_METRICS_ATTRIBUTE_HPP_
#define HANDYSTATS_METRICS_ATTRIBUTE_HPP_

#include <cstdint>
#include <string>

#include <boost/variant.hpp>

#include <handystats/chrono.hpp>

namespace handystats { namespace metrics {

struct attribute
{
	typedef boost::variant <
			bool,
			int,
			unsigned,
			int64_t,
			uint64_t,
			double,
			std::string
		>
		value_type;

	enum value_index {
		BOOL = 0,
		INT,
		UINT,
		INT64,
		UINT64,
		DOUBLE,
		STRING
	};

	typedef chrono::tsc_clock clock;
	typedef chrono::time_point time_point;

	attribute();

	void set(const value_type& value);

	// support for primitive types (as in rapidjson)
	void set(const bool& b);
	void set(const int& i);
	void set(const unsigned& u);
	void set(const int64_t& i64);
	void set(const uint64_t& u64);
	void set(const double& d);

	// support for strings
	void set(const char* s);
	void set(const std::string& s);

	const value_type& value() const;

private:
	value_type m_value;

}; // struct attribute

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_ATTRIBUTE_HPP_
