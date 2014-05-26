#include <handystats/metrics/gauge.hpp>


namespace handystats {

metrics::gauge message_queue_size;
metrics::gauge internal_metrics_size;

metrics::gauge message_process_time;
metrics::gauge message_pop_time;

} // namespace handystats
