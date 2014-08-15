// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_MEASURING_POINTS_GAUGE_PROXY_HPP_
#define HANDYSTATS_MEASURING_POINTS_GAUGE_PROXY_HPP_

#include <string>

#include <handystats/metrics/gauge.hpp>

#include <handystats/measuring_points/gauge.hpp>

namespace handystats { namespace measuring_points {

class gauge_proxy {
public:
	/*
	 * Ctors without sending init event
	 */
	gauge_proxy(const std::string& name)
		: name(name)
	{}

	gauge_proxy(const char* name)
		: name(name)
	{}

	/*
	 * Ctors with init parameters
	 */
	gauge_proxy(const std::string& name,
			const metrics::gauge::value_type& init_value,
			const metrics::gauge::time_point& timestamp = metrics::gauge::clock::now()
		)
		: name(name)
	{
		HANDY_GAUGE_INIT(name, init_value, timestamp);
	}

	gauge_proxy(const char* name,
			const metrics::gauge::value_type& init_value,
			const metrics::gauge::time_point& timestamp = metrics::gauge::clock::now()
		)
		: name(name)
	{
		HANDY_GAUGE_INIT(name, init_value, timestamp);
	}

	/*
	 * Proxy init event
	 */
	void init(
			const metrics::gauge::value_type& init_value,
			const metrics::gauge::time_point& timestamp = metrics::gauge::clock::now()
			)
	{
		HANDY_GAUGE_INIT(name, init_value, timestamp);
	}

	/*
	 * Proxy set event
	 */
	void set(
			const metrics::gauge::value_type& value,
			const metrics::gauge::time_point& timestamp = metrics::gauge::clock::now()
			)
	{
		HANDY_GAUGE_SET(name, value, timestamp);
	}

private:
	const std::string name;
};

}} // namespace handystats::measuring_points

#endif // HANDYSTATS_MEASURING_POINTS_GAUGE_PROXY_HPP_
