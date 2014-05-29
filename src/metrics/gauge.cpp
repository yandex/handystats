#include "configuration_impl.hpp"

#include <handystats/metrics/gauge.hpp>


namespace handystats { namespace metrics {

gauge::internal_stats::internal_stats()
	: values(
		boost::accumulators::tag::moving_average::alpha = config::gauge.moving_average_alpha,
		boost::accumulators::time_interval = config::gauge.moving_interval
	)
{
}

void gauge::internal_stats::update_value(value_type value, time_point timestamp) {
	values(value, boost::accumulators::timestamp = timestamp);
}

gauge::value_type gauge::internal_stats::min() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::min >(values);
}
gauge::value_type gauge::internal_stats::max() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::max >(values);
}
gauge::value_type gauge::internal_stats::sum() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::sum >(values);
}
gauge::value_type gauge::internal_stats::count() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::count >(values);
}
gauge::value_type gauge::internal_stats::mean() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::mean >(values);
}
gauge::value_type gauge::internal_stats::moving_average() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::moving_average >(values);
}
gauge::value_type gauge::internal_stats::interval_count() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::interval_count >(values);
}
gauge::value_type gauge::internal_stats::interval_sum() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::interval_sum >(values);
}
gauge::value_type gauge::internal_stats::interval_mean() const {
	return boost::accumulators::extract_result< boost::accumulators::tag::interval_mean >(values);
}

gauge::gauge()
{
}

gauge::gauge(value_type value, time_point timestamp)
	: value(value)
	, timestamp(timestamp)
{
	stats.update_value(value, timestamp);
}


void gauge::set(value_type value, time_point timestamp) {
	this->value = value;
	this->timestamp = timestamp;

	stats.update_value(this->value, this->timestamp);
}

}} // namespace handystats::metrics
