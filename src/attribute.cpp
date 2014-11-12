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

#include <handystats/attribute.hpp>


namespace handystats {

attribute::attribute()
	: m_value()
{
}

void attribute::set(const attribute::value_type& value) {
	m_value = value;
}

void attribute::set(const bool& b) {
	m_value = b;
}

void attribute::set(const int& i) {
	m_value = i;
}

void attribute::set(const unsigned& u) {
	m_value = u;
}

void attribute::set(const int64_t& i64) {
	m_value = i64;
}

void attribute::set(const uint64_t& u64) {
	m_value = u64;
}

void attribute::set(const double& d) {
	m_value = d;
}

void attribute::set(const char* s) {
	set(std::string(s));
}

void attribute::set(const std::string& s) {
	m_value = s;
}

const attribute::value_type& attribute::value() const {
	return m_value;
}

} // namespace handystats
