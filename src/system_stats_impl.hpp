#ifndef HANDYSTATS_SYSTEM_STATS_IMPL_H_
#define HANDYSTATS_SYSTEM_STATS_IMPL_H_

#include <handystats/metrics/gauge.hpp>
#include <handystats/metrics/counter.hpp>


namespace handystats {

extern metrics::gauge message_queue_size;
extern metrics::gauge internal_metrics_size;

extern metrics::gauge message_processing_time;
extern metrics::gauge message_pop_time;

extern metrics::gauge active_events;

} // namespace handystats


#endif // HANDYSTATS_SYSTEM_STATS_IMPL_H_
