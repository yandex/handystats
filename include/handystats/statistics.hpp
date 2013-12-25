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

} // namespace handystats

#endif // HANDY_STATISTICS_H_
