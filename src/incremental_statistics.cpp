// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include "configuration_impl.hpp"

#include <handystats/incremental_statistics.hpp>

namespace handystats {

incremental_statistics::incremental_statistics()
	: values(
			boost::accumulators::tag::moving_average::alpha = config::incremental_statistics.moving_average_alpha,
			boost::accumulators::time_interval = config::incremental_statistics.moving_interval
		)
{
}

void incremental_statistics::operator() (const value_type& value, const time_point& timestamp) {
	values(value, boost::accumulators::timestamp = timestamp);
}


incremental_statistics::value_type incremental_statistics::min() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::min >(values);
}
incremental_statistics::value_type incremental_statistics::max() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::max >(values);
}
incremental_statistics::value_type incremental_statistics::sum() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::sum >(values);
}
incremental_statistics::value_type incremental_statistics::count() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::count >(values);
}
incremental_statistics::value_type incremental_statistics::mean() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::mean >(values);
}
incremental_statistics::value_type incremental_statistics::moving_average() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::moving_average >(values);
}
incremental_statistics::value_type incremental_statistics::interval_count() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::interval_count >(values);
}
incremental_statistics::value_type incremental_statistics::interval_sum() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::interval_sum >(values);
}
incremental_statistics::value_type incremental_statistics::interval_mean() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::interval_mean >(values);
}

} // namespace handystats
