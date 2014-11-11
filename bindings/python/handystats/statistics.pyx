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

    def value(self):
        return extract_value(deref(self.thisptr))

    def min(self):
        return extract_min(deref(self.thisptr))

    def max(self):
        return extract_max(deref(self.thisptr))

    def sum(self):
        return extract_sum(deref(self.thisptr))

    def count(self):
        return extract_count(deref(self.thisptr))

    def avg(self):
        return extract_avg(deref(self.thisptr))

    def moving_count(self):
        return extract_moving_count(deref(self.thisptr))

    def moving_sum(self):
        return extract_moving_sum(deref(self.thisptr))

    def moving_avg(self):
        return extract_moving_avg(deref(self.thisptr))

    def quantile(self, double probability):
        return extract_quantile(deref(self.thisptr), probability)

    def entropy(self):
        return extract_entropy(deref(self.thisptr))

    def throughput(self):
        return extract_throughput(deref(self.thisptr))

    def frequency(self):
        return extract_frequency(deref(self.thisptr))

    def timestamp(self):
        cdef Timepoint ts
        ts = Timepoint()

        ts.thisptr[0] = extract_timestamp(deref(self.thisptr))

        return ts
