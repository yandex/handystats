// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <thread>
#include <sys/prctl.h>
#include <handystats/atomic.hpp>

#include <handystats/chrono.hpp>
#include <handystats/operation.hpp>
#include <handystats/operation.h>

#include "events/event_message_impl.hpp"
#include "message_queue_impl.hpp"
#include "internal_impl.hpp"
#include "metrics_dump_impl.hpp"
#include "config_impl.hpp"

#include "core_impl.hpp"

namespace handystats {

std::mutex operation_mutex;
std::atomic<bool> enabled_flag(false);

bool is_enabled() {
	return config::core_opts.enable && enabled_flag.load(std::memory_order_acquire);
}


std::thread processor_thread;

void process_message_queue() {
	auto* message = message_queue::pop();

	if (message) {
		internal::process_event_message(*message);
	}

	events::delete_event_message(message);
}

static void run_processor() {
	char thread_name[16];
	memset(thread_name, 0, sizeof(thread_name));

	sprintf(thread_name, "handystats");

	prctl(PR_SET_NAME, thread_name);

	while (is_enabled()) {
		if (!message_queue::empty()) {
			process_message_queue();
		}
		else {
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}

		metrics_dump::update();
	}
}

void initialize() {
	std::lock_guard<std::mutex> lock(operation_mutex);
	if (enabled_flag.load(std::memory_order_acquire)) {
		return;
	}

	if (!config::default_initialized) {
		config::initialize();
	}

	metrics_dump::initialize();
	internal::initialize();
	message_queue::initialize();

	if (!config::core_opts.enable) {
		return;
	}

	enabled_flag.store(true, std::memory_order_release);

	processor_thread = std::thread(run_processor);
}

void finalize() {
	if (!config::core_opts.enable) {
		return;
	}

	std::lock_guard<std::mutex> lock(operation_mutex);
	enabled_flag.store(false, std::memory_order_release);

	if (processor_thread.joinable()) {
		processor_thread.join();
	}

	internal::finalize();
	message_queue::finalize();
	metrics_dump::finalize();
	config::finalize();
}

} // namespace handystats


extern "C" {

void handystats_initialize() {
	handystats::initialize();
}

void handystats_finalize() {
	handystats::finalize();
}

} // extern "C"
