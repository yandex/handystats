// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIGURATION_DEFAULTS_H_
#define HANDYSTATS_CONFIGURATION_DEFAULTS_H_

#include <chrono>
#include <vector>

namespace handystats { namespace config { namespace defaults {

namespace incremental_statistics {

extern const double moving_average_alpha;
extern const std::chrono::milliseconds moving_interval;

}

namespace timer {

extern const std::chrono::milliseconds idle_timeout;

}

namespace metrics_dump {

extern const std::chrono::milliseconds interval;
extern const bool to_json;

}

}}} // namespace handystats::config::defaults

#endif // HANDYSTATS_CONFIGURATION_DEFAULTS_H_
