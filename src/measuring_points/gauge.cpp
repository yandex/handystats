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

#include <memory>
#include <algorithm>

#include "events/gauge_impl.hpp"
#include "core_impl.hpp"

#include <handystats/measuring_points/gauge.hpp>
#include <handystats/measuring_points/gauge.h>


namespace handystats { namespace measuring_points {

void gauge_init(
		std::string&& gauge_name,
		const handystats::metrics::gauge::value_type& init_value,
		const handystats::metrics::gauge::time_point& timestamp
	)
{
	if (core) {
		core->m_channel.push(
				handystats::events::gauge::create_init_event(std::move(gauge_name), init_value, timestamp)
			);
	}
}

void gauge_set(
		std::string&& gauge_name,
		const handystats::metrics::gauge::value_type& value,
		const handystats::metrics::gauge::time_point& timestamp
	)
{
	if (core) {
		core->m_channel.push(
				handystats::events::gauge::create_set_event(std::move(gauge_name), value, timestamp)
			);
	}
}

}} // namespace handystats::measuring_points


extern "C" {

void handystats_gauge_init(
		const char* gauge_name,
		const double init_value
	)
{
	handystats::measuring_points::gauge_init(gauge_name, init_value);
}

void handystats_gauge_set(
		const char* gauge_name,
		const double value
	)
{
	handystats::measuring_points::gauge_set(gauge_name, value);
}

} // extern "C"
