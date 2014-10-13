// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_METRICS_DUMP_IMPL_HPP_
#define HANDYSTATS_METRICS_DUMP_IMPL_HPP_

#include <string>
#include <memory>
#include <map>

#include <handystats/chrono.hpp>
#include <handystats/statistics.hpp>
#include <handystats/metrics/gauge.hpp>
#include <handystats/metrics_dump.hpp>

namespace handystats { namespace metrics_dump {

extern chrono::time_point dump_timestamp;

void update(const chrono::time_point& current_time, const chrono::time_point& internal_time);

const std::shared_ptr<const metrics_dump_type> get_dump();

void initialize();
void finalize();


namespace stats {

extern metrics::gauge dump_time;

void update(const chrono::time_point&);

void initialize();
void finalize();

} // namespace stats


}} // namespace handystats::metrics_dump

#endif // HANDYSTATS_METRICS_DUMP_IMPL_HPP_
