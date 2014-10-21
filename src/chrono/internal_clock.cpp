// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <cstdint>
#include <cassert>
#include <algorithm>
#include <ctime>
#include <unistd.h>

#include <handystats/chrono.hpp>

#include "cpuid_impl.hpp"

namespace handystats { namespace chrono {

inline uint64_t rdtsc() {
	uint64_t tsc;
	asm volatile (
			"mfence; rdtsc; "
			"shl $32,%%rdx; "
			"or %%rdx,%%rax "
			: "=a"(tsc)
			:
			: "%rcx", "%rdx");
	return tsc;
}

inline uint64_t rdtscp() {
	uint64_t tsc;
	asm volatile (
			"rdtscp; "
			"shl $32,%%rdx; "
			"or %%rdx,%%rax "
			: "=a"(tsc)
			:
			: "%rcx", "%rdx");
	return tsc;
}

long double cycles_per_nanosec;

}} // namespace handystats::chrono


namespace {

// Clock sources
enum clock_source_t {
	CS_RDTSC,
	CS_RDTSCP,
	CS_CLOCK_MONOTONIC,
	CS_CLOCK_REALTIME
};

clock_source_t get_available_clock_source() {
	if (handystats::tsc_supported() && handystats::invariant_tsc())
	{
		if (handystats::rdtscp_supported()) {
			return CS_RDTSCP;
		}
		else {
			return CS_RDTSC;
		}
	}
	else {
		// fallback to POSIX clocks
		if (sysconf(_SC_MONOTONIC_CLOCK) >= 0) {
			return CS_CLOCK_MONOTONIC;
		}
		else {
			return CS_CLOCK_REALTIME;
		}
	}
}

clock_source_t clock_source;

__attribute__((constructor(150)))
void init_clock_source() {
	clock_source = get_available_clock_source();
}


uint64_t get_nanoseconds() {
	timespec tm;

	int posix_clock;
	if (sysconf(_SC_MONOTONIC_CLOCK) >= 0) {
		posix_clock = CLOCK_MONOTONIC;
	}
	else {
		posix_clock = CLOCK_REALTIME;
	}

	clock_gettime(posix_clock, &tm);

	return (uint64_t)tm.tv_sec * (uint64_t)1E9 + tm.tv_nsec;
}

uint64_t get_cycles_count() {
	if (clock_source == CS_RDTSC) {
		return handystats::chrono::rdtsc();
	}
	else {
		return handystats::chrono::rdtscp();
	}
}


const uint64_t CYCLES_DELTA = 15000;

void get_simultaneous_pair(uint64_t* cycles_count, uint64_t* nanoseconds) {
	while(true) {
		uint64_t tsc1 = get_cycles_count();
		*nanoseconds = get_nanoseconds();
		uint64_t tsc2 = get_cycles_count();
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
	if (clock_source == CS_RDTSC || clock_source == CS_RDTSCP) {
		estimate_cycles_frequency();
	}
	else {
		handystats::chrono::cycles_per_nanosec = 3;
	}
}

} // unnamed namespace


namespace handystats { namespace chrono {

time_point internal_clock::now() {
	switch (clock_source) {
	case CS_RDTSC:
		{
			return time_point(duration(rdtsc(), time_unit::CYCLE), clock_type::INTERNAL_CLOCK);
		}
	case CS_RDTSCP:
		{
			return time_point(duration(rdtscp(), time_unit::CYCLE), clock_type::INTERNAL_CLOCK);
		}
	case CS_CLOCK_MONOTONIC:
		{
			timespec tm;

			clock_gettime(CLOCK_MONOTONIC, &tm);

			return time_point(duration((int64_t)tm.tv_sec * (int64_t)1E9 + tm.tv_nsec, time_unit::NSEC), clock_type::INTERNAL_CLOCK);
		}
	case CS_CLOCK_REALTIME:
		{
			timespec tm;

			clock_gettime(CLOCK_REALTIME, &tm);

			return time_point(duration((int64_t)tm.tv_sec * (int64_t)1E9 + tm.tv_nsec, time_unit::NSEC), clock_type::INTERNAL_CLOCK);
		}
	default:
		{
			return time_point();
		}
	}
}

}} // namespace handystats::chrono
