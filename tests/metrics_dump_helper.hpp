// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_TESTS_METRICS_DUMP_HELPER_HPP_
#define HANDYSTATS_TESTS_METRICS_DUMP_HELPER_HPP_

#include <thread>
#include <chrono>

#include <handystats/metrics/attribute.hpp>
#include <handystats/chrono.hpp>
#include "metrics_dump_impl.hpp"

namespace handystats { namespace metrics_dump {

struct dump_timestamp_visitor : public boost::static_visitor<int64_t>
{
	int64_t operator() (const int& value) const {
		return value;
	}
	int64_t operator() (const unsigned& value) const {
		return value;
	}
	int64_t operator() (const int64_t& value) const {
		return value;
	}
	int64_t operator() (const uint64_t& value) const {
		return value;
	}
	int64_t operator() (const double& value) const {
		return value;
	}
	int64_t operator() (const bool& value) const {
		return 0;
	}
	int64_t operator() (const std::string& value) const {
		return 0;
	}
};

void wait_until(const chrono::time_point& timestamp) {
	const char* timestamp_name = "handystats.internal_timestamp";
	while (true) {
		const auto& dump = get_dump();
		const auto& attributes = dump->second;
		if (attributes.find(timestamp_name) != attributes.cend()) {
			dump_timestamp_visitor visitor;
			const auto& dump_timestamp_ms =
				boost::apply_visitor(
						visitor,
						attributes.at(timestamp_name).value()
					);

			chrono::time_point dump_timestamp(chrono::duration(dump_timestamp_ms, chrono::time_unit::MSEC), chrono::clock_type::SYSTEM);

			if (dump_timestamp >= timestamp) {
				return;
			}
		}

		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

}} // namespace handystats::metrics_dump

#endif // HANDYSTATS_TESTS_METRICS_DUMP_HELPER_HPP_
