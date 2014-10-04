// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <vector>
#include <chrono>

#include <boost/program_options.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#include <handystats/math_utils.hpp>
#include <handystats/core.hpp>
#include <handystats/metrics_dump.hpp>
#include <handystats/measuring_points.hpp>

#include <handystats/module.h>

#include "command_executor.hpp"

#ifndef _HAVE_HANDY_MODULE_LOAD
#define _HAVE_HANDY_MODULE_LOAD 1
#endif

HANDY_MODULE(LOAD)

uint64_t threads = 1;
uint64_t timers = 0;
uint64_t counters = 0;
uint64_t gauges = 0;
std::vector<uint64_t> steps;
std::chrono::milliseconds output_interval = std::chrono::seconds(1);

std::atomic<uint64_t> rate(0);
std::atomic<int64_t> end_time(0);

// command() function is executed in multiple threads
// NOTE: rand() function may be not thread-safe.
void command() {
	// 0 .. counters .. counters + gauges .. counters + gauges + timers
	const double total_metrics = counters + gauges + timers;

	double counter_prob = double(counters) / total_metrics;
	double gauge_prob = double(counters + gauges) / total_metrics;
//	double timer_prob = 1.0;

	double choice = double(rand()) / RAND_MAX;

	if (handystats::math_utils::cmp(choice, counter_prob) < 0) {
		LOAD_COUNTER_INCREMENT("load_test.counter." + std::to_string(rand() % counters), rand());
	}
	else if (handystats::math_utils::cmp(choice, gauge_prob) < 0) {
		LOAD_GAUGE_SET("load_test.gauge." + std::to_string(rand() % gauges), rand());
	}
	else {
		if (rand() & 1) {
			LOAD_TIMER_START("load_test.timer." + std::to_string(rand() % timers), std::hash<std::thread::id>()(std::this_thread::get_id()));
		}
		else {
			LOAD_TIMER_STOP("load_test.timer." + std::to_string(rand() % timers), std::hash<std::thread::id>()(std::this_thread::get_id()));
		}
	}
}

void print_stats() {
	auto metrics_dump = HANDY_METRICS_DUMP();

	if (!metrics_dump || metrics_dump->first.empty() || metrics_dump->second.empty()) {
		return;
	}

	std::cout << "============= "
		<< " RPS: " << rate.load() << " "
		<< " ETR: " <<
			handystats::chrono::duration::convert_to(handystats::chrono::time_unit::SEC,
				handystats::chrono::duration(end_time.load(), handystats::chrono::time_unit::TICK) -
				handystats::chrono::tsc_clock::now().time_since_epoch()
			)
			.count()
			<< "s "
		<< " =============" << std::endl;
	std::cout << "timestamp: "
		<< boost::get<int64_t>(
				metrics_dump->second.at("handystats.dump_timestamp").value()
			) / 1000
		<< std::endl
		;
	std::cout << std::endl;

	{
		const char* name = "handystats.message_queue.size";
		const auto& message_queue_size = boost::get<handystats::metrics::gauge>(metrics_dump->first.at(name));
		std::cout << name << ":" << std::endl;
		std::cout << "         value: " << message_queue_size.values().get<handystats::statistics::tag::value>() << std::endl;
		std::cout << "    moving-avg: " << message_queue_size.values().get<handystats::statistics::tag::moving_avg>() << std::endl;
//		std::cout << "     timestamp: " << message_queue_size.values().get<handystats::statistics::tag::timestamp>() << std::endl;
		std::cout << std::endl;
	}

	{
		const char* name = "handystats.message_queue.pop_count";
		const auto& pop_count = boost::get<handystats::metrics::counter>(metrics_dump->first.at(name));
		std::cout << name << ":" << std::endl;
		std::cout << "          rate: " << pop_count.values().get<handystats::statistics::tag::rate>() << std::endl;
//		std::cout << "     timestamp: " << pop_count.incr_deltas().get<handystats::statistics::tag::timestamp>() << std::endl;
		std::cout << std::endl;
	}

	{
		const char* name = "load_test.timer.0";
		const auto& timer = boost::get<handystats::metrics::timer>(metrics_dump->first.at(name));
		std::cout << name << ":" << std::endl;
		std::cout << "          p25: " << timer.values().get<handystats::statistics::tag::quantile>().at(0.25) << std::endl;
		std::cout << "          p50: " << timer.values().get<handystats::statistics::tag::quantile>().at(0.50) << std::endl;
		std::cout << "          p75: " << timer.values().get<handystats::statistics::tag::quantile>().at(0.75) << std::endl;
		std::cout << "          p95: " << timer.values().get<handystats::statistics::tag::quantile>().at(0.95) << std::endl;
//		std::cout << "     timestamp: " << pop_count.incr_deltas().get<handystats::statistics::tag::timestamp>() << std::endl;
		std::cout << std::endl;
	}
}

int main(int argc, char** argv) {
	namespace po = boost::program_options;
	po::options_description desc("Options");
	desc.add_options()
		("help", "Print help messages")
		("handystats-config", po::value<std::string>(),
			"Handystats configuration (in JSON format)"
		)
		("threads", po::value<uint64_t>(&threads)->default_value(threads),
			"Number of worker threads"
		)
		("step", po::value<std::vector<uint64_t>>(&steps)->multitoken()->required(),
			"Load step pairs (rate, seconds), e.g. --step 100 10 --step 200 10"
		)
		("timers", po::value<uint64_t>(&timers)->default_value(timers),
			"Number of different timers (0 for no timers)"
		)
		("counters", po::value<uint64_t>(&counters)->default_value(counters),
			"Number of different counters (0 for no counters)"
		)
		("gauges", po::value<uint64_t>(&gauges)->default_value(gauges),
			"Number of different gauges (0 for no gauges)"
		)
		("output-interval", po::value<uint64_t>()->default_value(output_interval.count()),
			"Stats output interval (in milliseconds)"
		)
	;

	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
		if (vm.count("help")) {
			std::cout << desc << std::endl;
			return 0;
		}
		po::notify(vm);
	}
	catch(po::error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		std::cerr << desc << std::endl;
		return 1;
	}

	if (vm["threads"].as<uint64_t>() == 0) {
		std::cerr << "ERROR: number of threads must be greater than 0" << std::endl;
		return 1;
	}

	if (!vm["timers"].as<uint64_t>() && !vm["counters"].as<uint64_t>() && !vm["gauges"].as<uint64_t>()) {
		std::cerr << "ERROR: at least one metric type must be specified (timers, counters, gauges)" << std::endl;
		return 1;
	}

	if (vm.count("handystats-config")) {
		HANDY_CONFIG_JSON(vm["handystats-config"].as<std::string>().c_str());
	}

	output_interval = std::chrono::milliseconds(vm["output-interval"].as<uint64_t>());

	HANDY_INIT();

	std::atomic<bool> stop_flag(false);
	std::thread stats_printer(
			[&stop_flag] () {
				while (!stop_flag.load()) {
					const auto& start_time = handystats::chrono::tsc_clock::now();
					print_stats();
					const auto& end_time = handystats::chrono::tsc_clock::now();

					const auto& call_time = handystats::chrono::duration::convert_to(
							handystats::chrono::time_unit::NSEC, end_time - start_time
						);
					std::this_thread::sleep_for(output_interval - std::chrono::nanoseconds(call_time.count()));
				}
			}
		);

	std::vector<std::thread> workers(threads);
	for (size_t step_index = 0; step_index < steps.size(); step_index += 2) {
		uint64_t step_rate = steps[step_index];
		uint64_t step_time_limit = steps[step_index + 1];

		if (step_time_limit == 0) {
			continue;
		}

		rate.store(step_rate);
		end_time.store(
				(
					handystats::chrono::tsc_clock::now() +
					handystats::chrono::duration(step_time_limit, handystats::chrono::time_unit::SEC)
				).time_since_epoch().count()
			);

		if (step_rate == 0) {
			std::this_thread::sleep_for(std::chrono::seconds(step_time_limit));
			continue;
		}

		handystats::chrono::duration command_time_limit =
			handystats::chrono::duration::convert_to(
					handystats::chrono::time_unit::NSEC,
					handystats::chrono::duration(1, handystats::chrono::time_unit::SEC)
				) * threads / step_rate;
		handystats::chrono::duration time_limit(step_time_limit, handystats::chrono::time_unit::SEC);

		for (auto& worker : workers) {
			worker = std::thread(
					[command_time_limit, time_limit]
					() {
						handystats::benchmarks::command_executor::run_for(
							[] () {
								command();
							},
							command_time_limit,
							time_limit
						);
					}
				);
		}

		for (auto& worker : workers) {
			worker.join();
		}
	}

	stop_flag.store(true);
	stats_printer.join();

	HANDY_FINALIZE();
}
