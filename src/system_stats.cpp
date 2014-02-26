#include <handystats/metrics/gauge.hpp>
#include <handystats/metrics/counter.hpp>


namespace handystats {

metrics::counter message_queue_size;
metrics::counter internal_metrics_size;

metrics::gauge message_processing_time;
metrics::gauge message_push_time;
metrics::gauge message_pop_time;

} // namespace handystats
