// Copyright (c) 2014 Yandex LLC. All rights reserved.

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

	typedef chrono::clock clock;
	typedef clock::time_point time_point;

	attribute();

	void set(const value_type& value, const time_point& timestamp = clock::now());

	// support for primitive types (as in rapidjson)
	void set(const bool& b, const time_point& timestamp = clock::now());
	void set(const int& i, const time_point& timestamp = clock::now());
	void set(const unsigned& u, const time_point& timestamp = clock::now());
	void set(const int64_t& i64, const time_point& timestamp = clock::now());
	void set(const uint64_t& u64, const time_point& timestamp = clock::now());
	void set(const double& d, const time_point& timestamp = clock::now());

	// support for strings
	void set(const char* s, const time_point& timestamp = clock::now());
	void set(const std::string& s, const time_point& timestamp = clock::now());

	value_type value;
	time_point timestamp;

}; // struct attribute

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_ATTRIBUTE_HPP_
