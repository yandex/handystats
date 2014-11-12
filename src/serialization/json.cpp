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

#include <stdexcept>

#include <handystats/utils/rapidjson_writer.hpp>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <handystats/detail/serialization/json.hpp>
#include <handystats/detail/serialization/rapidjson.hpp>

using namespace rapidjson;

namespace handystats { namespace serialization { namespace json {

template <>
statistics::data loads<statistics::data>(const std::string& dump) {
	Document dump_value;
	dump_value.Parse<0>(dump.c_str());
	if (dump_value.HasParseError()) {
		throw std::invalid_argument(dump_value.GetParseError());
	}

	statistics::data data;
	rapidjson::load_value(data, dump_value);

	return data;
}

template <>
std::string dumps<statistics::data>(const statistics::data& data) {
	Value json_dump;
	MemoryPoolAllocator<> allocator;

	rapidjson::fill_value(json_dump, data, allocator);

	return utils::rapidjson::to_string <
				Value, StringBuffer,
				Writer<StringBuffer>
			>
			(json_dump);
}

}}} // namespace handystats::serialization::json
