// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <cstring>
#include <iterator>
#include <vector>
#include <utility>

#include <handystats/common.h>
#include <handystats/math_utils.hpp>

#include <handystats/statistics.hpp>
#include <handystats/detail/statistics.hpp>

namespace handystats {

const statistics::tag::type statistics::tag::empty;
const statistics::tag::type statistics::tag::value;
const statistics::tag::type statistics::tag::min;
const statistics::tag::type statistics::tag::max;
const statistics::tag::type statistics::tag::count;
const statistics::tag::type statistics::tag::sum;
const statistics::tag::type statistics::tag::avg;

const statistics::tag::type statistics::tag::moving_count;
const statistics::tag::type statistics::tag::moving_sum;
const statistics::tag::type statistics::tag::moving_avg;
const statistics::tag::type statistics::tag::histogram;
const statistics::tag::type statistics::tag::quantile;
const statistics::tag::type statistics::tag::entropy;
const statistics::tag::type statistics::tag::throughput;
const statistics::tag::type statistics::tag::frequency;

const statistics::tag::type statistics::tag::timestamp;

statistics::tag::type statistics::tag::from_string(const std::string& tag_name) {
	if (strcmp("value", tag_name.c_str()) == 0) {
		return value;
	}
	if (strcmp("min", tag_name.c_str()) == 0) {
		return min;
	}
	if (strcmp("max", tag_name.c_str()) == 0) {
		return max;
	}
	if (strcmp("count", tag_name.c_str()) == 0) {
		return count;
	}
	if (strcmp("sum", tag_name.c_str()) == 0) {
		return sum;
	}
	if (strcmp("avg", tag_name.c_str()) == 0) {
		return avg;
	}

	if (strcmp("moving-count", tag_name.c_str()) == 0) {
		return moving_count;
	}
	if (strcmp("moving-sum", tag_name.c_str()) == 0) {
		return moving_sum;
	}
	if (strcmp("moving-avg", tag_name.c_str()) == 0) {
		return moving_avg;
	}
	if (strcmp("histogram", tag_name.c_str()) == 0) {
		return histogram;
	}
	if (strcmp("quantile", tag_name.c_str()) == 0) {
		return quantile;
	}
	if (strcmp("entropy", tag_name.c_str()) == 0) {
		return entropy;
	}
	if (strcmp("throughput", tag_name.c_str()) == 0) {
		return throughput;
	}
	if (strcmp("frequency", tag_name.c_str()) == 0) {
		return frequency;
	}

	if (strcmp("timestamp", tag_name.c_str()) == 0) {
		return timestamp;
	}

	throw invalid_tag_error("tag::from_string: invalid tag");
}

bool statistics::enabled(const statistics::tag::type& t) const HANDYSTATS_NOEXCEPT {
	return (m_config.tags & t) && computed(t);
}

bool statistics::computed(const statistics::tag::type& t) const HANDYSTATS_NOEXCEPT {
	return m_data->computed(t);
}

statistics::tag::type statistics::tags() const HANDYSTATS_NOEXCEPT {
	return m_config.tags;
}

statistics::statistics(
			const config::statistics& opts
		)
	: m_config(opts)
	, m_data(new data(opts))
{
	reset();
}

statistics::statistics(statistics&& stats)
	: m_config(std::move(stats.m_config))
	, m_data(std::move(stats.m_data))
{
}

statistics::statistics(const statistics& stats)
	: m_config(stats.m_config)
	, m_data(new data(*stats.m_data)) // deep copy
{
}

statistics::statistics(const data& d)
	: m_config()
	, m_data(new data(d))
{
	m_config.moving_interval = m_data->m_moving_interval;
	m_config.histogram_bins = m_data->m_histogram_bins;

	m_config.tags = tag::empty;

	m_data->fulfill_dependencies();
}

statistics::~statistics()
{
	m_data.reset();
}

statistics& statistics::operator= (const statistics& stats) {
	m_config = stats.m_config;
	m_data = std::unique_ptr<data>(new data(*stats.m_data));

	return *this;
}

statistics& statistics::operator= (statistics&& stats) {
	m_config = std::move(stats.m_config);
	m_data.swap(stats.m_data);

	stats.m_data.reset();

	return *this;
}

void statistics::reset() {
	m_data->reset();
}

void statistics::update(const value_type& value, const time_point& timestamp) {
	m_data->update(value, timestamp);
}

void statistics::update_time(const time_point& timestamp) {
	m_data->update_time(timestamp);
}

// get_impl
template <>
statistics::result_type<statistics::tag::value>::type
statistics::get_impl<statistics::tag::value>(const data& data)
{
	if (data.computed(tag::value)) {
		return data.m_value;
	}
	else {
		throw invalid_tag_error("invalid tag: value");
	}
}

template <>
statistics::result_type<statistics::tag::min>::type
statistics::get_impl<statistics::tag::min>(const data& data)
{
	if (data.computed(tag::min)) {
		return data.m_min;
	}
	else {
		throw invalid_tag_error("invalid tag: min");
	}
}

template <>
statistics::result_type<statistics::tag::max>::type
statistics::get_impl<statistics::tag::max>(const data& data)
{
	if (data.computed(tag::max)) {
		return data.m_max;
	}
	else {
		throw invalid_tag_error("invalid tag: max");
	}
}

template <>
statistics::result_type<statistics::tag::count>::type
statistics::get_impl<statistics::tag::count>(const data& data)
{
	if (data.computed(tag::count)) {
		return data.m_count;
	}
	else {
		throw invalid_tag_error("invalid tag: count");
	}
}

template <>
statistics::result_type<statistics::tag::sum>::type
statistics::get_impl<statistics::tag::sum>(const data& data)
{
	if (data.computed(tag::sum)) {
		return data.m_sum;
	}
	else {
		throw invalid_tag_error("invalid tag: sum");
	}
}

template <>
statistics::result_type<statistics::tag::avg>::type
statistics::get_impl<statistics::tag::avg>(const data& data)
{
	if (data.computed(tag::avg)) {
		if (data.m_count == 0) {
			return 0;
		}
		else {
			return result_type<tag::avg>::type(data.m_sum) / data.m_count;
		}
	}
	else {
		throw invalid_tag_error("invalid tag: avg");
	}
}

template <>
statistics::result_type<statistics::tag::moving_count>::type
statistics::get_impl<statistics::tag::moving_count>(const data& data)
{
	if (data.computed(tag::moving_count)) {
		return data.m_moving_count;
	}
	else {
		throw invalid_tag_error("invalid tag: moving_count");
	}
}

template <>
statistics::result_type<statistics::tag::moving_sum>::type
statistics::get_impl<statistics::tag::moving_sum>(const data& data)
{
	if (data.computed(tag::moving_sum)) {
		return data.m_moving_sum;
	}
	else {
		throw invalid_tag_error("invalid tag: moving_sum");
	}
}

template <>
statistics::result_type<statistics::tag::moving_avg>::type
statistics::get_impl<statistics::tag::moving_avg>(const data& data)
{
	if (data.computed(tag::moving_avg)) {
		if (math_utils::cmp<result_type<tag::moving_count>::type>(data.m_moving_count, 0) <= 0) {
			return 0;
		}
		else {
			return result_type<tag::moving_avg>::type(data.m_moving_sum) / data.m_moving_count;
		}
	}
	else {
		throw invalid_tag_error("invalid tag: moving_avg");
	}
}

template <>
statistics::result_type<statistics::tag::histogram>::type
statistics::get_impl<statistics::tag::histogram>(const data& data)
{
	if (data.computed(tag::histogram)) {
		return data.m_histogram;
	}
	else {
		throw invalid_tag_error("invalid tag: histogram");
	}
}

template <>
statistics::result_type<statistics::tag::quantile>::type
statistics::get_impl<statistics::tag::quantile>(const data& data)
{
	if (data.computed(tag::quantile)) {
		return result_type<tag::quantile>::type(&data);
	}
	else {
		throw invalid_tag_error("invalid tag: quantile");
	}
}

template <>
statistics::result_type<statistics::tag::entropy>::type
statistics::get_impl<statistics::tag::entropy>(const data& data)
{
	if (data.computed(tag::entropy)) {
		const auto& histogram = data.m_histogram;

		if (histogram.size() <= 1) {
			return 0;
		}

		double moving_count = 0;
		for (auto bin = histogram.begin(); bin != histogram.end(); ++bin) {
			moving_count += std::get<BIN_COUNT>(*bin);
		}
		if (math_utils::cmp<double>(moving_count, 0) <= 0) {
			return 0;
		}

		double H = 0;
		for (size_t bin_index = 0; bin_index < histogram.size(); ++bin_index) {
			const auto& bin = histogram[bin_index];
			if (math_utils::cmp<double>(std::get<BIN_COUNT>(bin), 0) <= 0) {
				continue;
			}

			double bin_width = 0;
			if (bin_index > 0) {
				bin_width +=
					(std::get<BIN_CENTER>(bin) - std::get<BIN_CENTER>(histogram[bin_index - 1])) *
						std::get<BIN_COUNT>(bin) / (std::get<BIN_COUNT>(bin) + std::get<BIN_COUNT>(histogram[bin_index - 1]));
			}
			if (bin_index < histogram.size() - 1) {
				bin_width +=
					(std::get<BIN_CENTER>(histogram[bin_index + 1]) - std::get<BIN_CENTER>(bin)) *
						std::get<BIN_COUNT>(bin) / (std::get<BIN_COUNT>(bin) + std::get<BIN_COUNT>(histogram[bin_index + 1]));
			}
			if (bin_index == 0 || bin_index == histogram.size() - 1) {
				bin_width *= 2;
			}

			H -= std::get<BIN_COUNT>(bin) * log(std::get<BIN_COUNT>(bin) / moving_count / bin_width) / moving_count ;
		}

		return H;
	}
	else {
		throw invalid_tag_error("invalid tag: entropy");
	}
}

template <>
statistics::result_type<statistics::tag::throughput>::type
statistics::get_impl<statistics::tag::throughput>(const data& data)
{
	if (data.computed(tag::throughput)) {
		auto unit = chrono::time_unit::NSEC;
		return data.m_moving_sum * chrono::seconds(1).count(unit) / data.m_moving_interval.count(unit);
	}
	else {
		throw invalid_tag_error("invalid tag: throughput");
	}
}

template <>
statistics::result_type<statistics::tag::frequency>::type
statistics::get_impl<statistics::tag::frequency>(const data& data)
{
	if (data.computed(tag::frequency)) {
		auto unit = chrono::time_unit::NSEC;
		return data.m_moving_count * chrono::seconds(1).count(unit) / data.m_moving_interval.count(unit);
	}
	else {
		throw invalid_tag_error("invalid tag: frequency");
	}
}

template <>
statistics::result_type<statistics::tag::timestamp>::type
statistics::get_impl<statistics::tag::timestamp>(const data& data)
{
	if (data.computed(tag::timestamp)) {
		return data.m_timestamp;
	}
	else {
		throw invalid_tag_error("invalid tag: timestamp");
	}
}

} // namespace handystats
