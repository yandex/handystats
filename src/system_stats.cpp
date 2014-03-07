#include <handystats/metrics/gauge.hpp>
#include <handystats/metrics/counter.hpp>


namespace handystats {

metrics::gauge message_queue_size;
metrics::gauge internal_metrics_size;

metrics::gauge message_processing_time;
metrics::gauge message_pop_time;

} // namespace handystats
