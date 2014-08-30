// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_CORE_HPP_
#define HANDYSTATS_CONFIG_CORE_HPP_

namespace handystats { namespace config {

struct core {
	struct defaults {
		static const bool enable;
	};

	bool enable;

	core();
};

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_CORE_HPP_
