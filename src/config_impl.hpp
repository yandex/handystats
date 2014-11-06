// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_IMPL_HPP_
#define HANDYSTATS_CONFIG_IMPL_HPP_

#include <vector>
#include <utility>
#include <string>
#include <memory>

#include "config/statistics_impl.hpp"
#include "config/metrics/gauge_impl.hpp"
#include "config/metrics/counter_impl.hpp"
#include "config/metrics/timer_impl.hpp"

#include "rapidjson/document.h"

namespace handystats { namespace config {

struct opts_t {
	opts_t();

	rapidjson::Value* select_pattern(const std::string& name) const;

	statistics m_statistics;

	metrics::gauge m_gauge;
	metrics::counter m_counter;
	metrics::timer m_timer;

	chrono::duration m_dump_interval;
	bool m_core_enable;

	std::vector<
		std::pair<
			std::vector<std::string>,
			rapidjson::Value*
		>
	>
	m_patterns;

	rapidjson::Document m_source;
};

extern std::shared_ptr<opts_t> opts;

void initialize();
void finalize();

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_IMPL_HPP_
