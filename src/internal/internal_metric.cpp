#include "handystats/internal.hpp"

namespace handystats { namespace internal {

template<class impl, class metric>
internal_metric<impl, metric>::internal_metric() {
	base_metric = nullptr;
}

template<class impl, class metric>
internal_metric<impl, metric>::~internal_metric() {
	if (base_metric) {
		delete base_metric;
	}
}

}} // namespace handystats::internal
