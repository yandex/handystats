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

#include <handystats/config/metrics/gauge.hpp>

#include "config/statistics_impl.hpp"
#include "config/metrics/gauge_impl.hpp"

namespace handystats { namespace config { namespace metrics {

gauge::gauge()
	: values(statistics())
{
}

}}} // namespace handystats::config::metrics


namespace handystats { namespace config {

void apply(const rapidjson::Value& config, metrics::gauge& gauge_opts) {
	apply(config, gauge_opts.values);
}

}} // namespace handystats::config
