// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_METRICS_GAUGE_HPP_
#define HANDYSTATS_METRICS_GAUGE_HPP_

#include <utility>

#include <handystats/chrono.hpp>
#include <handystats/statistics.hpp>
#include <handystats/config/metrics/gauge.hpp>

namespace handystats { namespace metrics {

struct gauge
{
	typedef double value_type;
	typedef chrono::clock clock;
	typedef clock::time_point time_point;

	gauge(const config::metrics::gauge& opts = config::metrics::gauge());

	void set(const value_type& value, const time_point& timestamp = clock::now());

	void update_statistics(const time_point& timestamp = clock::now());

	const statistics& values() const;

private:
	statistics m_values;

}; // struct gauge

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_GAUGE_HPP_
