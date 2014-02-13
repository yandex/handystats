#ifndef HANDYSTATS_MOVING_AVERAGE_H_
#define HANDYSTATS_MOVING_AVERAGE_H_

#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>

namespace boost { namespace accumulators {

BOOST_PARAMETER_NESTED_KEYWORD(tag, moving_average_alpha, alpha)

namespace impl {

template <typename value_type>
struct moving_average_impl : accumulator_base {

	typedef double result_type;

	template <typename Args>
	moving_average_impl(Args const& args)
		: moving_average(args[sample | value_type()])
		, alpha(args[moving_average_alpha])
	{
	}

	template<typename Args>
	void operator() (Args const& args) {
		if (count(args[accumulator]) == 1) {
			this->moving_average = args[sample];
		}
		else {
			this->moving_average *= (1 - this->alpha);
			this->moving_average += this->alpha * args[sample];
		}
	}

	result_type result(dont_care) const {
		return this->moving_average;
	}

private:
	double moving_average;
	double alpha;
};

} // namespace boost::accumulators::impl


namespace tag {

struct moving_average
	: depends_on< count >
	, tag::moving_average_alpha
{
	typedef accumulators::impl::moving_average_impl< mpl::_1 > impl;
};

} // namespace boost::accumulators::tag


namespace extract {

extractor<tag::moving_average> const moving_average = {};

} // namespace boost::accumulators::extract

using extract::moving_average;

}} // namespace boost::accumulators


#endif // HANDYSTATS_MOVING_AVERAGE_H_
