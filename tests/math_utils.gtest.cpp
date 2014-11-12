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

#include <gtest/gtest.h>

#include <handystats/math_utils.hpp>

TEST(MathUtils, TestIntComparison) {
	ASSERT_LT(handystats::math_utils::cmp<int>(10, 11), 0);
	ASSERT_EQ(handystats::math_utils::cmp<int>(1000, 1000), 0);
	ASSERT_GT(handystats::math_utils::cmp<int>(10, -100), 0);
}

TEST(MathUtils, TestDoubleComparison) {
	ASSERT_LT(handystats::math_utils::cmp<double>(100.0, 100.00001), 0);
	ASSERT_EQ(handystats::math_utils::cmp<double>(101.0, 1111 / 11), 0);
	ASSERT_GT(handystats::math_utils::cmp<double>(101.0, (1111 - 0.000011) / 11), 0);
}

TEST(MathUtils, TestSqrt) {
	ASSERT_DOUBLE_EQ(handystats::math_utils::sqrt<double>(0), 0);

	const double value = 11.22334455;
	ASSERT_DOUBLE_EQ(handystats::math_utils::sqrt<double>(value * value), value);

	const double sqrt_value = handystats::math_utils::sqrt<double>(value);
	ASSERT_DOUBLE_EQ(sqrt_value * sqrt_value, value);
}

