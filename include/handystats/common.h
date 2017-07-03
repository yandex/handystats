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
	#define HANDYSTATS_NOEXCEPT noexcept
#endif

#endif // HANDYSTATS_COMMON_H_
