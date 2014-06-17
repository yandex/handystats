// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <thread>
#include <mutex>

#include <handystats/chrono.hpp>
#include <handystats/operation.hpp>

#include "message_queue_impl.hpp"
#include "internal_impl.hpp"
#include "json_dump_impl.hpp"
#include "metrics_dump_impl.hpp"
#include "system_stats_impl.hpp"
#include "configuration_impl.hpp"
#include "core_impl.hpp"

namespace handystats {

std::mutex operation_mutex;
bool enabled = false;

bool is_enabled() {
	return enabled;
}


std::thread* processor_thread = nullptr;

void process_message_queue() {
	auto pop_start_time = chrono::default_clock::now();
	auto message = message_queue::pop_event_message();
	auto pop_end_time = chrono::default_clock::now();

	system_stats::message_pop_time.set(std::chrono::duration_cast<chrono::default_duration>(pop_end_time - pop_start_time).count(), pop_end_time);
	system_stats::message_queue_size.set(message_queue::size(), pop_end_time);

	if (message) {
		auto processing_start_time = chrono::default_clock::now();
		internal::process_event_message(*message);
		auto processing_end_time = chrono::default_clock::now();

		system_stats::message_process_time.set(std::chrono::duration_cast<chrono::default_duration>(processing_end_time - processing_start_time).count(),
				processing_end_time);
		system_stats::internal_metrics_size.set(internal::size(), processing_end_time);
	}
}


void initialize() {
	std::lock_guard<std::mutex> lock(operation_mutex);
	if (is_enabled()) {
		return;
	}
	enabled = true;

	config::initialize();
	internal::initialize();
	message_queue::initialize();
	system_stats::initialize();

	processor_thread =
		new std::thread([]
				() {
					size_t sleep_interval_index = 0;
					while (is_enabled()) {
						if (!message_queue::empty()) {
							process_message_queue();
							sleep_interval_index = 0;
						}
						else {
							std::this_thread::sleep_for(config::message_queue.sleep_on_empty[sleep_interval_index]);
							if (sleep_interval_index + 1 < config::message_queue.sleep_on_empty.size()) {
								++sleep_interval_index;
							}
						}
						json::update_json_dump();
						metrics::update_metrics_dump();
					}
				}
		);
}

void finalize() {
	std::lock_guard<std::mutex> lock(operation_mutex);
	enabled = false;

	if (processor_thread) {
		if (processor_thread->joinable()) {
			processor_thread->join();
		}

		delete processor_thread;
		processor_thread = nullptr;
	}

	internal::finalize();
	message_queue::finalize();
	system_stats::finalize();
	config::finalize();
}

} // namespace handystats


void HANDY_INIT() {
	handystats::initialize();
}

void HANDY_FINALIZE() {
	handystats::finalize();
}

