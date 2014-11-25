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

#include <memory>

#include <gtest/gtest.h>

#include <handystats/attribute.hpp>
#include <handystats/core.hpp>

#include "events/event_message_impl.hpp"
#include "events/attribute_impl.hpp"

class AttributeEventsTest: public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

using namespace handystats::events::attribute;

TEST_F(AttributeEventsTest, TestAttributeSetDoubleEvent) {
	const char* attribute_name = "attr.test";
	const double value = 0.75;
	auto message = create_set_event(attribute_name, handystats::attribute::value_type(value), handystats::attribute::clock::now());

	ASSERT_EQ(message->destination_name, attribute_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::ATTRIBUTE);

	ASSERT_EQ(message->event_type, event_type::SET);
	ASSERT_NEAR(
			boost::get<double>(*reinterpret_cast<handystats::attribute::value_type*>(message->event_data)),
			value,
			1E-6
		);

	delete_event_message(message);
}

TEST_F(AttributeEventsTest, TestAttributeSetBoolEvent) {
	const char* attribute_name = "attr.test";
	const bool value = true;
	auto message = create_set_event(attribute_name, handystats::attribute::value_type(value), handystats::attribute::clock::now());

	ASSERT_EQ(message->destination_name, attribute_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::ATTRIBUTE);

	ASSERT_EQ(message->event_type, event_type::SET);
	ASSERT_EQ(
			boost::get<bool>(*reinterpret_cast<handystats::attribute::value_type*>(message->event_data)),
			value
		);

	delete_event_message(message);
}

TEST_F(AttributeEventsTest, TestAttributeSetIntEvent) {
	const char* attribute_name = "attr.test";
	const int value = -123;
	auto message = create_set_event(attribute_name, handystats::attribute::value_type(value), handystats::attribute::clock::now());

	ASSERT_EQ(message->destination_name, attribute_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::ATTRIBUTE);

	ASSERT_EQ(message->event_type, event_type::SET);
	ASSERT_EQ(
			boost::get<int>(*reinterpret_cast<handystats::attribute::value_type*>(message->event_data)),
			value
		);

	delete_event_message(message);
}

TEST_F(AttributeEventsTest, TestAttributeSetUintEvent) {
	const char* attribute_name = "attr.test";
	const unsigned value = 123;
	auto message = create_set_event(attribute_name, handystats::attribute::value_type(value), handystats::attribute::clock::now());

	ASSERT_EQ(message->destination_name, attribute_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::ATTRIBUTE);

	ASSERT_EQ(message->event_type, event_type::SET);
	ASSERT_EQ(
			boost::get<unsigned>(*reinterpret_cast<handystats::attribute::value_type*>(message->event_data)),
			value
		);

	delete_event_message(message);
}

TEST_F(AttributeEventsTest, TestAttributeSetInt64Event) {
	const char* attribute_name = "attr.test";
	const int64_t value = -1e13;
	auto message = create_set_event(attribute_name, handystats::attribute::value_type(value), handystats::attribute::clock::now());

	ASSERT_EQ(message->destination_name, attribute_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::ATTRIBUTE);

	ASSERT_EQ(message->event_type, event_type::SET);
	ASSERT_EQ(
			boost::get<int64_t>(*reinterpret_cast<handystats::attribute::value_type*>(message->event_data)),
			value
		);

	delete_event_message(message);
}

TEST_F(AttributeEventsTest, TestAttributeSetUint64Event) {
	const char* attribute_name = "attr.test";
	const uint64_t value = 1e13;
	auto message = create_set_event(attribute_name, handystats::attribute::value_type(value), handystats::attribute::clock::now());

	ASSERT_EQ(message->destination_name, attribute_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::ATTRIBUTE);

	ASSERT_EQ(message->event_type, event_type::SET);
	ASSERT_EQ(
			boost::get<uint64_t>(*reinterpret_cast<handystats::attribute::value_type*>(message->event_data)),
			value
		);

	delete_event_message(message);
}

TEST_F(AttributeEventsTest, TestAttributeSetStringEvent) {
	const char* attribute_name = "attr.test";
	const char* value = "attr.test.value";
	auto message = create_set_event(attribute_name, handystats::attribute::value_type(std::string(value)), handystats::attribute::clock::now());

	ASSERT_EQ(message->destination_name, attribute_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::ATTRIBUTE);

	ASSERT_EQ(message->event_type, event_type::SET);
	ASSERT_EQ(
			boost::get<std::string>(*reinterpret_cast<handystats::attribute::value_type*>(message->event_data)),
			value
		);

	delete_event_message(message);
}
