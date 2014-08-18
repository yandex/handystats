// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_INCREMENTAL_STATISTICS_HPP_
#define HANDYSTATS_INCREMENTAL_STATISTICS_HPP_

#include <handystats/chrono.hpp>
#include <handystats/configuration/defaults.hpp>

namespace handystats {

class incremental_statistics {
public:
	typedef double value_type;
	typedef chrono::clock clock;
	typedef clock::duration duration;
	typedef clock::time_point time_point;

	incremental_statistics(
			const chrono::time_duration& moving_interval = config::defaults::incremental_statistics::moving_interval,
			const double& moving_average_alpha = config::defaults::incremental_statistics::moving_average_alpha
		);

	void operator() (const value_type& value, const time_point& timestamp = clock::now());

	value_type min() const;
	value_type max() const;
	value_type sum() const;
	size_t count() const;
	value_type mean() const;
	value_type interval_count() const;
	value_type interval_sum() const;
	value_type interval_mean() const;
	value_type moving_average() const;
	time_point timestamp() const;

private:
	duration m_moving_interval;
	double m_moving_average_alpha;

	value_type m_min;
	value_type m_max;
	value_type m_sum;
	size_t m_count;
	value_type m_interval_count;
	value_type m_interval_sum;
	value_type m_moving_average;

	time_point m_timestamp;
};

} // namespace handystats

#endif // HANDYSTATS_INCREMENTAL_STATISTICS_HPP_
