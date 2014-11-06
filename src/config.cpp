// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <mutex>
#include <fstream>
#include <string>
#include <iostream>

#include <fnmatch.h>

#include <handystats/core.hpp>
#include <handystats/core.h>

#include "rapidjson/document.h"

#include "core_impl.hpp"
#include "config_impl.hpp"

namespace handystats { namespace config {

/*
 * pattern ::= [<preabmle>] [ { <list> } ] [<postscript>]
 *
 * preamble ::= <word>
 *
 * word ::= sequence without <special_symbol> (may be empty)
 * special_symbol ::= { | } | ,
 *
 * list ::= <pattern> [, <list>]
 *
 * postscript ::= <pattern>
 */

// returns pattern expansion as vector of strings
// if expansion fails empty vector is returned
// else pos points to end of pattern
static
std::vector<std::string> expand_pattern(const char* pattern, size_t& pos, bool nested) {
	static const char SYM_LIST_DELIM = ',';
	static const char SYM_LIST_START = '{';
	static const char SYM_LIST_END = '}';

	size_t preamble_end = pos;
	while (true) {
		if (pattern[preamble_end] == 0) {
			break;
		}
		if (pattern[preamble_end] == SYM_LIST_START) {
			break;
		}
		if (nested && (pattern[preamble_end] == SYM_LIST_END || pattern[preamble_end] == SYM_LIST_DELIM)) {
			break;
		}
		++preamble_end;
	}

	std::string preamble(pattern + pos, preamble_end - pos);
	if (pattern[preamble_end] != SYM_LIST_START) {
		// pattern is preamble
		pos = preamble_end;
		return {preamble};
	}

	std::vector<std::string> list_expansion;
	size_t list_end = preamble_end + 1;
	while (true) {
		auto sublist_expansion = expand_pattern(pattern, list_end, true);
		if (sublist_expansion.empty()) {
			return {};
		}

		list_expansion.insert(list_expansion.end(), sublist_expansion.begin(), sublist_expansion.end());
		if (pattern[list_end] == SYM_LIST_DELIM) {
			list_end++;
			continue;
		}
		else if (pattern[list_end] == SYM_LIST_END) {
			break;
		}
		else {
			return {};
		}
	}

	size_t postscript_start = list_end + 1;
	auto postscript_expansion = expand_pattern(pattern, postscript_start, nested);
	if (postscript_expansion.empty()) {
		return {};
	}

	std::vector<std::string> expansion;
	expansion.reserve(list_expansion.size() * postscript_expansion.size());
	for (auto list_iter = list_expansion.begin(); list_iter != list_expansion.end(); ++list_iter) {
		for (auto postscript_iter = postscript_expansion.begin(); postscript_iter != postscript_expansion.end(); ++postscript_iter) {
			expansion.push_back(preamble + *list_iter + *postscript_iter);
		}
	}

	pos = postscript_start;
	return expansion;
}

static
std::vector<std::string> expand_pattern(const std::string& pattern) {
	size_t pos = 0;
	auto expansion = expand_pattern(pattern.c_str(), pos, false);

	if (pos == pattern.size()) {
		return expansion;
	}
	else {
		return {};
	}
}

std::shared_ptr<opts_t> opts;

opts_t::opts_t()
	: m_dump_interval(750, chrono::time_unit::MSEC)
	, m_core_enable(true)
{
}

rapidjson::Value* opts_t::select_pattern(const std::string& name) const {
	for (auto pattern_group_iter = m_patterns.begin(); pattern_group_iter != m_patterns.end(); ++pattern_group_iter) {
		auto& pattern_group = pattern_group_iter->first;
		auto* pattern_cfg = pattern_group_iter->second;
		for (auto pattern_iter = pattern_group.begin(); pattern_iter != pattern_group.end(); ++pattern_iter) {
			if (fnmatch(pattern_iter->c_str(), name.c_str(), 0) == 0) {
				return pattern_cfg;
			}
		}
	}

	return nullptr;
}

__attribute__((constructor(300)))
static void init_opts() {
	initialize();
}

static
void reset() {
	opts.reset(new opts_t());
}

void initialize() {
	reset();
}

void finalize() {
	reset();
}

}} // namespace handystats::config


namespace handystats {

bool config_json(const char* config_data) {
	if (config::opts.use_count() == 0) {
		return false;
	}
	auto opts = config::opts;

	rapidjson::Document& cfg = opts->m_source;
	cfg.Parse<0>(config_data);
	if (cfg.HasParseError()) {
		// std::cerr << "Unable to parse configuration json: " << opts->m_source.GetParseError() << std::endl;
		return false;
	}

	if (!cfg.IsObject()) {
		return false;
	}

	if (cfg.HasMember("enable")) {
		rapidjson::Value& enable = cfg["enable"];
		if (enable.IsBool()) {
			opts->m_core_enable = enable.GetBool();
		}
		else {
			return false;
		}
	}

	if (cfg.HasMember("dump-interval")) {
		rapidjson::Value& dump_interval = cfg["dump-interval"];
		if (dump_interval.IsUint64()) {
			opts->m_dump_interval = chrono::milliseconds(dump_interval.GetUint64());
		}
		else {
			return false;
		}
	}

	if (cfg.HasMember("defaults")) {
		rapidjson::Value& defaults = cfg["defaults"];
		config::apply(defaults, opts->m_statistics);
		config::apply(defaults, opts->m_gauge.values);
		config::apply(defaults, opts->m_counter.values);
		config::apply(defaults, opts->m_timer.values);
	}

	if (cfg.HasMember("gauge")) {
		rapidjson::Value& gauge = cfg["gauge"];
		config::apply(gauge, opts->m_gauge);
	}

	if (cfg.HasMember("counter")) {
		rapidjson::Value& counter = cfg["counter"];
		config::apply(counter, opts->m_counter);
	}

	if (cfg.HasMember("timer")) {
		rapidjson::Value& timer = cfg["timer"];
		config::apply(timer, opts->m_timer);
	}

	for (auto config_member = opts->m_source.MemberBegin(); config_member != opts->m_source.MemberEnd(); ++config_member) {
		rapidjson::Value& member_name = config_member->name;
		rapidjson::Value& member_value = config_member->value;

		// skip reserved sections
		if (strcmp(member_name.GetString(), "enable") == 0
			|| strcmp(member_name.GetString(), "dump-interval") == 0
			|| strcmp(member_name.GetString(), "defaults") == 0
			|| strcmp(member_name.GetString(), "gauge") == 0
			|| strcmp(member_name.GetString(), "counter") == 0
			|| strcmp(member_name.GetString(), "timer") == 0
			)
		{
			continue;
		}

		// pattern
		std::vector<std::string> expansion = config::expand_pattern(member_name.GetString());
		if (expansion.empty()) {
			// invalid pattern
			return false;
		}

		config::statistics test_opts;
		config::apply(member_value, test_opts);
		// TODO: check test_opts

		opts->m_patterns.push_back(std::make_pair(expansion, &member_value));
	}

	return true;
}

bool config_file(const char* filename) {
	std::ifstream input(filename, std::ios::in | std::ios::binary);
	if (!input) {
		std::cerr << "Unable to open configuration file " << filename << std::endl;
		return false;
	}

	std::string config_data;
	input.seekg(0, std::ios::end);
	config_data.resize(input.tellg());
	input.seekg(0, std::ios::beg);
	input.read(&config_data[0], config_data.size());
	input.close();

	return config_json(config_data.c_str());
}

} // namespace handystats


extern "C" {

int handystats_config_file(const char* file) {
	if (handystats::core) {
		return 0;
	}

	return handystats::config_file(file);
}

int handystats_config_json(const char* config_data) {
	if (handystats::core) {
		return 0;
	}

	return handystats::config_json(config_data);
}

} // extern "C"
