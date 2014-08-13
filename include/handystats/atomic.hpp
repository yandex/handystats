// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_ATOMIC_HPP_
#define HANDYSTATS_ATOMIC_HPP_

#include <handystats/common.h>

#if GCC_VERSION >= 40500
	#include <atomic>
#else
	#include <cstdatomic>
#endif

#endif // HANDYSTATS_ATOMIC_HPP_
