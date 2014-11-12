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

#ifndef HANDYSTATS_SERIALIZATION_JSON_IMPL_HPP_
#define HANDYSTATS_SERIALIZATION_JSON_IMPL_HPP_

#include <string>

#include <handystats/detail/statistics.hpp>

namespace handystats { namespace serialization { namespace json {

template <typename Object>
Object loads(const std::string&);

template <typename Object>
std::string dumps(const Object&);

// specializations for statistics::data
template <>
statistics::data loads<statistics::data>(const std::string& dump);
template <>
std::string dumps<statistics::data>(const statistics::data& data);

}}} // namespace handystats::serialization::json

#endif // HANDYSTATS_SERIALIZATION_JSON_IMPL_HPP_
