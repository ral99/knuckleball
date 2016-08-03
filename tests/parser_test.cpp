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

#include "parser.h"

/////////////////////////////////////////////////// Valid statements ///////////////////////////////////////////////////

TEST(Parser, valid_statement_1) {
    Parser parser("Boolean create: b withValue: true;");
    EXPECT_EQ(parser.object(), "Boolean");
    EXPECT_EQ(parser.message_name(), "create:withValue:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"b", "true"}));
}

TEST(Parser, valid_statement_2) {
    Parser parser("b get;");
    EXPECT_EQ(parser.object(), "b");
    EXPECT_EQ(parser.message_name(), "get");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({}));
}

TEST(Parser, valid_statement_3) {
    Parser parser("b isTrue?;");
    EXPECT_EQ(parser.object(), "b");
    EXPECT_EQ(parser.message_name(), "isTrue?");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({}));
}

TEST(Parser, valid_statement_4) {
    Parser parser("b set: false;");
    EXPECT_EQ(parser.object(), "b");
    EXPECT_EQ(parser.message_name(), "set:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"false"}));
}

TEST(Parser, valid_statement_5) {
    Parser parser("Character create: numeric withValue: '1';");
    EXPECT_EQ(parser.object(), "Character");
    EXPECT_EQ(parser.message_name(), "create:withValue:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"numeric", "'1'"}));
}

TEST(Parser, valid_statement_6) {
    Parser parser("i isEqualTo? -42;");
    EXPECT_EQ(parser.object(), "i");
    EXPECT_EQ(parser.message_name(), "isEqualTo?");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"-42"}));
}

TEST(Parser, valid_statement_7) {
    Parser parser("Float createIfNotExists: f withValue: -42.0;");
    EXPECT_EQ(parser.object(), "Float");
    EXPECT_EQ(parser.message_name(), "createIfNotExists:withValue:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"f", "-42.0"}));
}

TEST(Parser, valid_statement_8) {
    Parser parser("String create: str withValue: \"knuckleball\";");
    EXPECT_EQ(parser.object(), "String");
    EXPECT_EQ(parser.message_name(), "create:withValue:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"str", "\"knuckleball\""}));
}

TEST(Parser, valid_statement_9) {
    Parser parser("str replaceFirst: \"knuckle\" fromIndex: 0 with: \"ball\";");
    EXPECT_EQ(parser.object(), "str");
    EXPECT_EQ(parser.message_name(), "replaceFirst:fromIndex:with:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"\"knuckle\"", "0", "\"ball\""}));
}

TEST(Parser, valid_statement_10) {
    Parser parser("Vector<Integer> create: prices;");
    EXPECT_EQ(parser.object(), "Vector<Integer>");
    EXPECT_EQ(parser.message_name(), "create:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"prices"}));
}

TEST(Parser, valid_statement_11) {
    Parser parser("Set<String> create: ids;");
    EXPECT_EQ(parser.object(), "Set<String>");
    EXPECT_EQ(parser.message_name(), "create:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"ids"}));
}

TEST(Parser, valid_statement_12) {
    Parser parser("Dictionary<String, Integer> create: ages;");
    EXPECT_EQ(parser.object(), "Dictionary<String, Integer>");
    EXPECT_EQ(parser.message_name(), "create:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"ages"}));
}

TEST(Parser, valid_statement_13) {
    Parser parser("Context listVariablesOfNamespace: std;");
    EXPECT_EQ(parser.object(), "Context");
    EXPECT_EQ(parser.message_name(), "listVariablesOfNamespace:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"std"}));
}

///////////////////////////////////////////////////// Corner cases /////////////////////////////////////////////////////

TEST(Parser, corner_case_1) {
    Parser parser("  prices  get  ;  ");
    EXPECT_EQ(parser.object(), "prices");
    EXPECT_EQ(parser.message_name(), "get");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({}));
}

TEST(Parser, corner_case_2) {
    Parser parser("  Integer  create:  i  withValue:  +42  ;");
    EXPECT_EQ(parser.object(), "Integer");
    EXPECT_EQ(parser.message_name(), "create:withValue:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"i", "+42"}));
}

TEST(Parser, corner_case_3) {
    Parser parser("Integer create:i withValue:-42;");
    EXPECT_EQ(parser.object(), "Integer");
    EXPECT_EQ(parser.message_name(), "create:withValue:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"i", "-42"}));
}

TEST(Parser, corner_case_4) {
    Parser parser("String create: str withValue: \"one two three four\";");
    EXPECT_EQ(parser.object(), "String");
    EXPECT_EQ(parser.message_name(), "create:withValue:");
    EXPECT_EQ(parser.arguments(), std::vector<std::string>({"str", "\"one two three four\""}));
}

////////////////////////////////////////////////// Invalid statements //////////////////////////////////////////////////

TEST(Parser, invalid_statements) {
    // invalid syntax:
    EXPECT_ANY_THROW(Parser("42;"));
    EXPECT_ANY_THROW(Parser("true;"));
    
    // invalid object:
    EXPECT_ANY_THROW(Parser("42 add: 1;"));
    EXPECT_ANY_THROW(Parser("true get;"));

    // missing message:
    EXPECT_ANY_THROW(Parser("Integer;"));
    EXPECT_ANY_THROW(Parser("Context;"));
    EXPECT_ANY_THROW(Parser("std::prices;"));

    // missing object:
    EXPECT_ANY_THROW(Parser("create: i withValue: 42;"));

    // missing semicolon:
    EXPECT_ANY_THROW(Parser("Integer create: i withValue: 42"));

    // missing argument:
    EXPECT_ANY_THROW(Parser("Integer create: i withValue: ;"));
    EXPECT_ANY_THROW(Parser("Integer create: withValue: 42;"));
}
