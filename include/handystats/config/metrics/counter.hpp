/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#ifndef HANDYSTATS_CONFIG_METRICS_COUNTER_HPP_
#define HANDYSTATS_CONFIG_METRICS_COUNTER_HPP_

#include <handystats/config/statistics.hpp>

namespace handystats { namespace config { namespace metrics {

struct counter {
	statistics values;

	counter();
};

}}} // namespace handystats::config::metrics

#endif // HANDYSTATS_CONFIG_METRICS_COUNTER_HPP_
