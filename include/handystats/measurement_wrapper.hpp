#ifndef HANDY_MEASUREMENT_WRAPPER_H_
#define HANDY_MEASUREMENT_WRAPPER_H_

namespace handystats {

template<class Value>
class measurement_wrapper {
public:
	typedef Value value_type;

	measurement_wrapper() : value{}
	{}

	measurement_wrapper(const value_type& value) : value(value)
	{}

	value_type get_value() const {
		return value;
	}

private:
	value_type value;

}; // class measurement_wrapper

} // namespace handystats

#endif // HANDY_MEASUREMENT_WRAPPER_H_
