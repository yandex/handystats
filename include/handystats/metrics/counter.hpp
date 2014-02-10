#ifndef HANDYSTATS_METRICS_COUNTER_H_
#define HANDYSTATS_METRICS_COUNTER_H_

#include <utility>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

#include <handystats/chrono.hpp>

namespace handystats { namespace metrics {

class counter
{
public:

	typedef long long value_type;
	typedef typename chrono::default_duration time_duration;
	typedef typename chrono::steady_clock<time_duration> clock;
	typedef typename clock::time_point time_point;

	struct internal_stats {
		typedef boost::accumulators::features <
				boost::accumulators::tag::min,
				boost::accumulators::tag::max,
				boost::accumulators::tag::count
			> value_features;

		typedef boost::accumulators::features <
				boost::accumulators::tag::min,
				boost::accumulators::tag::max,
				boost::accumulators::tag::sum,
				boost::accumulators::tag::count,
				boost::accumulators::tag::mean
			> delta_features;

		boost::accumulators::accumulator_set<value_type, value_features> values;

		boost::accumulators::accumulator_set<value_type, delta_features> incr_deltas;
		boost::accumulators::accumulator_set<value_type, delta_features> decr_deltas;
		boost::accumulators::accumulator_set<value_type, delta_features> deltas;

		static void initialize(internal_stats& stats);
		static void initialize(internal_stats& stats, value_type value, time_point timestamp);

		static void update_value(internal_stats& stats, value_type value, time_point timestamp);
		static void update_increment(internal_stats& stats, value_type delta, time_point timestamp);
		static void update_decrement(internal_stats& stats, value_type delta, time_point timestamp);
	};

	counter();
	counter(value_type value, time_point timestamp);

	void increment(value_type value, time_point timestamp);
	void decrement(value_type value, time_point timestamp);

	std::pair<value_type, time_point> get() const;

	internal_stats get_stats() const;

private:
	value_type value;
	time_point timestamp;

	internal_stats stats;

}; // class counter

}} // namespace handystats::metrics

#endif // HANDYSTATS_METRICS_COUNTER_H_
