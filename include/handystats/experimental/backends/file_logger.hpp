// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_BACKENDS_FILE_LOGGER_HPP_
#define HANDYSTATS_BACKENDS_FILE_LOGGER_HPP_

#include <fstream>
#include <thread>
#include <string>

#include <handystats/atomic.hpp>
#include <handystats/chrono.hpp>
#include <handystats/metrics_dump.hpp>

namespace handystats { namespace backends {

struct file_logger {
	file_logger(const std::string& filename, const chrono::duration& period);
	~file_logger();

	bool run();
	void stop();

	std::string m_filename;
	std::ofstream m_file;
	chrono::duration m_period;

	std::atomic<bool> m_running;
	std::thread m_thread;
};

}} // namespace handystats::backends

#endif // HANDYSTATS_BACKENDS_FILE_LOGGER_HPP_
