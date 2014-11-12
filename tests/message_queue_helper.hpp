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

#ifndef HANDYSTATS_TESTS_MESSAGE_QUEUE_HELPER_HPP_
#define HANDYSTATS_TESTS_MESSAGE_QUEUE_HELPER_HPP_

#include <thread>
#include <chrono>

#include "core_impl.hpp"

namespace handystats {

void wait_until_empty() {
	while (core && core->m_channel.size() > 0) {
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}

} // namespace handystats

#endif // HANDYSTATS_TESTS_MESSAGE_QUEUE_HELPER_HPP_
