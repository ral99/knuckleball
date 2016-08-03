/*
Copyright (c) 2016, Rodrigo Alves Lima
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the
       following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
       following disclaimer in the documentation and/or other materials provided with the distribution.

    3. Neither the name of Knuckleball nor the names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "gtest/gtest.h"

#include "context.h"
#include "object.h"

class ObjectTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        Context::get_instance("test.log", 3, 1e-9);
    }
};

/////////////////////////////////////////////////////// Boolean ////////////////////////////////////////////////////////

TEST_F(ObjectTest, Boolean_construction_with_representation_1) {
    Boolean t("true");
    EXPECT_EQ(t.type(), "Boolean");
    EXPECT_EQ(t.name(), "");
    EXPECT_EQ(t.value(), true);
    EXPECT_EQ(t.representation(), "true");
}

TEST_F(ObjectTest, Boolean_construction_with_representation_2) {
    Boolean f("false");
    EXPECT_EQ(f.type(), "Boolean");
    EXPECT_EQ(f.name(), "");
    EXPECT_EQ(f.value(), false);
    EXPECT_EQ(f.representation(), "false");
}

TEST_F(ObjectTest, Boolean_construction_with_message_1) {
    Boolean t("create:withValue:", std::vector<std::string>({"b", "true"}));
    EXPECT_EQ(t.type(), "Boolean");
    EXPECT_EQ(t.name(), "b");
    EXPECT_EQ(t.value(), true);
    EXPECT_EQ(t.representation(), "true");
}

TEST_F(ObjectTest, Boolean_construction_with_message_2) {
    Boolean f("create:withValue:", std::vector<std::string>({"b", "false"}));
    EXPECT_EQ(f.type(), "Boolean");
    EXPECT_EQ(f.name(), "b");
    EXPECT_EQ(f.value(), false);
    EXPECT_EQ(f.representation(), "false");
}

TEST_F(ObjectTest, Boolean_construction_with_message_3) {
    Boolean t("createIfNotExists:withValue:", std::vector<std::string>({"b", "true"}));
    EXPECT_EQ(t.type(), "Boolean");
    EXPECT_EQ(t.name(), "b");
    EXPECT_EQ(t.value(), true);
    EXPECT_EQ(t.representation(), "true");
}

TEST_F(ObjectTest, Boolean_construction_with_message_4) {
    Boolean f("createIfNotExists:withValue:", std::vector<std::string>({"b", "false"}));
    EXPECT_EQ(f.type(), "Boolean");
    EXPECT_EQ(f.name(), "b");
    EXPECT_EQ(f.value(), false);
    EXPECT_EQ(f.representation(), "false");
}

TEST_F(ObjectTest, Boolean_invalid_constructions) {
    EXPECT_ANY_THROW(Boolean("42"));
    EXPECT_ANY_THROW(Boolean("create:withValue:", std::vector<std::string>({"t"})));
    EXPECT_ANY_THROW(Boolean("create:withValue:", std::vector<std::string>({"t", "42"})));
    EXPECT_ANY_THROW(Boolean("create:withValue:", std::vector<std::string>({"true", "t"})));
    EXPECT_ANY_THROW(Boolean("createIfNotExists:withValue:", std::vector<std::string>({"t"})));
    EXPECT_ANY_THROW(Boolean("createIfNotExists:withValue:", std::vector<std::string>({"t", "42"})));
    EXPECT_ANY_THROW(Boolean("createIfNotExists:withValue:", std::vector<std::string>({"true", "t"})));
}

////////////////////////////////////////////////////// Character ///////////////////////////////////////////////////////

TEST_F(ObjectTest, Character_construction_with_representation) {
    Character numeric("'1'");
    EXPECT_EQ(numeric.type(), "Character");
    EXPECT_EQ(numeric.name(), "");
    EXPECT_EQ(numeric.value(), '1');
    EXPECT_EQ(numeric.representation(), "'1'");
}

TEST_F(ObjectTest, Character_construction_with_message_1) {
    Character numeric("create:withValue:", std::vector<std::string>({"numeric", "'1'"}));
    EXPECT_EQ(numeric.type(), "Character");
    EXPECT_EQ(numeric.name(), "numeric");
    EXPECT_EQ(numeric.value(), '1');
    EXPECT_EQ(numeric.representation(), "'1'");
}

TEST_F(ObjectTest, Character_construction_with_message_2) {
    Character numeric("createIfNotExists:withValue:", std::vector<std::string>({"numeric", "'1'"}));
    EXPECT_EQ(numeric.type(), "Character");
    EXPECT_EQ(numeric.name(), "numeric");
    EXPECT_EQ(numeric.value(), '1');
    EXPECT_EQ(numeric.representation(), "'1'");
}

TEST_F(ObjectTest, Character_invalid_constructions) {
    EXPECT_ANY_THROW(Character("42"));
    EXPECT_ANY_THROW(Character("create:withValue:", std::vector<std::string>({"numeric"})));
    EXPECT_ANY_THROW(Character("create:withValue:", std::vector<std::string>({"numeric", "42"})));
    EXPECT_ANY_THROW(Character("create:withValue:", std::vector<std::string>({"'1'", "numeric"})));
    EXPECT_ANY_THROW(Character("createIfNotExists:withValue:", std::vector<std::string>({"numeric"})));
    EXPECT_ANY_THROW(Character("createIfNotExists:withValue:", std::vector<std::string>({"numeric", "42"})));
    EXPECT_ANY_THROW(Character("createIfNotExists:withValue:", std::vector<std::string>({"'1'", "numeric"})));
}

/////////////////////////////////////////////////////// Integer ////////////////////////////////////////////////////////

TEST_F(ObjectTest, Integer_construction_with_value) {
    Integer i(42);
    EXPECT_EQ(i.type(), "Integer");
    EXPECT_EQ(i.name(), "");
    EXPECT_EQ(i.value(), 42);
    EXPECT_EQ(i.representation(), "42");
}

TEST_F(ObjectTest, Integer_construction_with_representation_1) {
    Integer i("+42");
    EXPECT_EQ(i.type(), "Integer");
    EXPECT_EQ(i.name(), "");
    EXPECT_EQ(i.value(), 42);
    EXPECT_EQ(i.representation(), "42");
}

TEST_F(ObjectTest, Integer_construction_with_representation_2) {
    Integer i("-42");
    EXPECT_EQ(i.type(), "Integer");
    EXPECT_EQ(i.name(), "");
    EXPECT_EQ(i.value(), -42);
    EXPECT_EQ(i.representation(), "-42");
}

TEST_F(ObjectTest, Integer_construction_with_representation_3) {
    Integer i("42.9");
    EXPECT_EQ(i.type(), "Integer");
    EXPECT_EQ(i.name(), "");
    EXPECT_EQ(i.value(), 42);
    EXPECT_EQ(i.representation(), "42");
}

TEST_F(ObjectTest, Integer_construction_with_message_1) {
    Integer i("create:withValue:", std::vector<std::string>({"i", "42"}));
    EXPECT_EQ(i.type(), "Integer");
    EXPECT_EQ(i.name(), "i");
    EXPECT_EQ(i.value(), 42);
    EXPECT_EQ(i.representation(), "42");
}

TEST_F(ObjectTest, Integer_construction_with_message_2) {
    Integer i("create:withValue:", std::vector<std::string>({"i", "-42"}));
    EXPECT_EQ(i.type(), "Integer");
    EXPECT_EQ(i.name(), "i");
    EXPECT_EQ(i.value(), -42);
    EXPECT_EQ(i.representation(), "-42");
}

TEST_F(ObjectTest, Integer_construction_with_message_3) {
    Integer i("create:withValue:", std::vector<std::string>({"i", "42.9"}));
    EXPECT_EQ(i.type(), "Integer");
    EXPECT_EQ(i.name(), "i");
    EXPECT_EQ(i.value(), 42);
    EXPECT_EQ(i.representation(), "42");
}

TEST_F(ObjectTest, Integer_construction_with_message_4) {
    Integer i("createIfNotExists:withValue:", std::vector<std::string>({"i", "42"}));
    EXPECT_EQ(i.type(), "Integer");
    EXPECT_EQ(i.name(), "i");
    EXPECT_EQ(i.value(), 42);
    EXPECT_EQ(i.representation(), "42");
}

TEST_F(ObjectTest, Integer_construction_with_message_5) {
    Integer i("createIfNotExists:withValue:", std::vector<std::string>({"i", "-42"}));
    EXPECT_EQ(i.type(), "Integer");
    EXPECT_EQ(i.name(), "i");
    EXPECT_EQ(i.value(), -42);
    EXPECT_EQ(i.representation(), "-42");
}

TEST_F(ObjectTest, Integer_construction_with_message_6) {
    Integer i("createIfNotExists:withValue:", std::vector<std::string>({"i", "42.9"}));
    EXPECT_EQ(i.type(), "Integer");
    EXPECT_EQ(i.name(), "i");
    EXPECT_EQ(i.value(), 42);
    EXPECT_EQ(i.representation(), "42");
}

TEST_F(ObjectTest, Integer_invalid_constructions) {
    EXPECT_ANY_THROW(Integer("true"));
    EXPECT_ANY_THROW(Integer("create:withValue:", std::vector<std::string>({"i"})));
    EXPECT_ANY_THROW(Integer("create:withValue:", std::vector<std::string>({"i", "true"})));
    EXPECT_ANY_THROW(Integer("create:withValue:", std::vector<std::string>({"42", "i"})));
    EXPECT_ANY_THROW(Integer("createIfNotExists:withValue:", std::vector<std::string>({"i"})));
    EXPECT_ANY_THROW(Integer("createIfNotExists:withValue:", std::vector<std::string>({"i", "true"})));
    EXPECT_ANY_THROW(Integer("createIfNotExists:withValue:", std::vector<std::string>({"42", "i"})));
}

//////////////////////////////////////////////////////// Float /////////////////////////////////////////////////////////

TEST_F(ObjectTest, Float_construction_with_value) {
    Float f(42.0);
    EXPECT_EQ(f.type(), "Float");
    EXPECT_EQ(f.name(), "");
    EXPECT_EQ(f.representation(), "42.000");
}

TEST_F(ObjectTest, Float_construction_with_representation_1) {
    Float f("42");
    EXPECT_EQ(f.type(), "Float");
    EXPECT_EQ(f.name(), "");
    EXPECT_EQ(f.representation(), "42.000");
}

TEST_F(ObjectTest, Float_construction_with_representation_2) {
    Float f("-42.0");
    EXPECT_EQ(f.type(), "Float");
    EXPECT_EQ(f.name(), "");
    EXPECT_EQ(f.representation(), "-42.000");
}

TEST_F(ObjectTest, Float_construction_with_representation_3) {
    Float f("1e-3");
    EXPECT_EQ(f.type(), "Float");
    EXPECT_EQ(f.name(), "");
    EXPECT_EQ(f.representation(), "0.001");
}

TEST_F(ObjectTest, Float_construction_with_message_1) {
    Float f("create:withValue:", std::vector<std::string>({"f", "42"}));
    EXPECT_EQ(f.type(), "Float");
    EXPECT_EQ(f.name(), "f");
    EXPECT_EQ(f.representation(), "42.000");
}

TEST_F(ObjectTest, Float_construction_with_message_2) {
    Float f("create:withValue:", std::vector<std::string>({"f", "-42.0"}));
    EXPECT_EQ(f.type(), "Float");
    EXPECT_EQ(f.name(), "f");
    EXPECT_EQ(f.representation(), "-42.000");
}

TEST_F(ObjectTest, Float_construction_with_message_3) {
    Float f("create:withValue:", std::vector<std::string>({"f", "1e-3"}));
    EXPECT_EQ(f.type(), "Float");
    EXPECT_EQ(f.name(), "f");
    EXPECT_EQ(f.representation(), "0.001");
}

TEST_F(ObjectTest, Float_construction_with_message_4) {
    Float f("createIfNotExists:withValue:", std::vector<std::string>({"f", "42"}));
    EXPECT_EQ(f.type(), "Float");
    EXPECT_EQ(f.name(), "f");
    EXPECT_EQ(f.representation(), "42.000");
}

TEST_F(ObjectTest, Float_construction_with_message_5) {
    Float f("createIfNotExists:withValue:", std::vector<std::string>({"f", "-42.0"}));
    EXPECT_EQ(f.type(), "Float");
    EXPECT_EQ(f.name(), "f");
    EXPECT_EQ(f.representation(), "-42.000");
}

TEST_F(ObjectTest, Float_construction_with_message_6) {
    Float f("createIfNotExists:withValue:", std::vector<std::string>({"f", "1e-3"}));
    EXPECT_EQ(f.type(), "Float");
    EXPECT_EQ(f.name(), "f");
    EXPECT_EQ(f.representation(), "0.001");
}

TEST_F(ObjectTest, Float_invalid_constructions) {
    EXPECT_ANY_THROW(Float("true"));
    EXPECT_ANY_THROW(Float("create:withValue:", std::vector<std::string>({"f"})));
    EXPECT_ANY_THROW(Float("create:withValue:", std::vector<std::string>({"f", "true"})));
    EXPECT_ANY_THROW(Float("create:withValue:", std::vector<std::string>({"42.0", "f"})));
    EXPECT_ANY_THROW(Float("createIfNotExists:withValue:", std::vector<std::string>({"f"})));
    EXPECT_ANY_THROW(Float("createIfNotExists:withValue:", std::vector<std::string>({"f", "true"})));
    EXPECT_ANY_THROW(Float("createIfNotExists:withValue:", std::vector<std::string>({"42.0", "f"})));
}

//////////////////////////////////////////////////////// String ////////////////////////////////////////////////////////

TEST_F(ObjectTest, String_construction_with_representation_1) {
    String str("\"\"");
    EXPECT_EQ(str.type(), "String");
    EXPECT_EQ(str.name(), "");
    EXPECT_EQ(str.value(), "");
    EXPECT_EQ(str.representation(), "\"\"");
}

TEST_F(ObjectTest, String_construction_with_representation_2) {
    String str("\"knuckleball\"");
    EXPECT_EQ(str.type(), "String");
    EXPECT_EQ(str.name(), "");
    EXPECT_EQ(str.value(), "knuckleball");
    EXPECT_EQ(str.representation(), "\"knuckleball\"");
}

TEST_F(ObjectTest, String_construction_with_message_1) {
    String str("create:", std::vector<std::string>({"str"}));
    EXPECT_EQ(str.type(), "String");
    EXPECT_EQ(str.name(), "str");
    EXPECT_EQ(str.value(), "");
    EXPECT_EQ(str.representation(), "\"\"");
}

TEST_F(ObjectTest, String_construction_with_message_2) {
    String str("createIfNotExists:", std::vector<std::string>({"str"}));
    EXPECT_EQ(str.type(), "String");
    EXPECT_EQ(str.name(), "str");
    EXPECT_EQ(str.value(), "");
    EXPECT_EQ(str.representation(), "\"\"");
}

TEST_F(ObjectTest, String_construction_with_message_3) {
    String str("create:withValue:", std::vector<std::string>({"str", "\"knuckleball\""}));
    EXPECT_EQ(str.type(), "String");
    EXPECT_EQ(str.name(), "str");
    EXPECT_EQ(str.value(), "knuckleball");
    EXPECT_EQ(str.representation(), "\"knuckleball\"");
}

TEST_F(ObjectTest, String_construction_with_message_4) {
    String str("create:withValue:", std::vector<std::string>({"str", "\"knuckle\\\"ball\""}));
    EXPECT_EQ(str.type(), "String");
    EXPECT_EQ(str.name(), "str");
    EXPECT_EQ(str.value(), "knuckle\"ball");
    EXPECT_EQ(str.representation(), "\"knuckle\\\"ball\"");
}

TEST_F(ObjectTest, String_construction_with_message_5) {
    String str("createIfNotExists:withValue:", std::vector<std::string>({"str", "\"knuckleball\""}));
    EXPECT_EQ(str.type(), "String");
    EXPECT_EQ(str.name(), "str");
    EXPECT_EQ(str.value(), "knuckleball");
    EXPECT_EQ(str.representation(), "\"knuckleball\"");
}

TEST_F(ObjectTest, String_construction_with_message_6) {
    String str("createIfNotExists:withValue:", std::vector<std::string>({"str", "\"knuckle\\\"ball\""}));
    EXPECT_EQ(str.type(), "String");
    EXPECT_EQ(str.name(), "str");
    EXPECT_EQ(str.value(), "knuckle\"ball");
    EXPECT_EQ(str.representation(), "\"knuckle\\\"ball\"");
}

TEST_F(ObjectTest, String_invalid_constructions) {
    EXPECT_ANY_THROW(String("42"));
    EXPECT_ANY_THROW(String("create:", std::vector<std::string>()));
    EXPECT_ANY_THROW(String("createIfNotExists:", std::vector<std::string>()));
    EXPECT_ANY_THROW(String("create:withValue:", std::vector<std::string>({"str"})));
    EXPECT_ANY_THROW(String("create:withValue:", std::vector<std::string>({"str", "42"})));
    EXPECT_ANY_THROW(String("create:withValue:", std::vector<std::string>({"\"knuckleball\"", "str"})));
    EXPECT_ANY_THROW(String("createIfNotExists:withValue:", std::vector<std::string>({"str"})));
    EXPECT_ANY_THROW(String("createIfNotExists:withValue:", std::vector<std::string>({"str", "42"})));
    EXPECT_ANY_THROW(String("createIfNotExists:withValue:", std::vector<std::string>({"\"knuckleball\"", "str"})));
}

//////////////////////////////////////////////////////// Vector ////////////////////////////////////////////////////////

TEST_F(ObjectTest, Vector_construction) {
    Vector prices("Integer", "create:", std::vector<std::string>({"prices"}));
    EXPECT_EQ(prices.type(), "Vector");
    EXPECT_EQ(prices.name(), "prices");
    EXPECT_EQ(prices.representation(), "[]");
}

TEST_F(ObjectTest, Vector_invalid_constructions) {
    EXPECT_ANY_THROW(Vector("Integer", "create:", std::vector<std::string>()));
    EXPECT_ANY_THROW(Vector("Integer", "create:", std::vector<std::string>({"42"})));
    EXPECT_ANY_THROW(Vector("Integer", "create:", std::vector<std::string>({"prices", "42"})));
}

///////////////////////////////////////////////////////// Set //////////////////////////////////////////////////////////

TEST_F(ObjectTest, Set_construction) {
    Set ids("String", "create:", std::vector<std::string>({"ids"}));
    EXPECT_EQ(ids.type(), "Set");
    EXPECT_EQ(ids.name(), "ids");
    EXPECT_EQ(ids.representation(), "{}");
}

TEST_F(ObjectTest, Set_invalid_constructions) {
    EXPECT_ANY_THROW(Set("String", "create:", std::vector<std::string>()));
    EXPECT_ANY_THROW(Set("String", "create:", std::vector<std::string>({"42"})));
    EXPECT_ANY_THROW(Set("String", "create:", std::vector<std::string>({"ids", "42"})));
}

////////////////////////////////////////////////////// Dictionary //////////////////////////////////////////////////////

TEST_F(ObjectTest, Dictionary_construction) {
    Dictionary ages("String", "Integer", "create:", std::vector<std::string>({"ages"}));
    EXPECT_EQ(ages.type(), "Dictionary");
    EXPECT_EQ(ages.name(), "ages");
    EXPECT_EQ(ages.representation(), "()");
}

TEST_F(ObjectTest, Dictionary_invalid_constructions) {
    EXPECT_ANY_THROW(Dictionary("String", "Integer", "create:", std::vector<std::string>()));
    EXPECT_ANY_THROW(Dictionary("String", "Integer", "create:", std::vector<std::string>({"42"})));
    EXPECT_ANY_THROW(Dictionary("String", "Integer", "create:", std::vector<std::string>({"ages", "42"})));
}
