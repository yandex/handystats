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

#ifndef HANDYSTATS_COMMON_H_
#define HANDYSTATS_COMMON_H_

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

/* extern */
#ifdef __cplusplus
	#define HANDYSTATS_EXTERN_C extern "C"
#else
	#define HANDYSTATS_EXTERN_C extern
#endif

/* noexcept */
#ifdef __cplusplus
	#if GCC_VERSION >= 40600
		#define HANDYSTATS_NOEXCEPT noexcept
	#else
		#define HANDYSTATS_NOEXCEPT throw()
	#endif
#endif

/* nullptr */
#ifndef HAVE_NULLPTR
	#ifndef __cplusplus
		#define nullptr ((void*)0)
	#else
		#define nullptr __null
	#endif
#endif

#endif // HANDYSTATS_COMMON_H_
