#include <thread>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include <tbb/concurrent_queue.h>

#include <handystats/operation.hpp>

#include <handystats/metrics/gauge.hpp>

#include "events/event_message_impl.hpp"
#include "internal_metrics_impl.hpp"


namespace handystats { namespace message_queue {

extern tbb::concurrent_queue<std::shared_ptr<events::event_message>>* event_message_queue;

std::shared_ptr<events::event_message> pop_event_message();

void initialize();
void finalize();

}} // namespace handystats::message_queue


namespace handystats { namespace json {

void update_json_dump();

}} // namespace handystats::json


namespace handystats { namespace internal {

extern metrics::gauge message_processing_time;
extern metrics::gauge internal_metrics_size;

}} // namespace handystats::internal


namespace handystats { namespace internal {

extern std::unordered_map<std::string, internal_metric> internal_metrics;

void process_event_message(const events::event_message&);

void initialize();
void finalize();

}} // namespace handystats::internal


namespace handystats {

std::mutex operation_mutex;
bool enabled = false;

std::thread* processor_thread = nullptr;

void process_message_queue() {
	if (!message_queue::event_message_queue) {
		return;
	}

	auto processing_start_time = chrono::default_clock::now();
	auto message = message_queue::pop_event_message();

	if (message) {
		internal::process_event_message(*message);
		auto processing_end_time = chrono::default_clock::now();

		internal::message_processing_time.set(std::chrono::duration_cast<chrono::default_duration>(processing_end_time - processing_start_time).count(), processing_end_time);
		internal::internal_metrics_size.set(internal::internal_metrics.size(), processing_end_time);
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

