#ifndef HANDYSTATS_SYSTEM_STATS_IMPL_H_
#define HANDYSTATS_SYSTEM_STATS_IMPL_H_

#include <handystats/metrics/gauge.hpp>
#include <handystats/metrics/counter.hpp>


namespace handystats {

extern metrics::counter message_queue_size;
extern metrics::counter internal_metrics_size;

extern metrics::gauge message_processing_time;
extern metrics::gauge message_push_time;
extern metrics::gauge message_pop_time;

} // namespace handystats


#endif // HANDYSTATS_SYSTEM_STATS_IMPL_H_
