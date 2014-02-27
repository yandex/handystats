#include <thread>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include <handystats/chrono.hpp>
#include <handystats/operation.hpp>

#include "message_queue_impl.hpp"
#include "internal_impl.hpp"
#include "json_dump_impl.hpp"
#include "system_stats_impl.hpp"


namespace handystats {

std::mutex operation_mutex;
bool enabled = false;

std::thread* processor_thread = nullptr;

void process_message_queue() {
	auto processing_start_time = chrono::default_clock::now();
	auto message = message_queue::pop_event_message();

	if (message) {
		internal::process_event_message(*message);
		auto processing_end_time = chrono::default_clock::now();

		message_processing_time.set(std::chrono::duration_cast<chrono::default_duration>(processing_end_time - processing_start_time).count(), processing_end_time);
	}

	json::update_json_dump();
}


void initialize() {
	std::lock_guard<std::mutex> lock(operation_mutex);
	if (enabled) {
		return;
	}
	enabled = true;

	internal::initialize();
	message_queue::initialize();

	processor_thread =
		new std::thread([]
				() {
					while (enabled) {
						process_message_queue();
					}
				}
		);
}

void finalize() {
	std::lock_guard<std::mutex> lock(operation_mutex);
	if (!enabled) {
		return;
	}
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
}

} // namespace handystats


void HANDY_INIT() {
	handystats::initialize();
}

void HANDY_FINALIZE() {
	handystats::finalize();
}

