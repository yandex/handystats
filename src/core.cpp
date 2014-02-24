#include "handystats/core.hpp"

#include <handystats/metrics/gauge.hpp>
#include <handystats/message_queue_impl.hpp>
#include <handystats/internal_impl.hpp>
#include <handystats/json_dump_impl.hpp>

namespace handystats {

metrics::gauge event_message_queue_size;
metrics::gauge monitors_size;
metrics::gauge message_processing_time;
metrics::gauge message_push_time;
metrics::gauge message_pop_time;
metrics::gauge message_delete_time;

bool handy_enabled = false;

std::thread* processor_thread = nullptr;

void process_message_queue() {
	if (!message_queue::event_message_queue) {
		return;
	}

	auto processing_start_time = chrono::default_clock::now();
	events::event_message* message = message_queue::pop_event_message();
	auto pop_end_time = chrono::default_clock::now();
	internal::process_event_message(message);
	auto delete_start_time = chrono::default_clock::now();
	events::delete_event_message(message);
	auto processing_end_time = chrono::default_clock::now();

	if (message) {
		auto timestamp = chrono::default_clock::now();

		message_processing_time.set(std::chrono::duration_cast<chrono::default_duration>(processing_end_time - processing_start_time).count(), timestamp);
		message_pop_time.set(std::chrono::duration_cast<chrono::default_duration>(pop_end_time - processing_start_time).count(), timestamp);
		message_delete_time.set(std::chrono::duration_cast<chrono::default_duration>(processing_end_time - delete_start_time).count(), timestamp);
		event_message_queue_size.set(message_queue::event_message_queue->unsafe_size(), timestamp);
		monitors_size.set(internal::monitors.size(), timestamp);
	}

	json::update_json_dump();
}


void initialize() {
	internal::initialize();
	message_queue::initialize();

	processor_thread =
		new std::thread([]
				() {
					while (handy_enabled) {
						process_message_queue();
					}
				}
		);
}

void clean_up() {
	if (!processor_thread) {
		return;
	}

	if (processor_thread->joinable()) {
		processor_thread->join();
	}

	delete processor_thread;
	processor_thread = nullptr;

	internal::clean_up();
	message_queue::clean_up();
}


void enable_handy() {
	handy_enabled = true;

	initialize();
}

void disable_handy() {
	handy_enabled = false;

	clean_up();
}

} // namespace handystats


void HANDY_ENABLE() {
	if (!handystats::handy_enabled) {
		handystats::enable_handy();
	}
}

void HANDY_DISABLE() {
	if (handystats::handy_enabled) {
		handystats::disable_handy();
	}
}

