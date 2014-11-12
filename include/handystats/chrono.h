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

#ifndef HANDYSTATS_CHRONO_H_
#define HANDYSTATS_CHRONO_H_

#include <stdint.h>

#include <handystats/common.h>

HANDYSTATS_EXTERN_C
int64_t handystats_now(void);

HANDYSTATS_EXTERN_C
double handystats_difftime(int64_t end, int64_t start);

#endif // HANDYSTATS_CHRONO_H_
