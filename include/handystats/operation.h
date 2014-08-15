// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_OPERATION_H_
#define HANDYSTATS_OPERATION_H_

#include <handystats/common.h>

HANDYSTATS_EXTERN_C
void handystats_initialize();

HANDYSTATS_EXTERN_C
void handystats_finalize();

#ifndef __cplusplus
	#ifndef HANDYSTATS_DISABLE

		#define HANDY_INIT(...) handystats_initialize(__VA_ARGS__)

		#define HANDY_FINALIZE(...) handystats_finalize(__VA_ARGS__)

	#else

		#define HANDY_INIT(...)

		#define HANDY_FINALIZE(...)

	#endif
#endif

#endif // HANDYSTATS_OPERATION_H_
