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

#include <handystats/chrono.h>
#include <handystats/chrono.hpp>

HANDYSTATS_EXTERN_C
int64_t handystats_now(void) {
	return (int64_t)handystats::chrono::internal_clock::now().time_since_epoch().count();
}

HANDYSTATS_EXTERN_C
double handystats_difftime(int64_t end, int64_t start) {
	const auto& tsc_unit = handystats::chrono::internal_clock::now().time_since_epoch().unit();
	const auto& duration = handystats::chrono::duration(end - start, tsc_unit);

	return (double)duration.count(handystats::chrono::time_unit::NSEC) / 1E9;
}
