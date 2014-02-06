#ifndef HANDYSTATS_MATH_UTILS_H_
#define HANDYSTATS_MATH_UTILS_H_

#include <limits>
#include <cmath>

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
		if (cmp(a, static_cast<Value>(0)) <= 0) {
			return 0;
		}
		return std::sqrt(static_cast<long double>(a));
	}

}; // class math_utils

} // namespace handystats

#endif // HANDYSTATS_MATH_UTILS_H_
