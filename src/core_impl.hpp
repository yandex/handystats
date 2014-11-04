// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CORE_IMPL_HPP_
#define HANDYSTATS_CORE_IMPL_HPP_

#include <mutex>
#include <memory>

#include "message_queue_impl.hpp"

namespace handystats {

extern std::mutex operation_mutex;
extern std::unique_ptr<handystats::message_queue> channel;

bool is_enabled();

} // namespace handystats


#endif // HANDYSTATS_CORE_IMPL_HPP_
