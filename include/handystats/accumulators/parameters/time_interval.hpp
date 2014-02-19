#ifndef HANDYSTATS_TIME_INTERVAL_PARAMETER_H_
#define HANDYSTATS_TIME_INTERVAL_PARAMETER_H_

#include <boost/parameter/keyword.hpp>
#include <boost/accumulators/accumulators_fwd.hpp>

namespace boost { namespace accumulators {

BOOST_PARAMETER_KEYWORD(tag, time_interval)
BOOST_ACCUMULATORS_IGNORE_GLOBAL(time_interval)

}} // namespace boost::accumulators

#endif // HANDYSTATS_TIME_INTERVAL_PARAMETER_H_
