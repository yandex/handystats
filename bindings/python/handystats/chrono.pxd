# distutils: language = c++

# Copyright (c) 2014 Yandex LLC. All rights reserved.

# This file is part of Handystats.

# Handystats is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.

# Handystats is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

from libc.stdint cimport int64_t
from libcpp cimport bool

cdef extern from "handystats/chrono.hpp" namespace "handystats::chrono":
    cpdef enum time_unit:
        CYCLE
        NSEC
        USEC
        MSEC
        SEC
        MIN
        HOUR
        DAY

    cpdef enum clock_type:
        INTERNAL_CLOCK
        SYSTEM_CLOCK

    cdef cppclass duration:
        duration()
        duration(const int64_t&, const time_unit& u)
        duration(const duration&)

        int64_t count() const
        time_unit unit() const

        int64_t count(const time_unit& unit) const

        duration operator+() const
        duration operator-() const

        duration operator+(const duration& d) const
        duration operator-(const duration& d) const

        duration operator*(const int64_t& d) const
        duration operator/(const int64_t& d) const
        duration operator%(const int64_t& d) const

        bool operator==(const duration& d) const
        bool operator!=(const duration& d) const
        bool operator<(const duration& d) const
        bool operator<=(const duration& d) const
        bool operator>(const duration& d) const
        bool operator>=(const duration& d) const

    cdef cppclass time_point:
        time_point()
        time_point(const duration& d, const clock_type& clock)

        duration time_since_epoch() const

        time_point operator+(const duration& d) const
        time_point operator-(const duration& d) const

        duration operator-(const time_point& t) const

        bool operator==(const time_point& t) const
        bool operator!=(const time_point& t) const
        bool operator<(const time_point& t) const
        bool operator<=(const time_point& t) const
        bool operator>(const time_point& t) const
        bool operator>=(const time_point& t) const

    cdef cppclass internal_clock:
        @staticmethod
        time_point now()

    cdef cppclass system_clock:
        @staticmethod
        time_point now()


cdef class Duration:
    cdef duration *thisptr

    cpdef int64_t count(self, time_unit unit = *)

cdef class Timepoint:
    cdef time_point *thisptr

    cpdef Duration time_since_epoch(self)
