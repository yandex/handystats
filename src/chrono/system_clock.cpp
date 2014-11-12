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

#include <ctime>

#include <handystats/chrono.hpp>

namespace handystats { namespace chrono {

time_point system_clock::now() {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	return time_point(duration(ts.tv_sec * 1000ull * 1000ull * 1000ull + ts.tv_nsec, time_unit::NSEC), clock_type::SYSTEM_CLOCK);
}

}} // namespace handystats::chrono
