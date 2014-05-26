#ifndef HANDYSTATS_SYSTEM_STATS_IMPL_H_
#define HANDYSTATS_SYSTEM_STATS_IMPL_H_

#include <handystats/metrics/gauge.hpp>


namespace handystats {

extern metrics::gauge message_queue_size;
extern metrics::gauge internal_metrics_size;

extern metrics::gauge message_process_time;
extern metrics::gauge message_pop_time;

} // namespace handystats


#endif // HANDYSTATS_SYSTEM_STATS_IMPL_H_
