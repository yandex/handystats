#include "handystats/core.hpp"

#include "handystats/message_queue.hpp"
#include "handystats/internal.hpp"

namespace handystats {

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

	events::event_message* message = message_queue::pop_event_message();
	internal::process_event_message(message);
	events::delete_event_message(message);
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

