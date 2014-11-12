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

#include <handystats/metrics_dump.hpp>
#include <handystats/json_dump.hpp>

#include <handystats/utils/rapidjson_writer.hpp>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

using namespace rapidjson;

std::string HANDY_JSON_DUMP() {
	Value json_dump;
	MemoryPoolAllocator<> allocator;

	handystats::utils::rapidjson::fill_value(json_dump, *HANDY_METRICS_DUMP(), allocator);

	return handystats::utils::rapidjson::to_string<Value, StringBuffer, PrettyWriter<StringBuffer>>(json_dump);
}

