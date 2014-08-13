// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_COMMON_H_
#define HANDYSTATS_COMMON_H_

/* extern */
#ifdef __cplusplus
	#define HANDYSTATS_EXTERN_C extern "C"
#else
	#define HANDYSTATS_EXTERN_C extern
#endif

/* noexcept */
#ifdef __cplusplus
	// GCC_VERSION = __GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__
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
