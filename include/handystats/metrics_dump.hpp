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

#ifndef HANDYSTATS_METRICS_DUMP_HPP_
#define HANDYSTATS_METRICS_DUMP_HPP_

#include <memory>
#include <string>
#include <map>

#include <handystats/metrics.hpp>

const std::shared_ptr <
	const std::map <
		std::string, handystats::metrics::metric_variant
		>
	>
HANDY_METRICS_DUMP();

#endif // HANDYSTATS_METRICS_DUMP_HPP_
