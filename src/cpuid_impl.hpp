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

#ifndef HANDYSTATS_CPUID_IMPL_HPP_
#define HANDYSTATS_CPUID_IMPL_HPP_

#include <cstdint>
#include <cpuid.h>

namespace handystats {

// TSC Support (1 EDX Bit 04)
inline
bool tsc_supported() {
	uint32_t eax, ebx, ecx, edx;

	if (!__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
		return false;
	}

	return ((edx >> 4) & 1);
}

// Invariant TSC support (80000007H EDX Bit 08)
static
bool invariant_tsc() {
	uint32_t eax, ebx, ecx, edx;

	if (!__get_cpuid(0x80000007, &eax, &ebx, &ecx, &edx)) {
		return false;
	}

	return ((edx >> 8) & 1);
}

// RDTSCP Instruction support (80000001H EDX Bit 27)
static
bool rdtscp_supported() {
	uint32_t eax, ebx, ecx, edx;

	if (!__get_cpuid(0x80000001, &eax, &ebx, &ecx, &edx)) {
		return false;
	}

	return ((edx >> 27) & 1);
}

} // namespace handystats

#endif // HANDYSTATS_CPUID_IMPL_HPP_
