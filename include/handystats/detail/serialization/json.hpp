// Copyright (c) 2014 Yandex LLC. All rights reserved.

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
