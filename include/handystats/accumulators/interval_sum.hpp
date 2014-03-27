#ifndef HANDYSTATS_INTERVAL_SUM_H_
#define HANDYSTATS_INTERVAL_SUM_H_

#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/statistics/count.hpp>

#include <handystats/chrono.hpp>
#include <handystats/math_utils.hpp>
#include <handystats/accumulators/parameters/time_interval.hpp>
#include <handystats/accumulators/parameters/timestamp.hpp>

namespace boost { namespace accumulators { namespace impl {

template <typename value_type>
struct interval_sum_impl : accumulator_base {

	typedef double result_type;
	typedef handystats::chrono::default_duration duration;
	typedef handystats::chrono::default_time_point time_point;

	template <typename Args>
	interval_sum_impl(Args const& args)
		: sum(args[sample | value_type()])
		, last_timestamp(args[parameter::keyword<tag::timestamp>::get() | time_point()])
		, time_interval(args[parameter::keyword<tag::time_interval>::get() | std::chrono::duration_cast<duration>(std::chrono::seconds(1))])
	{
	}

	template<typename Args>
	void operator() (Args const& args) {
		if (count(args[accumulator]) == 1) {
			this->sum = args[sample];
		}
		else {
			double elapsed = double(std::chrono::duration_cast<duration>(args[timestamp] - last_timestamp).count()) / this->time_interval.count();
			if (handystats::math_utils::cmp(elapsed, 1.0) > 0) {
				this->sum = (args[sample] / elapsed);
			}
			else if (handystats::math_utils::cmp(elapsed, 0.0) <= 0 &&
					handystats::math_utils::cmp(elapsed, -1.0) >= 0)
			{
				this->sum = this->sum + args[sample];
			}
			else if (handystats::math_utils::cmp(elapsed, 0.0) > 0) {
				this->sum = this->sum * (1.0 - elapsed) + args[sample];
			}
		}

		if (this->last_timestamp < args[timestamp]) {
			this->last_timestamp = args[timestamp];
		}
	}

	result_type result(dont_care) const {
		return this->sum;
	}

private:
	double sum;
	time_point last_timestamp;

	duration time_interval;
};

} // namespace boost::accumulators::impl


namespace tag {

struct interval_sum
	: depends_on< count >
{
	typedef accumulators::impl::interval_sum_impl< mpl::_1 > impl;
};

} // namespace boost::accumulators::tag


namespace extract {

extractor<tag::interval_sum> const interval_sum = {};

BOOST_ACCUMULATORS_IGNORE_GLOBAL(interval_sum)

} // namespace boost::accumulators::extract

using extract::interval_sum;

}} // namespace boost::accumulators


#endif // HANDYSTATS_INTERVAL_SUM_H_
