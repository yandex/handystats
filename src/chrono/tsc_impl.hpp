// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CHRONO_TSC_IMPL_HPP_
#define HANDYSTATS_CHRONO_TSC_IMPL_HPP_

namespace handystats { namespace chrono {

inline uint64_t rdtsc() {
	uint64_t tsc;
	asm volatile (
#if defined(_HANDYSTATS_USE_RDTSCP) && _HANDYSTATS_USE_RDTSCP > 0
			"rdtscp; "
#else // fallback to mfence; rdtsc
			"mfence; rdtsc; "
#endif
			"shl $32,%%rdx; "
			"or %%rdx,%%rax "
			: "=a"(tsc)
			:
			: "%rcx", "%rdx");
	return tsc;
}

extern long double cycles_per_nanosec;

}} // namespace handystats::chrono

#endif // HANDYSTATS_CHRONO_TSC_IMPL_HPP_
