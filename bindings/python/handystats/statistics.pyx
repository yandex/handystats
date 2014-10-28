# distutils: language = c++

from cython.operator cimport dereference as deref

cdef class Data:
    def __cinit__(self):
        self.thisptr = new statistics.data()

    def __dealloc__(self):
        del self.thisptr

    cpdef append(self, Data other):
        self.thisptr.append(deref(other.thisptr))

    cpdef merge(self, Data other):
        self.thisptr.merge(deref(other.thisptr))

    cpdef truncate(self, Timepoint before = None, Timepoint after = None):
        if before is not None:
            self.thisptr.truncate_before(deref(before.thisptr))
        if after is not None:
            self.thisptr.truncate_after(deref(after.thisptr))

    @staticmethod
    def from_json(json_object):
        cdef string s = json_object.encode('utf-8')

        cdef Data data
        data = Data()
        data.thisptr[0] = loads[statistics.data](s)

        return data

    def to_json(self):
        cdef string s = dumps[statistics.data](deref(self.thisptr))
        return (<bytes>s).decode('utf-8')

cdef class Statistics:
    def __cinit__(self):
        self.thisptr = new statistics()

    def __dealloc__(self):
        del self.thisptr

    @staticmethod
    def from_data(Data data):
        cdef Statistics stats
        stats = Statistics()
        stats.thisptr[0] = statistics(deref(data.thisptr))

        return stats

    def value(self):
        return self.thisptr.value()

    def min(self):
        return self.thisptr.min()

    def max(self):
        return self.thisptr.max()

    def sum(self):
        return self.thisptr.sum()

    def count(self):
        return self.thisptr.count()

    def avg(self):
        return self.thisptr.avg()

    def moving_count(self):
        return self.thisptr.moving_count()

    def moving_sum(self):
        return self.thisptr.moving_sum()

    def moving_avg(self):
        return self.thisptr.moving_avg()

    def quantile(self, double probability):
        return self.thisptr.quantile(probability)

    def timestamp(self):
        cdef Timepoint ts
        ts = Timepoint()

        ts.thisptr[0] = self.thisptr.timestamp()

        return ts
