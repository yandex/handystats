/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <mutex>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

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
//
// may throw std::logic_error
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

		list_expansion.insert(list_expansion.end(), sublist_expansion.begin(), sublist_expansion.end());
		if (pattern[list_end] == SYM_LIST_DELIM) {
			list_end++;
			continue;
		}
		else if (pattern[list_end] == SYM_LIST_END) {
			break;
		}
		else {
			std::ostringstream err;
			err << "Invalid symbol at pos " << list_end << " pattern '" << pattern << "'";
			throw std::logic_error(err.str());
		}
	}

	size_t postscript_start = list_end + 1;
	auto postscript_expansion = expand_pattern(pattern, postscript_start, nested);

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
		std::ostringstream err;
		err << "Parsing stopped at pos " << pos << " pattern '" << pattern << "'";
		throw std::logic_error(err.str());
	}
}

std::shared_ptr<opts_t> opts __attribute__((init_priority(200)));
std::shared_ptr<std::string> error __attribute__((init_priority(200)));

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
	error.reset();
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
	try {
		auto opts = config::opts;

		rapidjson::Document& cfg = opts->m_source;
		cfg.Parse<0>(config_data);
		if (cfg.HasParseError()) {
			std::ostringstream err;
			err << "Unable to parse configuration json: " << opts->m_source.GetParseError();
			throw std::logic_error(err.str());
		}

		if (!cfg.IsObject()) {
			std::ostringstream err;
			err << "Config data must be JSON object";
			throw std::logic_error(err.str());
		}

		if (cfg.HasMember("enable")) {
			rapidjson::Value& enable = cfg["enable"];
			if (enable.IsBool()) {
				opts->m_core_enable = enable.GetBool();
			}
			else {
				std::ostringstream err;
				err << "'enable' option must be boolean";
				throw std::logic_error(err.str());
			}
		}

		if (cfg.HasMember("dump-interval")) {
			rapidjson::Value& dump_interval = cfg["dump-interval"];
			if (dump_interval.IsUint64()) {
				opts->m_dump_interval = chrono::milliseconds(dump_interval.GetUint64());
			}
			else {
				std::ostringstream err;
				err << "'dump-interval' option must be unsigned integer";
				throw std::logic_error(err.str());
			}
		}

		if (cfg.HasMember("defaults")) {
			rapidjson::Value& defaults = cfg["defaults"];
			try {
				config::apply(defaults, opts->m_statistics);
				config::apply(defaults, opts->m_gauge.values);
				config::apply(defaults, opts->m_counter.values);
				config::apply(defaults, opts->m_timer.values);
			}
			catch (const std::logic_error& err) {
				throw std::logic_error(std::string("Error in 'defaults' section: ") + std::string(err.what()));
			}
		}

		if (cfg.HasMember("gauge")) {
			rapidjson::Value& gauge = cfg["gauge"];
			try {
				config::apply(gauge, opts->m_gauge);
			}
			catch (const std::logic_error& err) {
				throw std::logic_error(std::string("Error in 'gauge' section: ") + std::string(err.what()));
			}
		}

		if (cfg.HasMember("counter")) {
			rapidjson::Value& counter = cfg["counter"];
			try {
				config::apply(counter, opts->m_counter);
			}
			catch (const std::logic_error& err) {
				throw std::logic_error(std::string("Error in 'counter' section: ") + std::string(err.what()));
			}
		}

		if (cfg.HasMember("timer")) {
			rapidjson::Value& timer = cfg["timer"];
			try {
				config::apply(timer, opts->m_timer);
			}
			catch (const std::logic_error& err) {
				throw std::logic_error(std::string("Error in 'timer' section: ") + std::string(err.what()));
			}
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

			config::statistics test_opts;
			try {
				config::apply(member_value, test_opts);
			}
			catch (const std::logic_error& err) {
				throw std::logic_error(
						std::string("Error in pattern '") + std::string(member_name.GetString()) + std::string("': ") +
						std::string(err.what())
					);
			}

			opts->m_patterns.push_back(std::make_pair(expansion, &member_value));
		}

		return true;
	}
	catch (const std::logic_error& err) {
		config::error.reset(new std::string(err.what()));
		config::opts.reset(new config::opts_t());

		return false;
	}
}

bool config_file(const char* filename) {
	try {
		std::ifstream input(filename, std::ios::in | std::ios::binary);
		if (!input) {
			std::ostringstream err;
			err << "Unable to open configuration file '" << filename << "'";
			throw std::logic_error(err.str());
		}

		std::string config_data;
		input.seekg(0, std::ios::end);
		config_data.resize(input.tellg());
		input.seekg(0, std::ios::beg);
		input.read(&config_data[0], config_data.size());
		input.close();

		return config_json(config_data.c_str());
	}
	catch (const std::logic_error& err) {
		config::error.reset(new std::string(err.what()));
		config::opts.reset(new config::opts_t());

		return false;
	}
}

const char* config_error() {
	if (config::error.use_count() == 0) {
		return nullptr;
	}
	else {
		return config::error->c_str();
	}
}

} // namespace handystats


extern "C" {

int handystats_config_file(const char* file) {
	return handystats::config_file(file);
}

int handystats_config_json(const char* config_data) {
	return handystats::config_json(config_data);
}

const char* handystats_config_error() {
	return handystats::config_error();
}

} // extern "C"
