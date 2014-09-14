// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <mutex>
#include <memory>
#include <string>
#include <map>

#include <handystats/chrono.hpp>
#include <handystats/json_dump.hpp>

#include "config_impl.hpp"

#include "metrics_dump_impl.hpp"

#include "json_dump_impl.hpp"


namespace handystats { namespace json_dump {


namespace stats {

metrics::gauge dump_time;

void initialize() {
	dump_time = metrics::gauge(config::metrics::gauge_opts);
}

void finalize() {
	dump_time = metrics::gauge(config::metrics::gauge_opts);
}

} // namespace stats


std::mutex dump_mutex;
std::shared_ptr<const std::string> dump(new std::string());

const std::shared_ptr<const std::string>
get_dump() {
	std::lock_guard<std::mutex> lock(dump_mutex);
	return dump;
}

template<typename Allocator>
std::shared_ptr<const std::string> create_dump(Allocator&& allocator = Allocator()) {
	return std::shared_ptr<const std::string>(
			new std::string(to_string(*metrics_dump::get_dump(), allocator))
		);
}

void update() {
	if (!config::metrics_dump_opts.to_json) {
		return;
	}

	auto dump_start_time = chrono::clock::now();
	auto new_dump = create_dump<rapidjson::MemoryPoolAllocator<>>();
	auto dump_end_time = chrono::clock::now();

	stats::dump_time.set(
			chrono::duration_cast<chrono::time_duration>(dump_end_time - dump_start_time).count(),
			dump_end_time
		);

	{
		std::lock_guard<std::mutex> lock(dump_mutex);
		dump = new_dump;
	}
}

void initialize() {
	stats::initialize();

	{
		std::lock_guard<std::mutex> lock(dump_mutex);

		dump = std::shared_ptr<const std::string>(new std::string());
	}
}

void finalize() {
	stats::finalize();

	{
		std::lock_guard<std::mutex> lock(dump_mutex);

		dump = std::shared_ptr<const std::string>(new std::string());
	}
}

}} // namespace handystats::json_dump


const std::shared_ptr<const std::string> HANDY_JSON_DUMP() {
	return handystats::json_dump::get_dump();
}

