#include <gtest/gtest.h>

#include <handystats/rapidjson/allocators.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics/attribute.hpp>

#include <handystats/json/attribute_json_writer.hpp>

using namespace handystats;

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
