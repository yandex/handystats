#ifndef HANDYSTATS_SYSTEM_STATS_IMPL_H_
#define HANDYSTATS_SYSTEM_STATS_IMPL_H_


namespace handystats { namespace metrics {

struct gauge;
struct counter;

}} // namespace handystats::metrics


namespace handystats {

extern metrics::counter message_queue_size;
extern metrics::counter internal_metrics_size;

extern metrics::gauge message_processing_time;
extern metrics::gauge message_push_time;
extern metrics::gauge message_pop_time;

} // namespace handystats


#endif // HANDYSTATS_SYSTEM_STATS_IMPL_H_
