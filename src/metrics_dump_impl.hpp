// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_METRICS_DUMP_IMPL_HPP_
#define HANDYSTATS_METRICS_DUMP_IMPL_HPP_

#include <memory>

#include <handystats/chrono.hpp>
#include <handystats/metrics_dump.hpp>

#include "core_impl.hpp"

namespace handystats { namespace metrics_dump {

void update_dump(core_t& core, const chrono::time_point& current_timestamp);

const std::shared_ptr<const metrics_dump_type> get_dump(core_t& core);

}} // namespace handystats::metrics_dump

#endif // HANDYSTATS_METRICS_DUMP_IMPL_HPP_
