#ifndef HANDY_STATISTICS_H_
#define HANDY_STATISTICS_H_

#include <sstream>
#include <string>

namespace handystats {

template<class Statistics_impl, class Measurement>
class statistics {
public:
	typedef Measurement measurement_type;

	statistics() {}

	template<typename iterator>
	statistics(iterator first, iterator last)
	{
		new (this) Statistics_impl();
		for (iterator it = first; it != last; ++it) {
			add_measurement(*it);
		}
	}

	void add_measurement(const measurement_type& measurement) {
		static_cast<Statistics_impl*>(this)->add_measurement(measurement);
	}

	friend std::ostream& operator<< (std::ostream& os, const statistics& stats) {
		return os << "OUTPUT IS NOT IMPLEMENTED";
	}

	std::string to_string() const {
		std::ostringstream os;
		os << *this;
		return os.str();
	}

}; // class statistics

template<class Measurement>
class no_statistics : public statistics<no_statistics<Measurement>, Measurement> {
public:
	typedef statistics<no_statistics<Measurement>, Measurement> base_statistics;

	typedef typename base_statistics::measurement_type measurement_type;

	no_statistics() : base_statistics()
	{}

	void add_measurement(const measurement_type& measurement) {
	}

	friend std::ostream& operator<< (std::ostream& os, const no_statistics& stats) {
		return os;
	}

	std::string to_string() const {
		return {};
	}

}; // class no_statistics

} // namespace handystats

#endif // HANDY_STATISTICS_H_
