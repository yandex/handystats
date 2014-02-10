#ifndef HANDYSTATS_METRICS_GAUGE_H_
#define HANDYSTATS_METRICS_GAUGE_H_

#include <utility>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

#include <handystats/chrono.hpp>

namespace handystats { namespace metrics {

class gauge
{
public:

	typedef long double value_type;
	typedef chrono::default_duration time_duration;
	typedef typename chrono::steady_clock<time_duration> clock;
	typedef typename clock::time_point time_point;

	struct internal_stats {
		typedef boost::accumulators::features <
				boost::accumulators::tag::min,
				boost::accumulators::tag::max,
				boost::accumulators::tag::sum,
				boost::accumulators::tag::count,
				boost::accumulators::tag::mean
			> value_features;

		boost::accumulators::accumulator_set<value_type, value_features> values;

		static void initialize(internal_stats& stats);
		static void initialize(internal_stats& stats, value_type value, time_point timestamp);
		static void update_value(internal_stats& stats, value_type value, time_point timestamp);
	};

	gauge();
	gauge(value_type value, time_point timestamp);

	void set(value_type value, time_point timestamp);
	std::pair<value_type, time_point> get() const;

	internal_stats get_stats() const;

private:
	value_type value;
	time_point timestamp;

	internal_stats stats;

}; // class gauge

}} // namespace handystats::metrics

#endif // HANDYSTATS_METRICS_GAUGE_H_
