#ifndef HANDY_INCREMENTAL_STATISTICS_H_
#define HANDY_INCREMENTAL_STATISTICS_H_

#include <sstream>
#include <string>
#include <limits>

#include <handystats/math_utils.hpp>

namespace handystats {

class incremental_statistics {
public:
	incremental_statistics()
		: count(0)
		, sum(0)
		, squared_sum(0)
		, min(std::numeric_limits<double>::max())
		, max(std::numeric_limits<double>::min())
	{}

	template<typename iterator>
	incremental_statistics(iterator first, iterator last)
		: count(0)
		, sum(0)
		, squared_sum(0)
		, min(std::numeric_limits<double>::max())
		, max(std::numeric_limits<double>::min())
	{
		for (iterator it = first; it != last; ++it) {
			add_value(*it);
		}
	}

	void reset() {
		count = 0;
		sum = 0;
		squared_sum = 0;
		min = std::numeric_limits<double>::max();
		max = std::numeric_limits<double>::min();
	}

	void add_value(double value) {
		++count;
		sum += value;
		squared_sum += value * value;

		if (math_utils::cmp<double>(value, min) < 0) {
			min = value;
		}

		if (math_utils::cmp<double>(value, max) > 0) {
			max = value;
		}
	}

	int get_count() const {
		return count;
	}

	double get_sum() const {
		return sum;
	}

	double get_squared_sum() const {
		return squared_sum;
	}

	double get_mean() const {
		if (count == 0) {
			return 0;
		}
		return sum / count;
	}

	double get_variance() const {
		if (count == 0) {
			return 0;
		}
		return (squared_sum - sum * sum / count) / count;
	}

	double get_std_variance() const {
		if (count <= 1) {
			return 0;
		}
		return (squared_sum - sum * sum / count) / (count - 1);
	}

	double get_deviation() const {
		const double variance = get_variance();
		return math_utils::sqrt(variance);
	}

	double get_std_deviation() const {
		const double std_variance = get_std_variance();
		return math_utils::sqrt<double>(std_variance);
	}

	double get_min() const {
		return min;
	}

	double get_max() const {
		return max;
	}

	double get_range() const {
		return max - min;
	}

	friend std::ostream& operator<< (std::ostream& os, const incremental_statistics& stats) {
		// TODO implement proper output operator
		return os;
	}

	std::string to_string() const {
		std::ostringstream os;
		os << *this;
		return os.str();
	}

private:
	int count;
	double sum;
	double squared_sum;

	double min;
	double max;

}; // class incremental_statistics

} // namespace handystats

#endif // HANDY_INCREMENTAL_STATISTICS_H_
