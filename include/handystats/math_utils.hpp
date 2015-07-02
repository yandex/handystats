/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#ifndef HANDYSTATS_MATH_UTILS_IMPL_HPP_
#define HANDYSTATS_MATH_UTILS_IMPL_HPP_

#include <limits>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace handystats {

class math_utils
{
public:
	template<class Value>
	static int cmp(const Value& a, const Value& b, const Value& epsilon = std::numeric_limits<Value>::epsilon() * 4)
	{
		if (a + epsilon < b) {
			return -1;
		}
		if (a - epsilon > b) {
			return 1;
		}
		return 0;
	}

	template<class Value>
	static long double sqrt(const Value& a)
	{
		if (cmp(a, static_cast<Value>(0)) < 0) {
			throw std::logic_error("sqrt of negative number");
		}
		return std::sqrt(static_cast<long double>(a));
	}

	// a x^2 + b x + c = 0
	static std::vector<long double>
	solve_quadratic(const long double& a, const long double& b, const long double& c)
	{
		// b x + c = 0
		if (cmp<long double>(a, 0.0) == 0) {
			if (cmp<long double>(b, 0.0) == 0) {
				return {};
			}
			else {
				return {- c / b};
			}
		}
		else {
			double d = b * b - 4 * a * c;
			if (cmp<long double>(d, 0.0) < 0) {
				return {};
			}
			else if (cmp<long double>(d, 0.0) == 0) {
				return { - b / 2.0 / a };
			}
			else {
				d = sqrt(d);
				return {(-d - b) / 2.0 / a, (d - b) / 2.0 / a};
			}
		}

		return {};
	}

	static long double
	weighted_average(
			const long double& value1, const long double& weight1,
			const long double& value2, const long double& weight2
		)
	{
		const long double& total_weight = weight1 + weight2;
		if (cmp<long double>(total_weight, 0) == 0) {
			throw std::logic_error("weighted_average: zero total weight");
		}

		const long double& weighted_sum = value1 * weight1 + value2 * weight2;

		return weighted_sum / total_weight;
	}

}; // class math_utils

} // namespace handystats

#endif // HANDYSTATS_MATH_UTILS_IMPL_HPP_
