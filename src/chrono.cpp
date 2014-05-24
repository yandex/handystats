#include <ctime>
#include <algorithm>
#include <cassert>

#include <handystats/chrono.hpp>


namespace handystats { namespace chrono {

long double cycles_per_nanosec;

static inline uint64_t rdtsc() {
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

static inline uint64_t get_nanoseconds() noexcept {
	timespec tm;
	clock_gettime(CLOCK_MONOTONIC_RAW, &tm);
	return (uint64_t)tm.tv_sec * (uint64_t)1E9 + tm.tv_nsec;
}

const uint64_t NANOSEC_DELTA = 5000;

long double get_cycles_frequency(const timespec& sleep_interval) {
	uint64_t cycles_start;
	uint64_t nanoseconds_start;
	while(true) {
		uint64_t ns1 = get_nanoseconds();
		cycles_start = rdtsc();
		uint64_t ns2 = get_nanoseconds();
		if (ns2 - ns1 < NANOSEC_DELTA) {
			nanoseconds_start = ns1 + (ns2 - ns1) / 2;
			break;
		}
	}

	assert(nanosleep(&sleep_interval, NULL) == 0);

	uint64_t cycles_end;
	uint64_t nanoseconds_end;
	while(true) {
		uint64_t ns1 = get_nanoseconds();
		cycles_end = rdtsc();
		uint64_t ns2 = get_nanoseconds();
		if (ns2 - ns1 < NANOSEC_DELTA) {
			nanoseconds_end = ns1 + (ns2 - ns1) / 2;
			break;
		}
	}

	return (long double)(cycles_end - cycles_start) / (nanoseconds_end - nanoseconds_start);
}


/*
 * Cycles frequency = num. of cycles / 1ns
 */
__attribute__((constructor))
void estimate_cycles_frequency() {
	const timespec sleep_interval = {0, (long)1E6}; // 1ms

	const int TESTS_COUNT = 15;
	long double cycles_tests[TESTS_COUNT];

	for (int test = 0; test < TESTS_COUNT; ++test) {
		cycles_tests[test] = get_cycles_frequency(sleep_interval);
	}

	std::sort(cycles_tests, cycles_tests + TESTS_COUNT);
	cycles_per_nanosec = cycles_tests[TESTS_COUNT / 2];
}


tsc_clock::time_point tsc_clock::now() noexcept {
	return rdtsc();
}

std::chrono::nanoseconds tsc_clock::to_nanoseconds(const duration& cycles_count) noexcept {
	return std::chrono::nanoseconds(uint64_t(cycles_count / cycles_per_nanosec));
}

tsc_clock::duration tsc_clock::from_nanoseconds(const std::chrono::nanoseconds& nanoseconds) noexcept {
	return tsc_clock::duration(uint64_t(nanoseconds.count() * cycles_per_nanosec));
}

}} // namespace handystats::chrono
