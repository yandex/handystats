#ifndef HANDY_INCREMENTAL_STATISTICS_H_
#define HANDY_INCREMENTAL_STATISTICS_H_

#include <sstream>
#include <string>
#include <limits>

#include <handystats/math_utils.hpp>
#include <handystats/statistics.hpp>

namespace handystats {

template<class Measurement>
class incremental_statistics : public statistics<incremental_statistics<Measurement>, Measurement> {
public:
	typedef statistics<incremental_statistics<Measurement>, Measurement> base_statistics;
	typedef typename base_statistics::measurement_type measurement_type;
	typedef typename measurement_type::value_type value_type;

	static_assert(std::is_convertible<value_type, double>::value, "Measurement value type must be convertible to double!");

	incremental_statistics()
		: count{}
		, sum{}
		, squared_sum{}
		, min(std::numeric_limits<double>::max())
		, max(std::numeric_limits<double>::min())
	{}

	template<typename iterator>
	incremental_statistics(iterator first, iterator last)
		: base_statistics(first, last)
	{
	}

	void add_measurement(const measurement_type& measurement) {
		add_value(measurement.get_value());
	}

	void clear() {
		count = {};
		sum = {};
		squared_sum = {};
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

private:
	int count;
	double sum;
	double squared_sum;

	double min;
	double max;

}; // class incremental_statistics

} // namespace handystats

#endif // HANDY_INCREMENTAL_STATISTICS_H_
