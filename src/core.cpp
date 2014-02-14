#include "handystats/core.hpp"

namespace handystats {

metrics::gauge event_message_queue_size;
metrics::gauge monitors_size;
metrics::gauge message_processing_time;

bool handy_enabled = false;

void enable_handy() {
	handy_enabled = true;

	initialize();
}

void disable_handy() {
	handy_enabled = false;

	clean_up();
}


std::thread* processor_thread = nullptr;

void process_message_queue() {
	if (!message_queue::event_message_queue) {
		return;
	}

	auto processing_start_time = chrono::default_clock::now();
	events::event_message* message = message_queue::pop_event_message();
	internal::process_event_message(message);
	events::delete_event_message(message);
	auto processing_end_time = chrono::default_clock::now();

	if (message) {
		auto processing_duration = processing_end_time - processing_start_time;
		auto timestamp = chrono::default_clock::now();

		message_processing_time.set(processing_duration.count(), timestamp);
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
					while (is_handy_enabled()) {
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

} // namespace handystats

