// Copyright (c) 2014 Yandex LLC. All rights reserved.

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
