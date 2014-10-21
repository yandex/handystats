# distutils: language = c++

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
