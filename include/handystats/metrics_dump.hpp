// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_METRICS_DUMP_HPP_
#define HANDYSTATS_METRICS_DUMP_HPP_

#include <memory>
#include <string>
#include <utility>
#include <map>

#include <handystats/metrics.hpp>
#include <handystats/attribute.hpp>

namespace handystats {

typedef std::map<std::string, metrics::metric_variant> metric_variant_map;
typedef std::map<std::string, attribute> attribute_map;

typedef std::pair<metric_variant_map, attribute_map> metrics_dump_type;

} // namespace handystats

const std::shared_ptr<const handystats::metrics_dump_type> HANDY_METRICS_DUMP();

#endif // HANDYSTATS_METRICS_DUMP_HPP_
