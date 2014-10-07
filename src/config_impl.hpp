// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_IMPL_HPP_
#define HANDYSTATS_CONFIG_IMPL_HPP_

#include "config/statistics_impl.hpp"
#include "config/metrics/gauge_impl.hpp"
#include "config/metrics/counter_impl.hpp"
#include "config/metrics/timer_impl.hpp"

#include "config/metrics_dump_impl.hpp"
#include "config/core_impl.hpp"

namespace handystats { namespace config {

extern statistics statistics_opts;

namespace metrics {
	extern gauge gauge_opts;
	extern counter counter_opts;
	extern timer timer_opts;
}

extern metrics_dump metrics_dump_opts;
extern core core_opts;

void initialize();
void finalize();

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_IMPL_HPP_
