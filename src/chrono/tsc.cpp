// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <cstdint>
#include <cassert>
#include <algorithm>
#include <ctime>

#include "chrono/tsc_impl.hpp"

namespace handystats { namespace chrono {

long double cycles_per_nanosec;

}} // namespace handystats::chrono

namespace {

uint64_t get_nanoseconds() {
	timespec tm;

#if defined(CLOCK_MONOTONIC_RAW)
	clock_gettime(CLOCK_MONOTONIC_RAW, &tm);
#elif defined(CLOCK_MONOTONIC)
	clock_gettime(CLOCK_MONOTONIC, &tm);
#elif defined(CLOCK_REALTIME)
	clock_gettime(CLOCK_REALTIME, &tm);
#else
	#error "No POSIX clocks are available."
#endif

	return (uint64_t)tm.tv_sec * (uint64_t)1E9 + tm.tv_nsec;
}

const uint64_t CYCLES_DELTA = 15000;

void get_simultaneous_pair(uint64_t* cycles_count, uint64_t* nanoseconds) {
	while(true) {
		uint64_t tsc1 = handystats::chrono::rdtsc();
		*nanoseconds = get_nanoseconds();
		uint64_t tsc2 = handystats::chrono::rdtsc();
		if (tsc2 - tsc1 < CYCLES_DELTA) {
			*cycles_count = tsc1 + (tsc2 - tsc1) / 2;
			break;
		}
	}
}

long double get_cycles_frequency(const timespec& sleep_interval) {
	uint64_t nanoseconds_start, cycles_start;
	uint64_t nanoseconds_end, cycles_end;

	get_simultaneous_pair(&cycles_start, &nanoseconds_start);

	assert(nanosleep(&sleep_interval, NULL) == 0);

	get_simultaneous_pair(&cycles_end, &nanoseconds_end);

	return (long double)(cycles_end - cycles_start) / (nanoseconds_end - nanoseconds_start);
}

void estimate_cycles_frequency() {
	const timespec sleep_interval = {0, (long)1E6}; // 1ms

	const int TESTS_COUNT = 15;
	long double cycles_tests[TESTS_COUNT];

	for (int test = 0; test < TESTS_COUNT; ++test) {
		cycles_tests[test] = get_cycles_frequency(sleep_interval);
	}

	std::sort(cycles_tests, cycles_tests + TESTS_COUNT);
	handystats::chrono::cycles_per_nanosec = cycles_tests[TESTS_COUNT / 2];
}

__attribute__((constructor(200)))
void init_cycles_per_nanosec() {
	estimate_cycles_frequency();
}

} // unnamed namespace
