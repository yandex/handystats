/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

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
	while (true) {
		const auto& dump = get_dump();
		if (dump->find("handystats.dump_timestamp") != dump->cend()) {
			dump_timestamp_visitor visitor;
			const auto& dump_timestamp_ms =
				boost::apply_visitor(
						visitor,
						boost::get<handystats::metrics::attribute>(dump->at("handystats.dump_timestamp")).value()
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
