#include <handystats/metrics/gauge.hpp>


namespace handystats { namespace internal {

metrics::gauge event_message_queue_size;
metrics::gauge internal_metrics_size;
metrics::gauge message_processing_time;
metrics::gauge message_push_time;
metrics::gauge message_pop_time;

}} // namespace handystats::internal
