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

#ifndef HANDY_GAUGE_MEASURING_POINTS_H_
#define HANDY_GAUGE_MEASURING_POINTS_H_

#include <handystats/common.h>

HANDYSTATS_EXTERN_C
void handystats_gauge_init(
		const char* gauge_name,
		const double init_value
	);

HANDYSTATS_EXTERN_C
void handystats_gauge_set(
		const char* gauge_name,
		const double value
	);


#ifndef __cplusplus
	#ifndef HANDYSTATS_DISABLE

		#define HANDY_GAUGE_INIT(...) handystats_gauge_init(__VA_ARGS__)

		#define HANDY_GAUGE_SET(...) handystats_gauge_set(__VA_ARGS__)

	#else

		#define HANDY_GAUGE_INIT(...)

		#define HANDY_GAUGE_SET(...)

	#endif

#endif

#endif // HANDYSTATS_GAUGE_MEASURING_POINTS_H_
