/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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

	std::string m_error;
};

}} // namespace handystats::backends

#endif // HANDYSTATS_BACKENDS_FILE_LOGGER_HPP_
