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

#include <ctime>

#include <handystats/chrono.hpp>

namespace handystats { namespace chrono {

time_point system_clock::now() {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	return time_point(duration(ts.tv_sec * 1000ull * 1000ull * 1000ull + ts.tv_nsec, time_unit::NSEC), clock_type::SYSTEM);
}

}} // namespace handystats::chrono
