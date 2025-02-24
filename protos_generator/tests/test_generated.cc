// Copyright (c) 2009-2021, Google LLC
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Google LLC nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL Google LLC BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <limits>

#include "gtest/gtest.h"
#include "protos_generator/tests/child_model.upb.proto.h"
#include "protos_generator/tests/no_package.upb.proto.h"
#include "protos_generator/tests/test_model.upb.proto.h"
#include "upb/string_view.h"

using ::protos_generator::test::protos::ChildModel1;
using ::protos_generator::test::protos::other_ext;
using ::protos_generator::test::protos::RED;
using ::protos_generator::test::protos::TestEnum;
using ::protos_generator::test::protos::TestModel;
using ::protos_generator::test::protos::TestModel_Category_IMAGES;
using ::protos_generator::test::protos::TestModel_Category_NEWS;
using ::protos_generator::test::protos::TestModel_Category_VIDEO;
using ::protos_generator::test::protos::theme;
using ::protos_generator::test::protos::ThemeExtension;

TEST(CppGeneratedCode, Constructor) { TestModel test_model; }

TEST(CppGeneratedCode, MessageEnum) { EXPECT_EQ(5, TestModel_Category_IMAGES); }

TEST(CppGeneratedCode, ImportedEnum) { EXPECT_EQ(3, TestEnum::DEVICE_MONITOR); }

TEST(CppGeneratedCode, Enum) { EXPECT_EQ(1, RED); }

TEST(CppGeneratedCode, EnumNoPackage) { EXPECT_EQ(1, ::protos_CELSIUS); }

TEST(CppGeneratedCode, ArenaConstructor) {
  ::protos::Arena arena;
  auto testModel = ::protos::CreateMessage<TestModel>(arena);
  EXPECT_EQ(false, testModel.has_b1());
}

TEST(CppGeneratedCode, Booleans) {
  ::protos::Arena arena;
  auto testModel = ::protos::CreateMessage<TestModel>(arena);
  EXPECT_FALSE(testModel.b1());
  testModel.set_b1(true);
  EXPECT_TRUE(testModel.b1());
  testModel.set_b1(false);
  EXPECT_FALSE(testModel.b1());
  testModel.set_b1(true);
  EXPECT_TRUE(testModel.b1());
  testModel.clear_b1();
  EXPECT_FALSE(testModel.has_b1());
}

TEST(CppGeneratedCode, ScalarInt32) {
  ::protos::Arena arena;
  auto testModel = ::protos::CreateMessage<TestModel>(arena);
  // Test int32 defaults.
  EXPECT_EQ(testModel.value(), 0);
  EXPECT_FALSE(testModel.has_value());
  // Floating point defautls.
  EXPECT_EQ(std::numeric_limits<float>::infinity(),
            testModel.float_value_with_default());
  EXPECT_EQ(-std::numeric_limits<double>::infinity(),
            testModel.double_value_with_default());

  // Set value.
  testModel.set_value(5);
  EXPECT_TRUE(testModel.has_value());
  EXPECT_EQ(testModel.value(), 5);
  // Change value.
  testModel.set_value(10);
  EXPECT_TRUE(testModel.has_value());
  EXPECT_EQ(testModel.value(), 10);
  // Clear value.
  testModel.clear_value();
  EXPECT_FALSE(testModel.has_value());
  EXPECT_EQ(testModel.value(), 0);
}

const char kTestStr1[] = "abcdefg";
const char kTestStr2[] = "just another test string";

TEST(CppGeneratedCode, Strings) {
  TestModel testModel;
  testModel.set_str1(kTestStr1);
  testModel.set_str2(kTestStr2);
  EXPECT_EQ(testModel.str1(), kTestStr1);
  EXPECT_EQ(testModel.str2(), kTestStr2);
  EXPECT_TRUE(testModel.has_str1());
  EXPECT_TRUE(testModel.has_str2());

  testModel.clear_str1();
  EXPECT_FALSE(testModel.has_str1());
  EXPECT_TRUE(testModel.has_str2());
}

TEST(CppGeneratedCode, ScalarUInt32) {
  ::protos::Arena arena;
  auto testModel = ::protos::CreateMessage<TestModel>(arena);
  // Test defaults.
  EXPECT_EQ(testModel.optional_uint32(), 0);
  EXPECT_FALSE(testModel.has_optional_uint32());
  // Set value.
  testModel.set_optional_uint32(0xA0001000);
  EXPECT_TRUE(testModel.has_optional_uint32());
  EXPECT_EQ(testModel.optional_uint32(), 0xA0001000);
  // Change value.
  testModel.set_optional_uint32(0x70002000);
  EXPECT_TRUE(testModel.has_optional_uint32());
  EXPECT_EQ(testModel.optional_uint32(), 0x70002000);
  // Clear value.
  testModel.clear_optional_uint32();
  EXPECT_FALSE(testModel.has_optional_uint32());
  EXPECT_EQ(testModel.optional_uint32(), 0);
}

TEST(CppGeneratedCode, ScalarInt64) {
  ::protos::Arena arena;
  auto testModel = ::protos::CreateMessage<TestModel>(arena);
  // Test defaults.
  EXPECT_EQ(testModel.optional_int64(), 0);
  EXPECT_FALSE(testModel.has_optional_int64());
  // Set value.
  testModel.set_optional_int64(0xFF00CCDDA0001000);
  EXPECT_TRUE(testModel.has_optional_int64());
  EXPECT_EQ(testModel.optional_int64(), 0xFF00CCDDA0001000);
  // Change value.
  testModel.set_optional_int64(0xFF00CCDD70002000);
  EXPECT_TRUE(testModel.has_optional_int64());
  EXPECT_EQ(testModel.optional_int64(), 0xFF00CCDD70002000);
  // Clear value.
  testModel.clear_optional_int64();
  EXPECT_FALSE(testModel.has_optional_int64());
  EXPECT_EQ(testModel.optional_int64(), 0);
  // Set after clear.
  testModel.set_optional_int64(0xFF00CCDDA0001000);
  EXPECT_TRUE(testModel.has_optional_int64());
  EXPECT_EQ(testModel.optional_int64(), 0xFF00CCDDA0001000);
}

TEST(CppGeneratedCode, ScalarFloat) {
  ::protos::Arena arena;
  auto testModel = ::protos::CreateMessage<TestModel>(arena);
  // Test defaults.
  EXPECT_EQ(testModel.optional_float(), 0.0f);
  EXPECT_FALSE(testModel.has_optional_float());
  EXPECT_EQ(std::numeric_limits<float>::infinity(),
            testModel.float_value_with_default());
  EXPECT_EQ(-std::numeric_limits<double>::infinity(),
            testModel.double_value_with_default());
  // Set value.
  testModel.set_optional_float(3.14159265f);
  EXPECT_TRUE(testModel.has_optional_float());
  EXPECT_NEAR(testModel.optional_float(), 3.14159265f, 1e-9f);
  // Change value.
  testModel.set_optional_float(-2.0f);
  EXPECT_TRUE(testModel.has_optional_float());
  EXPECT_NEAR(testModel.optional_float(), -2, 1e-9f);
  // Clear value.
  testModel.clear_optional_float();
  EXPECT_FALSE(testModel.has_optional_float());
  EXPECT_EQ(testModel.optional_float(), 0.0f);
  // Set after clear.
  testModel.set_optional_float(3.14159265f);
  EXPECT_TRUE(testModel.has_optional_float());
  EXPECT_NEAR(testModel.optional_float(), 3.14159265f, 1e-9f);
}

TEST(CppGeneratedCode, ScalarDouble) {
  ::protos::Arena arena;
  auto testModel = ::protos::CreateMessage<TestModel>(arena);
  // Test defaults.
  EXPECT_EQ(testModel.optional_double(), 0.0);
  EXPECT_FALSE(testModel.has_optional_double());
  // Set value.
  testModel.set_optional_double(3.141592653589793);
  EXPECT_TRUE(testModel.has_optional_double());
  EXPECT_NEAR(testModel.optional_double(), 3.141592653589793, 1e-16f);
  // Change value.
  testModel.set_optional_double(-1.0);
  EXPECT_TRUE(testModel.has_optional_double());
  EXPECT_NEAR(testModel.optional_double(), -1.0, 1e-16f);
  // Clear value.
  testModel.clear_optional_double();
  EXPECT_FALSE(testModel.has_optional_double());
  EXPECT_EQ(testModel.optional_double(), 0.0f);
  // Set after clear.
  testModel.set_optional_double(3.141592653589793);
  EXPECT_TRUE(testModel.has_optional_double());
  EXPECT_NEAR(testModel.optional_double(), 3.141592653589793, 1e-16f);
}

TEST(CppGeneratedCode, Enums) {
  ::protos::Arena arena;
  auto testModel = ::protos::CreateMessage<TestModel>(arena);

  // Check enum default value.
  EXPECT_EQ(TestModel_Category_IMAGES, 5);

  // Test defaults.
  EXPECT_FALSE(testModel.has_category());
  EXPECT_EQ(testModel.category(), TestModel_Category_IMAGES);
  // Set value.
  testModel.set_category(TestModel_Category_NEWS);
  EXPECT_TRUE(testModel.has_category());
  EXPECT_EQ(testModel.category(), TestModel_Category_NEWS);
  // Change value.
  testModel.set_category(TestModel_Category_VIDEO);
  EXPECT_TRUE(testModel.has_category());
  EXPECT_EQ(testModel.category(), TestModel_Category_VIDEO);
  // Clear value.
  testModel.clear_category();
  EXPECT_FALSE(testModel.has_category());
  EXPECT_EQ(testModel.category(), TestModel_Category_IMAGES);
  // Set after clear.
  testModel.set_category(TestModel_Category_VIDEO);
  EXPECT_TRUE(testModel.has_category());
  EXPECT_EQ(testModel.category(), TestModel_Category_VIDEO);
}

TEST(CppGeneratedCode, FieldWithDefaultValue) {
  ::protos::Arena arena;
  auto testModel = ::protos::CreateMessage<TestModel>(arena);

  EXPECT_FALSE(testModel.has_int_value_with_default());
  EXPECT_EQ(testModel.int_value_with_default(), 65);
  testModel.set_int_value_with_default(10);
  EXPECT_EQ(testModel.int_value_with_default(), 10);

  EXPECT_FALSE(testModel.has_string_value_with_default());
  EXPECT_EQ(testModel.string_value_with_default(), "hello");
  testModel.set_string_value_with_default("new string");
  EXPECT_EQ(testModel.string_value_with_default(), "new string");
}

TEST(CppGeneratedCode, OneOfFields) {
  ::protos::Arena arena;
  auto test_model = ::protos::CreateMessage<TestModel>(arena);

  EXPECT_FALSE(test_model.has_oneof_member1());
  EXPECT_FALSE(test_model.has_oneof_member2());

  test_model.set_oneof_member1("one of string");
  EXPECT_TRUE(test_model.has_oneof_member1());
  EXPECT_FALSE(test_model.has_oneof_member2());
  EXPECT_EQ(test_model.oneof_member1(), "one of string");

  test_model.set_oneof_member2(true);
  EXPECT_FALSE(test_model.has_oneof_member1());
  EXPECT_TRUE(test_model.has_oneof_member2());
  EXPECT_EQ(test_model.oneof_member2(), true);

  test_model.clear_oneof_member2();
  EXPECT_FALSE(test_model.has_oneof_member1());
  EXPECT_FALSE(test_model.has_oneof_member2());
  EXPECT_EQ(test_model.oneof_member1(), "");
  EXPECT_EQ(test_model.oneof_member2(), false);
}

TEST(CppGeneratedCode, Messages) {
  ::protos::Arena arena;
  auto test_model = ::protos::CreateMessage<TestModel>(arena);
  EXPECT_EQ(false, test_model.has_child_model_1());
  auto child_model = test_model.child_model_1();
  EXPECT_EQ(false, child_model->has_child_b1());
  EXPECT_EQ(false, child_model->child_b1());
  auto mutable_child = test_model.mutable_child_model_1();
  mutable_child->set_child_b1(true);
  EXPECT_EQ(true, mutable_child->has_child_b1());
  EXPECT_EQ(true, mutable_child->child_b1());
  // The View should not change due to mutation since it
  // is default_instance.
  EXPECT_EQ(false, child_model->has_child_b1());
  // Readonly View should now show change.
  child_model = test_model.child_model_1();
  EXPECT_EQ(true, child_model->has_child_b1());
  EXPECT_EQ(true, child_model->child_b1());
  // Clear message field.
  EXPECT_EQ(true, test_model.has_child_model_1());
  test_model.clear_child_model_1();
  EXPECT_EQ(false, test_model.has_child_model_1());
}

TEST(CppGeneratedCode, NestedMessages) {
  ::protos::Arena arena;
  auto test_model = ::protos::CreateMessage<TestModel>(arena);
  auto nested_child = test_model.nested_child_1();
  EXPECT_EQ(0, nested_child->nested_child_name().size());
  auto mutable_nested_child = test_model.mutable_nested_child_1();
  EXPECT_EQ(false, mutable_nested_child->has_nested_child_name());
  mutable_nested_child->set_nested_child_name(kTestStr1);
  EXPECT_EQ(true, mutable_nested_child->has_nested_child_name());
}

TEST(CppGeneratedCode, RepeatedMessages) {
  ::protos::Arena arena;
  auto test_model = ::protos::CreateMessage<TestModel>(arena);
  EXPECT_EQ(0, test_model.child_model_2_size());
  // Should be able to clear repeated field when empty.
  test_model.clear_child_model_2();
  EXPECT_EQ(0, test_model.child_model_2_size());
  // Add 2 children.
  auto new_child = test_model.add_child_model_2();
  EXPECT_EQ(true, new_child.ok());
  new_child.value()->set_child_str1(kTestStr1);
  new_child = test_model.add_child_model_2();
  EXPECT_EQ(true, new_child.ok());
  new_child.value()->set_child_str1(kTestStr2);
  EXPECT_EQ(2, test_model.child_model_2_size());
  // Mutable access.
  auto mutable_first = test_model.mutable_child_model_2(0);
  EXPECT_EQ(mutable_first->child_str1(), kTestStr1);
  mutable_first->set_child_str1("change1");
  auto mutable_second = test_model.mutable_child_model_2(1);
  EXPECT_EQ(mutable_second->child_str1(), kTestStr2);
  mutable_second->set_child_str1("change2");
  // Check mutations using views.
  auto view_first = test_model.child_model_2(0);
  EXPECT_EQ(view_first->child_str1(), "change1");
  auto view_second = test_model.child_model_2(1);
  EXPECT_EQ(view_second->child_str1(), "change2");
}

TEST(CppGeneratedCode, RepeatedScalar) {
  ::protos::Arena arena;
  auto test_model = ::protos::CreateMessage<TestModel>(arena);
  EXPECT_EQ(0, test_model.value_array_size());
  // Should be able to clear repeated field when empty.
  test_model.clear_value_array();
  EXPECT_EQ(0, test_model.value_array_size());
  // Add 2 children.
  EXPECT_EQ(true, test_model.add_value_array(5));
  EXPECT_EQ(true, test_model.add_value_array(6));
  EXPECT_EQ(2, test_model.value_array_size());
  EXPECT_EQ(5, test_model.value_array(0));
  EXPECT_EQ(6, test_model.value_array(1));
  EXPECT_EQ(true, test_model.resize_value_array(3));
  EXPECT_EQ(3, test_model.value_array_size());
  test_model.set_value_array(2, 7);
  EXPECT_EQ(5, test_model.value_array(0));
  EXPECT_EQ(6, test_model.value_array(1));
  EXPECT_EQ(7, test_model.value_array(2));
}

TEST(CppGeneratedCode, RepeatedStrings) {
  ::protos::Arena arena;
  auto test_model = ::protos::CreateMessage<TestModel>(arena);
  EXPECT_EQ(0, test_model.repeated_string_size());
  // Should be able to clear repeated field when empty.
  test_model.clear_repeated_string();
  EXPECT_EQ(0, test_model.repeated_string_size());
  // Add 2 children.
  EXPECT_EQ(true, test_model.add_repeated_string("Hello"));
  EXPECT_EQ(true, test_model.add_repeated_string("World"));
  EXPECT_EQ(2, test_model.repeated_string_size());
  EXPECT_EQ("Hello", test_model.repeated_string(0));
  EXPECT_EQ("World", test_model.repeated_string(1));
  EXPECT_EQ(true, test_model.resize_repeated_string(3));
  EXPECT_EQ(3, test_model.repeated_string_size());
  test_model.set_repeated_string(2, "Test");
  EXPECT_EQ("Hello", test_model.repeated_string(0));
  EXPECT_EQ("World", test_model.repeated_string(1));
  EXPECT_EQ("Test", test_model.repeated_string(2));
}

TEST(CppGeneratedCode, MessageMapInt32KeyMessageValue) {
  const int key_test_value = 3;
  ::protos::Arena arena;
  auto test_model = ::protos::CreateMessage<TestModel>(arena);
  EXPECT_EQ(0, test_model.child_map_size());
  test_model.clear_child_map();
  EXPECT_EQ(0, test_model.child_map_size());
  auto child_model1 = ::protos::CreateMessage<ChildModel1>(arena);
  child_model1.set_child_str1("abc");
  test_model.set_child_map(key_test_value, child_model1);
  auto map_result = test_model.get_child_map(key_test_value);
  EXPECT_EQ(true, map_result.ok());
  EXPECT_EQ("abc", map_result.value()->child_str1());
  test_model.delete_child_map(key_test_value);
  auto map_result_after_delete = test_model.get_child_map(key_test_value);
  EXPECT_EQ(false, map_result_after_delete.ok());
}

TEST(CppGeneratedCode, MessageMapStringKeyAndStringValue) {
  ::protos::Arena arena;
  auto test_model = ::protos::CreateMessage<TestModel>(arena);
  EXPECT_EQ(0, test_model.str_to_str_map_size());
  test_model.clear_str_to_str_map();
  EXPECT_EQ(0, test_model.str_to_str_map_size());
  test_model.set_str_to_str_map("first", "abc");
  test_model.set_str_to_str_map("second", "def");
  auto result = test_model.get_str_to_str_map("second");
  EXPECT_EQ(true, result.ok());
  EXPECT_EQ("def", result.value());
  test_model.delete_str_to_str_map("first");
  auto result_after_delete = test_model.get_str_to_str_map("first");
  EXPECT_EQ(false, result_after_delete.ok());
}

TEST(CppGeneratedCode, MessageMapStringKeyAndInt32Value) {
  ::protos::Arena arena;
  auto test_model = ::protos::CreateMessage<TestModel>(arena);
  EXPECT_EQ(0, test_model.str_to_int_map_size());
  test_model.clear_str_to_int_map();
  EXPECT_EQ(0, test_model.str_to_int_map_size());
  test_model.set_str_to_int_map("first", 10);
  test_model.set_str_to_int_map("second", 20);
  auto result = test_model.get_str_to_int_map("second");
  EXPECT_EQ(true, result.ok());
  EXPECT_EQ(20, result.value());
  test_model.delete_str_to_int_map("first");
  auto result_after_delete = test_model.get_str_to_int_map("first");
  EXPECT_EQ(false, result_after_delete.ok());
}

TEST(CppGeneratedCode, HasExtension) {
  TestModel model;
  EXPECT_EQ(false, ::protos::HasExtension(model, theme));
}

TEST(CppGeneratedCode, HasExtensionPtr) {
  TestModel model;
  EXPECT_EQ(false, ::protos::HasExtension(model.recursive_child(), theme));
}

TEST(CppGeneratedCode, ClearExtensionWithEmptyExtension) {
  TestModel model;
  EXPECT_EQ(false, ::protos::HasExtension(model, theme));
  ::protos::ClearExtension(model, theme);
  EXPECT_EQ(false, ::protos::HasExtension(model, theme));
}

TEST(CppGeneratedCode, ClearExtensionWithEmptyExtensionPtr) {
  TestModel model;
  ::protos::Ptr<TestModel> recursive_child = model.mutable_recursive_child();
  ::protos::ClearExtension(recursive_child, theme);
  EXPECT_EQ(false, ::protos::HasExtension(recursive_child, theme));
}

TEST(CppGeneratedCode, SetExtension) {
  TestModel model;
  ThemeExtension extension1;
  extension1.set_ext_name("Hello World");
  EXPECT_EQ(false, ::protos::HasExtension(model, theme));
  EXPECT_EQ(true, ::protos::SetExtension(model, theme, extension1).ok());
  EXPECT_EQ(true, ::protos::HasExtension(model, theme));
}

TEST(CppGeneratedCode, SetExtensionOnMutableChild) {
  TestModel model;
  ThemeExtension extension1;
  extension1.set_ext_name("Hello World");
  EXPECT_EQ(false,
            ::protos::HasExtension(model.mutable_recursive_child(), theme));
  EXPECT_EQ(true, ::protos::SetExtension(model.mutable_recursive_child(), theme,
                                         extension1)
                      .ok());
  EXPECT_EQ(true,
            ::protos::HasExtension(model.mutable_recursive_child(), theme));
}

TEST(CppGeneratedCode, GetExtension) {
  TestModel model;
  ThemeExtension extension1;
  extension1.set_ext_name("Hello World");
  EXPECT_EQ(false, ::protos::HasExtension(model, theme));
  EXPECT_EQ(true, ::protos::SetExtension(model, theme, extension1).ok());
  EXPECT_EQ("Hello World",
            ::protos::GetExtension(model, theme).value()->ext_name());
}

TEST(CppGeneratedCode, GetExtensionOnMutableChild) {
  TestModel model;
  ThemeExtension extension1;
  extension1.set_ext_name("Hello World");
  ::protos::Ptr<TestModel> mutable_recursive_child =
      model.mutable_recursive_child();
  EXPECT_EQ(false, ::protos::HasExtension(mutable_recursive_child, theme));
  EXPECT_EQ(
      true,
      ::protos::SetExtension(mutable_recursive_child, theme, extension1).ok());
  EXPECT_EQ("Hello World",
            ::protos::GetExtension(mutable_recursive_child, theme)
                .value()
                ->ext_name());
}

TEST(CppGeneratedCode, GetExtensionOnImmutableChild) {
  TestModel model;
  ThemeExtension extension1;
  extension1.set_ext_name("Hello World");
  ::protos::Ptr<TestModel> mutable_recursive_child =
      model.mutable_recursive_child();
  EXPECT_EQ(false, ::protos::HasExtension(mutable_recursive_child, theme));
  EXPECT_EQ(
      true,
      ::protos::SetExtension(mutable_recursive_child, theme, extension1).ok());
  ::protos::Ptr<const TestModel> recursive_child = model.recursive_child();
  EXPECT_EQ("Hello World",
            ::protos::GetExtension(recursive_child, theme).value()->ext_name());
}

TEST(CppGeneratedCode, SerializeUsingArena) {
  TestModel model;
  model.set_str1("Hello World");
  ::upb::Arena arena;
  absl::StatusOr<absl::string_view> bytes = ::protos::Serialize(model, arena);
  EXPECT_EQ(true, bytes.ok());
  TestModel parsed_model = ::protos::Parse<TestModel>(bytes.value()).value();
  EXPECT_EQ("Hello World", parsed_model.str1());
}

TEST(CppGeneratedCode, SerializeNestedMessageUsingArena) {
  TestModel model;
  model.mutable_recursive_child()->set_str1("Hello World");
  ::upb::Arena arena;
  ::protos::Ptr<const TestModel> child = model.recursive_child();
  absl::StatusOr<absl::string_view> bytes = ::protos::Serialize(child, arena);
  EXPECT_EQ(true, bytes.ok());
  TestModel parsed_model = ::protos::Parse<TestModel>(bytes.value()).value();
  EXPECT_EQ("Hello World", parsed_model.str1());
}

TEST(CppGeneratedCode, Parse) {
  TestModel model;
  model.set_str1("Test123");
  ThemeExtension extension1;
  extension1.set_ext_name("Hello World");
  EXPECT_EQ(true, ::protos::SetExtension(model, theme, extension1).ok());
  ::upb::Arena arena;
  auto bytes = ::protos::Serialize(model, arena);
  EXPECT_EQ(true, bytes.ok());
  TestModel parsed_model = ::protos::Parse<TestModel>(bytes.value()).value();
  EXPECT_EQ("Test123", parsed_model.str1());
  // Should not return an extension since we did not pass ExtensionRegistry.
  EXPECT_EQ(false, ::protos::GetExtension(parsed_model, theme).ok());
}

TEST(CppGeneratedCode, ParseWithExtensionRegistry) {
  TestModel model;
  model.set_str1("Test123");
  ThemeExtension extension1;
  extension1.set_ext_name("Hello World");
  EXPECT_EQ(true, ::protos::SetExtension(model, theme, extension1).ok());
  ::upb::Arena arena;
  auto bytes = ::protos::Serialize(model, arena);
  EXPECT_EQ(true, bytes.ok());
  ::protos::ExtensionRegistry extensions({&theme, &other_ext}, arena);
  TestModel parsed_model =
      ::protos::Parse<TestModel>(bytes.value(), extensions).value();
  EXPECT_EQ("Test123", parsed_model.str1());
  EXPECT_EQ(true, ::protos::GetExtension(parsed_model, theme).ok());
}

TEST(CppGeneratedCode, NameCollisions) {
  TestModel model;
  model.set_template_("test");
  EXPECT_EQ("test", model.template_());
  model.set_arena__("test");
  EXPECT_EQ("test", model.arena__());
}
