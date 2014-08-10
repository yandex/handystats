// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_TIME_INTERVAL_PARAMETER_HPP_
#define HANDYSTATS_TIME_INTERVAL_PARAMETER_HPP_

#include <boost/parameter/keyword.hpp>
#include <boost/accumulators/accumulators_fwd.hpp>

namespace boost { namespace accumulators {

BOOST_PARAMETER_KEYWORD(tag, time_interval)
BOOST_ACCUMULATORS_IGNORE_GLOBAL(time_interval)

}} // namespace boost::accumulators

#endif // HANDYSTATS_TIME_INTERVAL_PARAMETER_HPP_
