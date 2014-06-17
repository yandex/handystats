// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_INTERVAL_MEAN_H_
#define HANDYSTATS_INTERVAL_MEAN_H_

#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/statistics/count.hpp>

#include <handystats/accumulators/interval_count.hpp>
#include <handystats/accumulators/interval_sum.hpp>

namespace boost { namespace accumulators { namespace impl {

template <typename value_type>
struct interval_mean_impl : accumulator_base {

	typedef double result_type;

	interval_mean_impl(dont_care) {}

	template <typename Args>
	result_type result(Args const &args) const {
		if (count(args) == 0) {
			return 0.0;
		}
		return interval_sum(args) / interval_count(args);
	}
};

} // namespace boost::accumulators::impl


namespace tag {

struct interval_mean
	: depends_on< interval_count, interval_sum >
{
	typedef accumulators::impl::interval_mean_impl< mpl::_1 > impl;
};

} // namespace boost::accumulators::tag


namespace extract {

extractor<tag::interval_mean> const interval_mean = {};

BOOST_ACCUMULATORS_IGNORE_GLOBAL(interval_mean)

} // namespace boost::accumulators::extract

using extract::interval_mean;

}} // namespace boost::accumulators


#endif // HANDYSTATS_INTERVAL_MEAN_H_
