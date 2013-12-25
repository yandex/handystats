#ifndef HANDY_OBSERVATION_H_
#define HANDY_OBSERVATION_H_

namespace handystats {

template<class Measurement, template<class Measurement> class Storage, template<class Measurement> class Statistics>
class observation : public Storage<Measurement>, public Statistics<Measurement>
{
public:
	typedef Measurement measurement_type;
	typedef Storage<Measurement> storage_type;
	typedef Statistics<Measurement> statistics_type;

	observation()
		: Storage<Measurement>()
		, Statistics<Measurement>()
	{}

	void add_measurement(const measurement_type& measurement) {
		storage_type::add_measurement(measurement);
		statistics_type::add_measurement(measurement);
	}
};

} // namespace handystats

#endif // HANDY_OBSERVATION_H_
