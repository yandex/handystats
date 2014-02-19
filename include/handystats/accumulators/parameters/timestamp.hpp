#ifndef HANDYSTATS_TIMESTAMP_PARAMETER_H_
#define HANDYSTATS_TIMESTAMP_PARAMETER_H_

#include <boost/parameter/keyword.hpp>
#include <boost/accumulators/accumulators_fwd.hpp>

namespace boost { namespace accumulators {

BOOST_PARAMETER_KEYWORD(tag, timestamp)
BOOST_ACCUMULATORS_IGNORE_GLOBAL(timestamp)

}} // namespace boost::accumulators

#endif // HANDYSTATS_TIMESTAMP_PARAMETER_H_
