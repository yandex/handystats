// Copyright (c) 2014 Yandex LLC. All rights reserved.

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
	std::chrono::nanoseconds
	run_command(
			command_type command
		)
	{
		auto start_time = chrono::clock::now();
		command();
		auto end_time = chrono::clock::now();

		return chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
	}

	static
	std::chrono::nanoseconds
	run_command_for(
			command_type command,
			std::chrono::nanoseconds time_limit
		)
	{
		auto start_time = chrono::clock::now();
		auto call_time = run_command(command);

		while (call_time < time_limit) {
			if (time_limit - call_time > std::chrono::microseconds(100)) {
				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}
			else {
				std::this_thread::yield();
			}
			call_time = chrono::duration_cast<std::chrono::nanoseconds>(chrono::clock::now() - start_time);
		}

		return call_time;
	}

	static
	std::chrono::nanoseconds
	run_for(
			command_type command,
			std::chrono::nanoseconds command_time_limit,
			std::chrono::nanoseconds total_time_limit
		)
	{
		std::chrono::nanoseconds total_time(0);

		while (total_time < total_time_limit) {
			total_time += run_command_for(command, command_time_limit);
		}

		return total_time;
	}

}; // struct command_executor

}} // namespace handystats::benchmarks

#endif // HANDYSTATS_BENCHMARKS_COMMAND_EXECUTOR_HPP_
