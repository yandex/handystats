// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_TIMESTAMP_PARAMETER_HPP_
#define HANDYSTATS_TIMESTAMP_PARAMETER_HPP_

#include <boost/parameter/keyword.hpp>
#include <boost/accumulators/accumulators_fwd.hpp>

namespace boost { namespace accumulators {

BOOST_PARAMETER_KEYWORD(tag, timestamp)
BOOST_ACCUMULATORS_IGNORE_GLOBAL(timestamp)

}} // namespace boost::accumulators

#endif // HANDYSTATS_TIMESTAMP_PARAMETER_HPP_
