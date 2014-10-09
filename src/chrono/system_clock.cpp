// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <ctime>

#include <handystats/chrono.hpp>

namespace handystats { namespace chrono {

time_point system_clock::now() {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	return time_point(duration(ts.tv_sec * 1000ull * 1000ull * 1000ull + ts.tv_nsec, time_unit::NSEC), clock_type::SYSTEM);
}

}} // namespace handystats::chrono
