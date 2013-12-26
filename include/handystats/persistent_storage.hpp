#ifndef HANDY_PERSISTENT_STORAGE_H_
#define HANDY_PERSISTENT_STORAGE_H_

#include <deque>

namespace handystats {

template<class Measurement>
class persistent_storage : public storage<persistent_storage<Measurement>, Measurement> {
public:
	typedef storage<persistent_storage<Measurement>, Measurement> base_storage;

	typedef typename base_storage::measurement_type measurement_type;

	persistent_storage() : base_storage()
	{}

	void add_measurement(const measurement_type& measurement) {
		measurements.push_back(measurement);
	}

	std::deque<measurement_type> get_measurements() const {
		return measurements;
	}

private:
	std::deque<measurement_type> measurements;

}; // class persistent_storage

} // namespace handystats

#endif // HANDY_PERSISTENT_STORAGE_H_
