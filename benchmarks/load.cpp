// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <vector>

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

	if (!metrics_dump || metrics_dump->empty()) {
		return;
	}

	std::cout << "============= "
		<< " RPS: " << rate.load() << " "
		<< " ETR: " <<
			handystats::chrono::duration_cast<std::chrono::seconds>(
				handystats::chrono::clock::duration(end_time.load()) - handystats::chrono::clock::now().time_since_epoch()
			)
			.count()
			<< "s "
		<< " =============" << std::endl;
	std::cout << "timestamp: "
		<< boost::get<int64_t>(
				boost::get<handystats::metrics::attribute>(metrics_dump->at("handystats.dump_timestamp")).value()
			) / 1000
		<< std::endl
		;
	std::cout << std::endl;

	{
		const char* name = "handystats.message_queue.size";
		const auto& message_queue_size = boost::get<handystats::metrics::gauge>(metrics_dump->at(name));
		std::cout << name << ":" << std::endl;
		std::cout << "         value: " << message_queue_size.values().get<handystats::statistics::tag::value>() << std::endl;
		std::cout << "    moving-avg: " << message_queue_size.values().get<handystats::statistics::tag::moving_avg>() << std::endl;
//		std::cout << "     timestamp: " << message_queue_size.values().get<handystats::statistics::tag::timestamp>() << std::endl;
		std::cout << std::endl;
	}

	{
		const char* name = "handystats.message_queue.pop_count";
		const auto& pop_count = boost::get<handystats::metrics::counter>(metrics_dump->at(name));
		std::cout << name << ":" << std::endl;
		std::cout << "          rate: " << pop_count.values().get<handystats::statistics::tag::rate>() << std::endl;
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
					auto start_time = handystats::chrono::clock::now();
					print_stats();
					auto end_time = handystats::chrono::clock::now();

					auto call_time = handystats::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
					std::this_thread::sleep_for(output_interval - call_time);
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

		if (step_rate == 0) {
			rate.store(0);
			end_time.store(
					(
						handystats::chrono::clock::now() +
						handystats::chrono::duration_cast<handystats::chrono::clock::duration>(
							std::chrono::seconds(step_time_limit)
						)
					)
					.time_since_epoch().count()
				);

			std::this_thread::sleep_for(std::chrono::seconds(step_time_limit));
			continue;
		}

		std::chrono::nanoseconds command_time_limit =
			std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) * threads / step_rate;
		std::chrono::nanoseconds time_limit =
			std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(step_time_limit));

		rate.store(step_rate);
		end_time.store(
				(
					handystats::chrono::clock::now() +
					handystats::chrono::duration_cast<handystats::chrono::clock::duration>(
						std::chrono::seconds(step_time_limit)
					)
				)
				.time_since_epoch().count()
			);

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
