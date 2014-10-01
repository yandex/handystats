// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_INTERNAL_IMPL_HPP_
#define HANDYSTATS_INTERNAL_IMPL_HPP_

#include <map>
#include <string>

#include <handystats/metrics.hpp>
#include <handystats/metrics/gauge.hpp>


namespace handystats { namespace events {

struct event_message;

}} // namespace handystats::events


namespace handystats { namespace internal {

extern std::map<std::string, metrics::metric_ptr_variant> metrics_map;

void update_metrics(const chrono::time_point&);

void process_event_message(const events::event_message&);

size_t size();

void initialize();
void finalize();


namespace stats {

extern metrics::gauge size;
extern metrics::gauge process_time;

void update(const chrono::time_point&);

void initialize();
void finalize();

} // namespace stats


}} // namespace handystats::internal


#endif // HANDYSTATS_INTERNAL_IMPL_HPP_
