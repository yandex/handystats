// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_INTERVAL_COUNT_HPP_
#define HANDYSTATS_INTERVAL_COUNT_HPP_

#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/statistics/count.hpp>

#include <handystats/chrono.hpp>
#include <handystats/configuration/defaults.hpp>
#include <handystats/math_utils.hpp>
#include <handystats/accumulators/parameters/time_interval.hpp>
#include <handystats/accumulators/parameters/timestamp.hpp>

namespace boost { namespace accumulators { namespace impl {

template <typename value_type>
struct interval_count_impl : accumulator_base {

	typedef double result_type;
	typedef typename handystats::chrono::time_duration time_duration;
	typedef typename handystats::chrono::clock::time_point time_point;

	template <typename Args>
	interval_count_impl(Args const& args)
		: count(0)
		, last_timestamp(args[parameter::keyword<tag::timestamp>::get() | time_point()])
		, time_interval(args[parameter::keyword<tag::time_interval>::get() |
				handystats::chrono::duration_cast<time_duration>(
					handystats::config::defaults::incremental_statistics::moving_interval
					)
				])
	{
	}

	template<typename Args>
	void operator() (Args const& args) {
		if (extract::count(args[accumulator]) == 1) {
			this->count = 1.0;
		}
		else {
			double elapsed = double(handystats::chrono::duration_cast<time_duration>(args[timestamp] - last_timestamp).count()) / this->time_interval.count();
			if (handystats::math_utils::cmp(elapsed, 1.0) > 0) {
				this->count = (1.0 / elapsed);
			}
			else if (handystats::math_utils::cmp(elapsed, 0.0) <= 0 &&
					handystats::math_utils::cmp(elapsed, -1.0) >= 0)
			{
				this->count = this->count + 1.0;
			}
			else if (handystats::math_utils::cmp(elapsed, 0.0) > 0) {
				this->count = this->count * (1.0 - elapsed) + 1.0;
			}
		}

		if (this->last_timestamp < args[timestamp]) {
			this->last_timestamp = args[timestamp];
		}
	}

	result_type result(dont_care) const {
		return this->count;
	}

private:
	double count;
	time_point last_timestamp;

	time_duration time_interval;
};

} // namespace boost::accumulators::impl


namespace tag {

struct interval_count
	: depends_on< count >
{
	typedef accumulators::impl::interval_count_impl< mpl::_1 > impl;
};

} // namespace boost::accumulators::tag


namespace extract {

extractor<tag::interval_count> const interval_count = {};

//BOOST_ACCUMULATORS_IGNORE_GLOBAL(interval_count)

} // namespace boost::accumulators::extract

using extract::interval_count;

}} // namespace boost::accumulators


#endif // HANDYSTATS_INTERVAL_COUNT_HPP_
