/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HANDYSTATS_METRICS_DUMP_HPP_
#define HANDYSTATS_METRICS_DUMP_HPP_

#include <memory>
#include <string>
#include <utility>
#include <map>

#include <handystats/statistics.hpp>
#include <handystats/attribute.hpp>

namespace handystats {

typedef std::map<std::string, statistics> metric_map;
typedef std::map<std::string, attribute> attribute_map;

typedef std::pair<metric_map, attribute_map> metrics_dump_type;

} // namespace handystats

const std::shared_ptr<const handystats::metrics_dump_type> HANDY_METRICS_DUMP();

#endif // HANDYSTATS_METRICS_DUMP_HPP_
