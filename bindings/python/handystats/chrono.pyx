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

from cython.operator cimport dereference as deref

cdef class Duration:
    def __cinit__(self):
        self.thisptr = new duration()

    def __dealloc__(self):
        del self.thisptr

    cpdef int64_t count(self, time_unit unit = SEC):
        return self.thisptr.count(unit)

    def __pos__(Duration self):
        cdef Duration d
        d = Duration()

        d.thisptr[0] = +deref(self.thisptr)

        return d

    def __neg__(Duration self):
        cdef Duration d
        d = Duration()

        d.thisptr[0] = -deref(self.thisptr)

        return d

    def __add__(Duration self, Duration other):
        cdef Duration res
        res = Duration()

        res.thisptr[0] = deref(self.thisptr) + deref(other.thisptr)

        return res

    def __sub__(Duration self, Duration other):
        cdef Duration res
        res = Duration()

        res.thisptr[0] = deref(self.thisptr) - deref(other.thisptr)

        return res

    def __mul__(Duration self, const int64_t& d):
        cdef Duration res
        res = Duration()

        res.thisptr[0] = deref(self.thisptr) * d

        return res

    def __div__(Duration self, const int64_t& d):
        cdef Duration res
        res = Duration()

        res.thisptr[0] = deref(self.thisptr) / d

        return res

    def __mod__(Duration self, const int64_t& d):
        cdef Duration res
        res = Duration()

        res.thisptr[0] = deref(self.thisptr) % d

        return res

    def __richcmp__(Duration self, Duration other not None, int op):
        if op == 0: # <
            return deref(self.thisptr) < deref(other.thisptr)
        elif op == 1: # <=
            return deref(self.thisptr) <= deref(other.thisptr)
        elif op == 2: # ==
            return deref(self.thisptr) == deref(other.thisptr)
        elif op == 3: # !=
            return deref(self.thisptr) != deref(other.thisptr)
        elif op == 4: # >
            return deref(self.thisptr) > deref(other.thisptr)
        elif op == 5: # >=
            return deref(self.thisptr) >= deref(other.thisptr)

cdef class Timepoint:
    def __cinit__(self):
        self.thisptr = new time_point()

    def __dealloc__(self):
        del self.thisptr

    @staticmethod
    def from_unixtime(unixtime):
        cdef duration _dur
        cdef time_point _tp
        cdef Timepoint t

        _dur = duration(unixtime, SEC)
        _tp = time_point(_dur, SYSTEM_CLOCK)

        t = Timepoint()
        t.thisptr[0] = _tp

        return t

    cpdef Duration time_since_epoch(self):
        cdef Duration since_epoch
        since_epoch = Duration()

        since_epoch.thisptr[0] = self.thisptr.time_since_epoch()

        return since_epoch

    def __add__(Timepoint self, Duration d):
        cdef Timepoint t
        t = Timepoint()

        t.thisptr[0] = deref(self.thisptr) + deref(d.thisptr)

        return t

    def __sub__(Timepoint self, other):
        cdef Timepoint t
        cdef Duration d
        if isinstance(other, Duration):
            t = Timepoint()

            t.thisptr[0] = deref(self.thisptr) - deref((<Duration>other).thisptr)

            return t
        elif isinstance(other, Timepoint):
            d = Duration()

            d.thisptr[0] = deref(self.thisptr) - deref((<Timepoint>other).thisptr)

            return d
        else:
            raise TypeError


    def __richcmp__(Timepoint self, Timepoint other not None, int op):
        if op == 0: # <
            return deref(self.thisptr) < deref(other.thisptr)
        elif op == 1: # <=
            return deref(self.thisptr) <= deref(other.thisptr)
        elif op == 2: # ==
            return deref(self.thisptr) == deref(other.thisptr)
        elif op == 3: # !=
            return deref(self.thisptr) != deref(other.thisptr)
        elif op == 4: # >
            return deref(self.thisptr) > deref(other.thisptr)
        elif op == 5: # >=
            return deref(self.thisptr) >= deref(other.thisptr)

class InternalClock:
    @staticmethod
    def now():
        cdef Timepoint t
        t = Timepoint()

        t.thisptr[0] = internal_clock.now()

        return t

class SystemClock:
    @staticmethod
    def now():
        cdef Timepoint t
        t = Timepoint()

        t.thisptr[0] = system_clock.now()

        return t
