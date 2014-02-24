#ifndef HANDYSTATS_INTERNAL_STATISTICS_H_
#define HANDYSTATS_INTERNAL_STATISTICS_H_

#include <handystats/metrics/gauge.hpp>

namespace handystats { namespace internal {

extern metrics::gauge event_message_queue_size;
extern metrics::gauge internal_metrics_size;
extern metrics::gauge message_processing_time;
extern metrics::gauge message_push_time;
extern metrics::gauge message_pop_time;

}} // namespace handystats::internal

#endif // HANDYSTATS_INTERNAL_STATISTICS_H_
