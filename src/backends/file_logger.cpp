// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <chrono>

#include <handystats/experimental/backends/file_logger.hpp>
#include <handystats/detail/serialization/rapidjson.hpp>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace handystats { namespace backends {

file_logger::file_logger(const std::string& filename, const chrono::duration& period)
	: m_filename(filename)
	, m_period(period)
{
	m_running.store(false);
}

file_logger::~file_logger() {
	stop();
}

void file_logger::stop() {
	m_running.store(false);

	if (m_thread.joinable()) {
		m_thread.join();
	}

	if (m_file.is_open()) {
		m_file.flush();
		m_file.close();
	}
}

static
void log_dump(std::ofstream& log, const chrono::time_point& timestamp, const metric_map& dump) {
	rapidjson::Value json_dump;
	rapidjson::MemoryPoolAllocator<> allocator;

	serialization::rapidjson::fill_value(json_dump, dump, allocator);

	const auto& dump_str =
		utils::rapidjson::to_string <
				rapidjson::Value, rapidjson::StringBuffer,
				rapidjson::Writer<rapidjson::StringBuffer>
			>
			(json_dump);

	log << timestamp.time_since_epoch().count() << " " << dump_str << "\n";
}

static
void run_file_logger(std::atomic<bool>& running_flag, std::ofstream& log, const chrono::duration& period) {
	chrono::time_point last_timestamp = chrono::system_clock::now();
	chrono::time_point last_dump_timestamp;

	while (running_flag.load()) {
		chrono::time_point current_time = chrono::system_clock::now();
		if (current_time - last_timestamp >= period) {
			auto dump = HANDY_METRICS_DUMP();
			auto dump_timestamp_iter = dump->second.find("handystats.internal_timestamp");
			if (dump_timestamp_iter == dump->second.end()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}
			chrono::time_point dump_timestamp(
					chrono::milliseconds(boost::get<int64_t>(dump_timestamp_iter->second.value())),
					chrono::clock_type::SYSTEM_CLOCK
				);
			if (dump_timestamp == last_dump_timestamp) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}

			last_timestamp = current_time;
			last_dump_timestamp = dump_timestamp;

			log_dump(log, dump_timestamp, dump->first);

			log.flush();
		}

		const auto& sleep_time = (period - (current_time - last_timestamp)).count(chrono::time_unit::MSEC);

		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
	}
}

bool file_logger::run() {
	if (m_running.load()) {
		// already running
		return false;
	}

	if (m_period.count() <= 0) {
		return false;
	}

	m_file.open(m_filename.c_str());

	if (m_file.fail()) {
		return false;
	}

	m_running.store(true);

	m_thread = std::thread(run_file_logger, std::ref(m_running), std::ref(m_file), m_period);

	return true;
}

}} // namespace handystats::backends

