// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_IMPL_HPP_
#define HANDYSTATS_CONFIG_IMPL_HPP_

#include <handystats/config/statistics.hpp>
#include <handystats/config/timer.hpp>
#include <handystats/config/metrics_dump.hpp>
#include <handystats/config/core.hpp>

namespace handystats { namespace config {

extern statistics statistics_opts;
extern timer timer_opts;
extern metrics_dump metrics_dump_opts;
extern core core_opts;

extern bool default_initialized;

void initialize();
void finalize();

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_IMPL_HPP_
