// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/atomic.hpp>

#if GCC_VERSION < 40500

namespace std {

template <typename T>
atomic<T*>::store(T* value, memory_order order) volatile {
	this->exchange(value, order);
}

}

#endif
