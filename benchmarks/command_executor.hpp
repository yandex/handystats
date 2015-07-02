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

#ifndef HANDYSTATS_BENCHMARKS_COMMAND_EXECUTOR_HPP_
#define HANDYSTATS_BENCHMARKS_COMMAND_EXECUTOR_HPP_

#include <functional>
#include <chrono>

#include <handystats/metrics/gauge.hpp>
#include <handystats/chrono.hpp>

namespace handystats { namespace benchmarks {

struct command_executor {
	typedef std::function<void()> command_type;

	static
	chrono::duration
	run_command(
			command_type command
		)
	{
		auto start_time = chrono::tsc_clock::now();
		command();
		auto end_time = chrono::tsc_clock::now();

		return end_time - start_time;
	}

	static
	chrono::duration
	run_command_for(
			command_type command,
			chrono::duration time_limit
		)
	{
		auto start_time = chrono::tsc_clock::now();
		auto call_time = run_command(command);

		while (call_time < time_limit) {
			if (time_limit - call_time > chrono::duration(100, chrono::time_unit::USEC)) {
				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}
			else {
				std::this_thread::yield();
			}
			call_time = chrono::tsc_clock::now() - start_time;
		}

		return call_time;
	}

	static
	chrono::duration
	run_for(
			command_type command,
			chrono::duration command_time_limit,
			chrono::duration total_time_limit
		)
	{
		chrono::duration total_time(0, chrono::time_unit::TICK);

		while (total_time < total_time_limit) {
			total_time += run_command_for(command, command_time_limit);
		}

		return total_time;
	}

}; // struct command_executor

}} // namespace handystats::benchmarks

#endif // HANDYSTATS_BENCHMARKS_COMMAND_EXECUTOR_HPP_
