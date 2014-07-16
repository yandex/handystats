// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_JSON_DUMP_IMPL_H_
#define HANDYSTATS_JSON_DUMP_IMPL_H_

#include <handystats/metrics/gauge.hpp>

namespace handystats { namespace json_dump {

void update();

void initialize();
void finalize();


namespace stats {

extern metrics::gauge dump_time;

void initialize();
void finalize();

} // namespace stats


}} // namespace handystats::json_dump

#endif // HANDYSTATS_JSON_DUMP_IMPL_H_
