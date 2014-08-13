// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_MEASURING_POINTS_ATTRIBUTE_PROXY_HPP_
#define HANDYSTATS_MEASURING_POINTS_ATTRIBUTE_PROXY_HPP_

#include <cstdint>

#include <string>

#include <handystats/metrics/attribute.hpp>

#include <handystats/measuring_points/attribute_measuring_points.hpp>

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
		HANDY_ATTRIBUTE_SET(name, value, timestamp);
	}

	void set(
			const bool& b,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET(name, b, timestamp);
	}

	void set(
			const int& i,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET(name, i, timestamp);
	}

	void set(
			const unsigned& u,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET(name, u, timestamp);
	}

	void set(
			const int64_t& i64,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET(name, i64, timestamp);
	}

	void set(
			const uint64_t& u64,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET(name, u64, timestamp);
	}

	void set(
			const double& d,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET(name, d, timestamp);
	}

	void set(
			const char* s,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET(name, s, timestamp);
	}

	void set(
			const std::string& s,
			const metrics::attribute::time_point& timestamp = metrics::attribute::clock::now()
		)
	{
		HANDY_ATTRIBUTE_SET(name, s, timestamp);
	}

private:
	const std::string name;
};

}} // namespace handystats::measuring_points

#endif // HANDYSTATS_MEASURING_POINTS_ATTRIBUTE_PROXY_HPP_
