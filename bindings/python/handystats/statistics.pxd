# distutils: language = c++

from libcpp.string cimport string
from libc.stdint cimport uint64_t

from handystats.chrono cimport time_point

cdef extern from "handystats/detail/statistics.hpp" namespace "handystats":
    cdef cppclass statistics:
        cppclass data:
            data()
            void append(data)
            void merge(const data)

        statistics()
        statistics(const data&)

        double value() except +
        double min() except +
        double max() except +
        double sum() except +
        uint64_t count() except +
        double avg() except +
        double moving_count() except +
        double moving_sum() except +
        double moving_avg() except +
        double quantile(const double&) except +
        time_point timestamp() except +

cdef extern from "handystats/detail/serialization/json.hpp" namespace "handystats::serialization::json":
    cdef T loads[T](const string) except +
    cdef string dumps[T](const T&)

cdef class Data:
    cdef statistics.data *thisptr

    cpdef append(self, Data other)
    cpdef merge(self, Data other)

cdef class Statistics:
    cdef statistics *thisptr
