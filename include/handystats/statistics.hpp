// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_STATISTICS_HPP_
#define HANDYSTATS_STATISTICS_HPP_

#include <utility>
#include <vector>
#include <string>
#include <exception>

#include <handystats/common.h>
#include <handystats/chrono.hpp>
#include <handystats/config/statistics.hpp>

namespace handystats {

class statistics {
public:
	typedef double value_type;
	typedef chrono::clock clock;
	typedef clock::duration duration;
	typedef clock::time_point time_point;

	// histogram
	typedef std::pair<value_type, double> bin_type;
	typedef std::vector<bin_type> histogram_type;

	typedef std::exception invalid_tag_error;

	// quantile extractor
	// result of statistics::get<tag::quantile>
	struct quantile_extractor {
		quantile_extractor(const statistics* const = nullptr);
		double at(const double& probability) const;
	private:
		const statistics* const m_statistics;
	};
	friend struct quantile_extractor;

	struct tag {
		typedef int type;

		static const type empty = 0;

		static const type value = 1 << 1;
		static const type min = 1 << 2;
		static const type max = 1 << 3;
		static const type count = 1 << 4;
		static const type sum = 1 << 5;
		static const type avg = 1 << 6;
		static const type moving_count = 1 << 7;
		static const type moving_sum = 1 << 8;
		static const type moving_avg = 1 << 9;
		static const type histogram = 1 << 10;
		static const type quantile = 1 << 11;
		static const type timestamp = 1 << 12;
		static const type rate = 1 << 13;
		static const type entropy = 1 << 14;

		static type from_string(const std::string&);
	};

	template <tag::type Tag, tag::type Expected, typename T>
	struct enable_if_eq : std::enable_if<Tag == Expected, T>
	{};

	// Result type of specific statistics tag
	template <tag::type Tag>
	struct result_type
		: enable_if_eq<Tag, tag::value, value_type>
		, enable_if_eq<Tag, tag::min, value_type>
		, enable_if_eq<Tag, tag::max, value_type>
		, enable_if_eq<Tag, tag::count, size_t>
		, enable_if_eq<Tag, tag::sum, value_type>
		, enable_if_eq<Tag, tag::avg, double>
		, enable_if_eq<Tag, tag::moving_count, double>
		, enable_if_eq<Tag, tag::moving_sum, double>
		, enable_if_eq<Tag, tag::moving_avg, double>
		, enable_if_eq<Tag, tag::histogram, histogram_type>
		, enable_if_eq<Tag, tag::quantile, quantile_extractor>
		, enable_if_eq<Tag, tag::timestamp, time_point>
		, enable_if_eq<Tag, tag::rate, double>
		, enable_if_eq<Tag, tag::entropy, double>
	{};

	// statistics is enabled from configuration
	// but could also be computed due to data dependency
	bool enabled(const tag::type& t) const HANDYSTATS_NOEXCEPT;
	bool computed(const tag::type& t) const HANDYSTATS_NOEXCEPT;

	tag::type tags() const HANDYSTATS_NOEXCEPT;

	// Ctor
	statistics(
			const config::statistics& opts = config::statistics()
		);

	void reset();

	void update(const value_type& value, const time_point& timestamp = clock::now());
	void update_time(const time_point& timestamp = clock::now());

	// Depricated iface, use get<tag>
	value_type value() const;
	value_type min() const;
	value_type max() const;
	value_type sum() const;
	size_t count() const;
	double avg() const;
	double moving_count() const;
	double moving_sum() const;
	double moving_avg() const;
	histogram_type histogram() const;
	double quantile(const double& probability) const;
	time_point timestamp() const;

	// Method will throw if statistics tag is not computed
	template <tag::type Tag>
	typename result_type<Tag>::type
	get() const
	{
		if (computed(Tag)) {
			return get_impl<Tag>();
		}
		else {
			throw invalid_tag_error();
		}
	}

	// Method will not throw if statistics tag is not computed
	// instead it will return passed default value
	template <tag::type Tag>
	typename result_type<Tag>::type
	get_safe(
			const typename result_type<Tag>::type& default_value =
				typename result_type<Tag>::type()
		)
		const HANDYSTATS_NOEXCEPT
	{
		if (computed(Tag)) {
			return get_impl<Tag>();
		}
		else {
			return default_value;
		}
	}

private:
	// configuration (internal form)
	duration m_moving_interval;
	size_t m_histogram_bins;
	tag::type m_tags;
	duration m_rate_unit;

	template <tag::type Tag>
	typename result_type<Tag>::type get_impl() const;

	value_type m_value;
	value_type m_min;
	value_type m_max;
	value_type m_sum;
	size_t m_count;
	value_type m_moving_count;
	value_type m_moving_sum;
	histogram_type m_histogram;
	time_point m_timestamp;
	value_type m_rate;
};

} // namespace handystats

#endif // HANDYSTATS_INCREMENTAL_STATISTICS_HPP_
