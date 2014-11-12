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
