// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_METRICS_GAUGE_HPP_
#define HANDYSTATS_CONFIG_METRICS_GAUGE_HPP_

#include <handystats/config/statistics.hpp>

namespace handystats { namespace config { namespace metrics {

struct gauge {
	statistics values;

	gauge();
};

}}} // namespace handystats::config::metrics

#endif // HANDYSTATS_CONFIG_METRICS_GAUGE_HPP_
