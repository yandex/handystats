#ifndef HANDYSTATS_INTERNAL_IMPL_H_
#define HANDYSTATS_INTERNAL_IMPL_H_

#include <handystats/events/event_message.hpp>

namespace handystats { namespace internal {

void process_event_message(const events::event_message& message);

void initialize();
void finalize();

}} // namespace handystats::internal

#endif // HANDYSTATS_INTERNAL_IMPL_H_
