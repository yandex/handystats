#ifndef HANDYSTATS_METRICS_GAUGE_H_
#define HANDYSTATS_METRICS_GAUGE_H_

#include <utility>

#include <handystats/chrono_impl.hpp>
#include <handystats/accumulators_impl.hpp>

namespace handystats { namespace metrics {

namespace default_parameters {
	extern std::vector<double> quantile_probs;
	extern std::vector<std::string> quantile_percents;
	extern double moving_average_alpha;
}

class gauge
{
public:

	typedef double value_type;
	typedef chrono::default_duration time_duration;
	typedef typename chrono::default_clock clock;
	typedef typename chrono::default_time_point time_point;

	struct internal_stats {
		typedef boost::accumulators::features <
				boost::accumulators::tag::min,
				boost::accumulators::tag::max,
				boost::accumulators::tag::sum,
				boost::accumulators::tag::count,
				boost::accumulators::tag::mean,
				boost::accumulators::tag::moving_average,
				boost::accumulators::tag::interval_count,
				boost::accumulators::tag::interval_sum,
				boost::accumulators::tag::interval_mean
			> value_features;

		boost::accumulators::accumulator_set<value_type, value_features> values;

		internal_stats()
			: values(
					boost::accumulators::tag::moving_average::alpha = default_parameters::moving_average_alpha,
					boost::accumulators::time_interval = std::chrono::seconds(1)
					)
		{
		}

		void update_value(value_type value, time_point timestamp);
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
