// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CORE_IMPL_HPP_
#define HANDYSTATS_CORE_IMPL_HPP_

#include <mutex>

namespace handystats {

extern std::mutex operation_mutex;

bool is_enabled();

} // namespace handystats


#endif // HANDYSTATS_CORE_IMPL_HPP_
