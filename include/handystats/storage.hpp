#ifndef HANDY_STORAGE_H_
#define HANDY_STORAGE_H_

namespace handystats {

template<class Storage_impl, class Measurement>
class storage {
public:
	typedef Measurement measurement_type;

	storage() {}

	void add_measurement(const measurement_type& measurement) {
		static_cast<Storage_impl*>(this)->add_measurement(measurement);
	}

}; // class storage

template<class Measurement>
class no_storage : public storage<no_storage<Measurement>, Measurement> {
public:
	typedef storage<no_storage<Measurement>, Measurement> base_storage;

	typedef typename base_storage::measurement_type measurement_type;

	no_storage() : base_storage()
	{}

	void add_measurement(const measurement_type& measurement) {
	}

}; // class no_storage

} // namespace handystats

#endif // HANDY_STORAGE_H_
