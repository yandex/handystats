/*
 * Copyright (c) YANDEX LLC. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

#include <gtest/gtest.h>

#include <rapidjson/allocators.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics/attribute.hpp>

#include <handystats/json/attribute_json_writer.hpp>

using namespace handystats::metrics;

TEST(AttributeTest, TestAttributeConstruction) {
	attribute sample_attr;
	const int value = -10;
	sample_attr.set(value);

	ASSERT_EQ(boost::get<int>(sample_attr.value()), value);
}

TEST(AttributeTest, TestAttributeSetMethods) {
	attribute attr;

	{ // bool
		const bool value = false;
		attr.set(value);
		ASSERT_EQ(boost::get<decltype(value)>(attr.value()), value);
	}
	{ // double
		const double value = 1.25;
		attr.set(value);
		ASSERT_NEAR(boost::get<decltype(value)>(attr.value()), value, 1E-9);
	}
	{ // int
		const int value = -123;
		attr.set(value);
		ASSERT_EQ(boost::get<decltype(value)>(attr.value()), value);
	}
	{ // unsigned
		const unsigned value = 123;
		attr.set(value);
		ASSERT_EQ(boost::get<decltype(value)>(attr.value()), value);
	}
	{ // int64_t
		const int64_t value = -1e18;
		attr.set(value);
		ASSERT_EQ(boost::get<decltype(value)>(attr.value()), value);
	}
	{ // uint64_t
		const uint64_t value = 1e18;
		attr.set(value);
		ASSERT_EQ(boost::get<decltype(value)>(attr.value()), value);
	}
	{ // string
		const std::string value = "test.string";
		attr.set(value);
		ASSERT_EQ(boost::get<decltype(value)>(attr.value()), value);
	}
	{ // char*
		const char* value = "test.const.char";
		attr.set(value);
		ASSERT_EQ(boost::get<std::string>(attr.value()), value);
	}
}
