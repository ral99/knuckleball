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

#include "grammar.h"

TEST(Grammar, is_spaces) {
    EXPECT_TRUE(Grammar::is_spaces(""));
    EXPECT_TRUE(Grammar::is_spaces(" "));
    EXPECT_TRUE(Grammar::is_spaces("\n"));
    EXPECT_TRUE(Grammar::is_spaces("\t"));
}

TEST(Grammar, is_not_spaces) {
    EXPECT_FALSE(Grammar::is_spaces("knuckleball"));
    EXPECT_FALSE(Grammar::is_spaces(" knuckleball"));
    EXPECT_FALSE(Grammar::is_spaces("knuckleball "));
}

TEST(Grammar, is_boolean_type) {
    EXPECT_TRUE(Grammar::is_boolean_type("Boolean"));
}

TEST(Grammar, is_not_boolean_type) {
    EXPECT_FALSE(Grammar::is_boolean_type(""));
    EXPECT_FALSE(Grammar::is_boolean_type("boolean"));
    EXPECT_FALSE(Grammar::is_boolean_type("BOOLEAN"));
    EXPECT_FALSE(Grammar::is_boolean_type(" Boolean"));
    EXPECT_FALSE(Grammar::is_boolean_type("Boolean "));
}

TEST(Grammar, is_boolean_value) {
    EXPECT_TRUE(Grammar::is_boolean_value("true"));
    EXPECT_TRUE(Grammar::is_boolean_value("false"));
}

TEST(Grammar, is_not_boolean_value) {
    EXPECT_FALSE(Grammar::is_boolean_value(""));
    EXPECT_FALSE(Grammar::is_boolean_value("1"));
    EXPECT_FALSE(Grammar::is_boolean_value("0"));
    EXPECT_FALSE(Grammar::is_boolean_value("TRUE"));
    EXPECT_FALSE(Grammar::is_boolean_value("FALSE"));
    EXPECT_FALSE(Grammar::is_boolean_value("True"));
    EXPECT_FALSE(Grammar::is_boolean_value("False"));
    EXPECT_FALSE(Grammar::is_boolean_value(" true"));
    EXPECT_FALSE(Grammar::is_boolean_value("false "));
}

TEST(Grammar, is_character_type) {
    EXPECT_TRUE(Grammar::is_character_type("Character"));
}

TEST(Grammar, is_not_character_type) {
    EXPECT_FALSE(Grammar::is_character_type(""));
    EXPECT_FALSE(Grammar::is_character_type("character"));
    EXPECT_FALSE(Grammar::is_character_type("CHARACTER"));
    EXPECT_FALSE(Grammar::is_character_type(" Character"));
    EXPECT_FALSE(Grammar::is_character_type("Character "));
}

TEST(Grammar, is_character_value) {
    EXPECT_TRUE(Grammar::is_character_value("'a'"));
    EXPECT_TRUE(Grammar::is_character_value("'A'"));
    EXPECT_TRUE(Grammar::is_character_value("'0'"));
    EXPECT_TRUE(Grammar::is_character_value("'#'"));
    EXPECT_TRUE(Grammar::is_character_value("' '"));
    EXPECT_TRUE(Grammar::is_character_value("'\''"));
}

TEST(Grammar, is_not_character_value) {
    EXPECT_FALSE(Grammar::is_character_value(""));
    EXPECT_FALSE(Grammar::is_character_value("''"));
    EXPECT_FALSE(Grammar::is_character_value("'aa'"));
    EXPECT_FALSE(Grammar::is_character_value("'a' "));
    EXPECT_FALSE(Grammar::is_character_value(" 'a'"));
}

TEST(Grammar, is_integer_type) {
    EXPECT_TRUE(Grammar::is_integer_type("Integer"));
}

TEST(Grammar, is_not_integer_type) {
    EXPECT_FALSE(Grammar::is_integer_type(""));
    EXPECT_FALSE(Grammar::is_integer_type("integer"));
    EXPECT_FALSE(Grammar::is_integer_type("INTEGER"));
    EXPECT_FALSE(Grammar::is_integer_type(" Integer"));
    EXPECT_FALSE(Grammar::is_integer_type("Integer "));
}

TEST(Grammar, is_integer_value) {
    EXPECT_TRUE(Grammar::is_integer_value("0"));
    EXPECT_TRUE(Grammar::is_integer_value("+0"));
    EXPECT_TRUE(Grammar::is_integer_value("-0"));
    EXPECT_TRUE(Grammar::is_integer_value("42"));
    EXPECT_TRUE(Grammar::is_integer_value("+42"));
    EXPECT_TRUE(Grammar::is_integer_value("-42"));
}

TEST(Grammar, is_not_integer_value) {
    EXPECT_FALSE(Grammar::is_integer_value(""));
    EXPECT_FALSE(Grammar::is_integer_value("+"));
    EXPECT_FALSE(Grammar::is_integer_value("-"));
    EXPECT_FALSE(Grammar::is_integer_value("42.0"));
    EXPECT_FALSE(Grammar::is_integer_value("42e5"));
    EXPECT_FALSE(Grammar::is_integer_value(" 42"));
    EXPECT_FALSE(Grammar::is_integer_value("42 "));
}

TEST(Grammar, is_float_type) {
    EXPECT_TRUE(Grammar::is_float_type("Float"));
}

TEST(Grammar, is_not_float_type) {
    EXPECT_FALSE(Grammar::is_float_type(""));
    EXPECT_FALSE(Grammar::is_float_type("float"));
    EXPECT_FALSE(Grammar::is_float_type("FLOAT"));
    EXPECT_FALSE(Grammar::is_float_type(" Float"));
    EXPECT_FALSE(Grammar::is_float_type("Float "));
}

TEST(Grammar, is_float_value) {
    EXPECT_TRUE(Grammar::is_float_value("0"));
    EXPECT_TRUE(Grammar::is_float_value("+0"));
    EXPECT_TRUE(Grammar::is_float_value("-0"));
    EXPECT_TRUE(Grammar::is_float_value("42"));
    EXPECT_TRUE(Grammar::is_float_value("+42"));
    EXPECT_TRUE(Grammar::is_float_value("-42"));
    EXPECT_TRUE(Grammar::is_float_value("42."));
    EXPECT_TRUE(Grammar::is_float_value("+42."));
    EXPECT_TRUE(Grammar::is_float_value("-42."));
    EXPECT_TRUE(Grammar::is_float_value(".42"));
    EXPECT_TRUE(Grammar::is_float_value("+.42"));
    EXPECT_TRUE(Grammar::is_float_value("-.42"));
    EXPECT_TRUE(Grammar::is_float_value("42.0"));
    EXPECT_TRUE(Grammar::is_float_value("+42.0"));
    EXPECT_TRUE(Grammar::is_float_value("-42.0"));
    EXPECT_TRUE(Grammar::is_float_value("42e1"));
    EXPECT_TRUE(Grammar::is_float_value("42e+1"));
    EXPECT_TRUE(Grammar::is_float_value("42e-1"));
    EXPECT_TRUE(Grammar::is_float_value("42.0e1"));
    EXPECT_TRUE(Grammar::is_float_value("42.0e+1"));
    EXPECT_TRUE(Grammar::is_float_value("42.0e-1"));
    EXPECT_TRUE(Grammar::is_float_value("42.e1"));
    EXPECT_TRUE(Grammar::is_float_value("42.e+1"));
    EXPECT_TRUE(Grammar::is_float_value("42.e-1"));
    EXPECT_TRUE(Grammar::is_float_value(".42e1"));
    EXPECT_TRUE(Grammar::is_float_value(".42e+1"));
    EXPECT_TRUE(Grammar::is_float_value(".42e-1"));
}

TEST(Grammar, is_not_float_value) {
    EXPECT_FALSE(Grammar::is_float_value(""));
    EXPECT_FALSE(Grammar::is_float_value("+"));
    EXPECT_FALSE(Grammar::is_float_value("-"));
    EXPECT_FALSE(Grammar::is_float_value("."));
    EXPECT_FALSE(Grammar::is_float_value("+."));
    EXPECT_FALSE(Grammar::is_float_value("-."));
    EXPECT_FALSE(Grammar::is_float_value("42..0"));
    EXPECT_FALSE(Grammar::is_float_value("e1"));
    EXPECT_FALSE(Grammar::is_float_value("+e1"));
    EXPECT_FALSE(Grammar::is_float_value("-e1"));
    EXPECT_FALSE(Grammar::is_float_value(".e1"));
    EXPECT_FALSE(Grammar::is_float_value("+.e1"));
    EXPECT_FALSE(Grammar::is_float_value("-.e1"));
    EXPECT_FALSE(Grammar::is_float_value("42e1.0"));
    EXPECT_FALSE(Grammar::is_float_value(" 42.0"));
    EXPECT_FALSE(Grammar::is_float_value("42.0 "));
}

TEST(Grammar, is_string_type) {
    EXPECT_TRUE(Grammar::is_string_type("String"));
}

TEST(Grammar, is_not_string_type) {
    EXPECT_FALSE(Grammar::is_string_type(""));
    EXPECT_FALSE(Grammar::is_string_type("string"));
    EXPECT_FALSE(Grammar::is_string_type("STRING"));
    EXPECT_FALSE(Grammar::is_string_type(" String"));
    EXPECT_FALSE(Grammar::is_string_type("String "));
}

TEST(Grammar, is_string_value) {
    EXPECT_TRUE(Grammar::is_string_value("\"\""));
    EXPECT_TRUE(Grammar::is_string_value("\"knuckleball\""));
    EXPECT_TRUE(Grammar::is_string_value("\"knuckle\\\"ball\""));
    EXPECT_TRUE(Grammar::is_string_value("\"\\\\\""));
}

TEST(Grammar, is_not_string_value) {
    EXPECT_FALSE(Grammar::is_string_value(""));
    EXPECT_FALSE(Grammar::is_string_value("\""));
    EXPECT_FALSE(Grammar::is_string_value("\"\"\""));
    EXPECT_FALSE(Grammar::is_string_value("\"\"\"\""));
    EXPECT_FALSE(Grammar::is_string_value("\"\\\""));
    EXPECT_FALSE(Grammar::is_string_value("\"knuckle\"ball\""));
    EXPECT_FALSE(Grammar::is_string_value("\"knuckleball"));
    EXPECT_FALSE(Grammar::is_string_value("knuckleball\""));
    EXPECT_FALSE(Grammar::is_string_value(" \"\""));
    EXPECT_FALSE(Grammar::is_string_value("\"\" "));
}

TEST(Grammar, is_object_type) {
    EXPECT_TRUE(Grammar::is_object_type("Boolean"));
    EXPECT_TRUE(Grammar::is_object_type("Character"));
    EXPECT_TRUE(Grammar::is_object_type("Integer"));
    EXPECT_TRUE(Grammar::is_object_type("Float"));
    EXPECT_TRUE(Grammar::is_object_type("String"));
}

TEST(Grammar, is_not_object_type) {
    EXPECT_FALSE(Grammar::is_object_type("Vector"));
    EXPECT_FALSE(Grammar::is_object_type("Vector<Integer>"));
    EXPECT_FALSE(Grammar::is_object_type("Set"));
    EXPECT_FALSE(Grammar::is_object_type("Set<Integer>"));
    EXPECT_FALSE(Grammar::is_object_type("Dictionary"));
    EXPECT_FALSE(Grammar::is_object_type("Dictionary<String, Integer>"));
}

TEST(Grammar, is_object_value) {
    EXPECT_TRUE(Grammar::is_object_value("true"));
    EXPECT_TRUE(Grammar::is_object_value("false"));
    EXPECT_TRUE(Grammar::is_object_value("'a'"));
    EXPECT_TRUE(Grammar::is_object_value("42"));
    EXPECT_TRUE(Grammar::is_object_value("42e5"));
    EXPECT_TRUE(Grammar::is_object_value("\"knuckleball\""));
}

TEST(Grammar, is_not_object_value) {
    EXPECT_FALSE(Grammar::is_object_value(""));
    EXPECT_FALSE(Grammar::is_object_value("prices"));
    EXPECT_FALSE(Grammar::is_object_value("isEmpty?"));
    EXPECT_FALSE(Grammar::is_object_value(" 42"));
    EXPECT_FALSE(Grammar::is_object_value("42 "));
}

TEST(Grammar, is_vector_type) {
    EXPECT_TRUE(Grammar::is_vector_type("Vector<Boolean>"));
    EXPECT_TRUE(Grammar::is_vector_type("Vector< Character>"));
    EXPECT_TRUE(Grammar::is_vector_type("Vector<Integer >"));
    EXPECT_TRUE(Grammar::is_vector_type("Vector< String >"));
}

TEST(Grammar, is_not_vector_type) {
    EXPECT_FALSE(Grammar::is_vector_type(""));
    EXPECT_FALSE(Grammar::is_vector_type("Vector"));
    EXPECT_FALSE(Grammar::is_vector_type("Vector <Integer>"));
    EXPECT_FALSE(Grammar::is_vector_type("Vector<Vector<Integer>>"));
    EXPECT_FALSE(Grammar::is_vector_type("Vector<Integer, Integer>"));
    EXPECT_FALSE(Grammar::is_vector_type("vector<Integer>"));
    EXPECT_FALSE(Grammar::is_vector_type("VECTOR<Integer>"));
    EXPECT_FALSE(Grammar::is_vector_type(" Vector<Integer>"));
    EXPECT_FALSE(Grammar::is_vector_type("Vector<Integer> "));
}

TEST(Grammar, is_set_type) {
    EXPECT_TRUE(Grammar::is_set_type("Set<Boolean>"));
    EXPECT_TRUE(Grammar::is_set_type("Set< Character>"));
    EXPECT_TRUE(Grammar::is_set_type("Set<Integer >"));
    EXPECT_TRUE(Grammar::is_set_type("Set< String >"));
}

TEST(Grammar, is_not_set_type) {
    EXPECT_FALSE(Grammar::is_set_type(""));
    EXPECT_FALSE(Grammar::is_set_type("Set"));
    EXPECT_FALSE(Grammar::is_set_type("Set <Integer>"));
    EXPECT_FALSE(Grammar::is_set_type("Set<Set<Integer>>"));
    EXPECT_FALSE(Grammar::is_set_type("Set<Integer, Integer>"));
    EXPECT_FALSE(Grammar::is_set_type("set<Integer>"));
    EXPECT_FALSE(Grammar::is_set_type("SET<Integer>"));
    EXPECT_FALSE(Grammar::is_set_type(" Set<Integer>"));
    EXPECT_FALSE(Grammar::is_set_type("Set<Integer> "));
}

TEST(Grammar, is_dictionary_type) {
    EXPECT_TRUE(Grammar::is_dictionary_type("Dictionary<String, Boolean>"));
    EXPECT_TRUE(Grammar::is_dictionary_type("Dictionary< String, Integer>"));
    EXPECT_TRUE(Grammar::is_dictionary_type("Dictionary<String, Float >"));
    EXPECT_TRUE(Grammar::is_dictionary_type("Dictionary< String , String >"));
}

TEST(Grammar, is_not_dictionary_type) {
    EXPECT_FALSE(Grammar::is_dictionary_type(""));
    EXPECT_FALSE(Grammar::is_dictionary_type("Dictionary"));
    EXPECT_FALSE(Grammar::is_dictionary_type("Dictionary <String, Integer>"));
    EXPECT_FALSE(Grammar::is_dictionary_type("Dictionary<String, Set<Integer>>"));
    EXPECT_FALSE(Grammar::is_dictionary_type("Dictionary<Set<Integer>, String>"));
    EXPECT_FALSE(Grammar::is_dictionary_type("Dictionary<Integer>"));
    EXPECT_FALSE(Grammar::is_dictionary_type("dictionary<String, Integer>"));
    EXPECT_FALSE(Grammar::is_dictionary_type("DICTIONARY<String, Integer>"));
    EXPECT_FALSE(Grammar::is_dictionary_type(" Dictionary<String, Integer>"));
    EXPECT_FALSE(Grammar::is_dictionary_type("Dictionary<String, Integer> "));
}

TEST(Grammar, is_container_type) {
    EXPECT_TRUE(Grammar::is_container_type("Vector<Boolean>"));
    EXPECT_TRUE(Grammar::is_container_type("Set<Character>"));
    EXPECT_TRUE(Grammar::is_container_type("Dictionary<String, Float>"));
}

TEST(Grammar, is_not_container_type) {
    EXPECT_FALSE(Grammar::is_container_type("Boolean"));
    EXPECT_FALSE(Grammar::is_container_type("Character"));
    EXPECT_FALSE(Grammar::is_container_type("Integer"));
    EXPECT_FALSE(Grammar::is_container_type("Float"));
    EXPECT_FALSE(Grammar::is_container_type("String"));
}

TEST(Grammar, is_type) {
    EXPECT_TRUE(Grammar::is_type("Boolean"));
    EXPECT_TRUE(Grammar::is_type("Character"));
    EXPECT_TRUE(Grammar::is_type("Integer"));
    EXPECT_TRUE(Grammar::is_type("Float"));
    EXPECT_TRUE(Grammar::is_type("String"));
    EXPECT_TRUE(Grammar::is_type("Vector<Boolean>"));
    EXPECT_TRUE(Grammar::is_type("Set<Character>"));
    EXPECT_TRUE(Grammar::is_type("Dictionary<String, Integer>"));
}

TEST(Grammar, is_not_type) {
    EXPECT_FALSE(Grammar::is_type(""));
    EXPECT_FALSE(Grammar::is_type("42"));
    EXPECT_FALSE(Grammar::is_type("prices"));
    EXPECT_FALSE(Grammar::is_type("isEmpty?"));
    EXPECT_FALSE(Grammar::is_type(" Integer"));
    EXPECT_FALSE(Grammar::is_type("Vector<Integer> "));
}

TEST(Grammar, is_connection) {
    EXPECT_TRUE(Grammar::is_connection("Connection"));
}

TEST(Grammar, is_not_connection) {
    EXPECT_FALSE(Grammar::is_connection(""));
    EXPECT_FALSE(Grammar::is_connection("connection"));
    EXPECT_FALSE(Grammar::is_connection("CONNECTION"));
    EXPECT_FALSE(Grammar::is_connection(" Connection"));
    EXPECT_FALSE(Grammar::is_connection("Connection "));
}

TEST(Grammar, is_context) {
    EXPECT_TRUE(Grammar::is_context("Context"));
}

TEST(Grammar, is_not_context) {
    EXPECT_FALSE(Grammar::is_context(""));
    EXPECT_FALSE(Grammar::is_context("context"));
    EXPECT_FALSE(Grammar::is_context("CONTEXT"));
    EXPECT_FALSE(Grammar::is_context(" Context"));
    EXPECT_FALSE(Grammar::is_context("Context "));
}

TEST(Grammar, is_reserverd_word) {
    EXPECT_TRUE(Grammar::is_reserved_word("null"));
    EXPECT_TRUE(Grammar::is_reserved_word("true"));
    EXPECT_TRUE(Grammar::is_reserved_word("false"));
    EXPECT_TRUE(Grammar::is_reserved_word("Connection"));
    EXPECT_TRUE(Grammar::is_reserved_word("Context"));
    EXPECT_TRUE(Grammar::is_reserved_word("Boolean"));
    EXPECT_TRUE(Grammar::is_reserved_word("Character"));
    EXPECT_TRUE(Grammar::is_reserved_word("Integer"));
    EXPECT_TRUE(Grammar::is_reserved_word("Float"));
    EXPECT_TRUE(Grammar::is_reserved_word("String"));
    EXPECT_TRUE(Grammar::is_reserved_word("Vector"));
    EXPECT_TRUE(Grammar::is_reserved_word("Set"));
    EXPECT_TRUE(Grammar::is_reserved_word("Dictionary"));
}

TEST(Grammar, is_not_reserverd_word) {
    EXPECT_FALSE(Grammar::is_reserved_word(""));
    EXPECT_FALSE(Grammar::is_reserved_word("42"));
    EXPECT_FALSE(Grammar::is_reserved_word("prices"));
    EXPECT_FALSE(Grammar::is_reserved_word("isEmpty?"));
    EXPECT_FALSE(Grammar::is_reserved_word(" Integer"));
    EXPECT_FALSE(Grammar::is_reserved_word("Vector "));
}

TEST(Grammar, is_identifier) {
    EXPECT_TRUE(Grammar::is_identifier("i"));
    EXPECT_TRUE(Grammar::is_identifier("id"));
    EXPECT_TRUE(Grammar::is_identifier("id2016"));
    EXPECT_TRUE(Grammar::is_identifier("id_2016"));
}

TEST(Grammar, is_not_identifier) {
    EXPECT_FALSE(Grammar::is_identifier(""));
    EXPECT_FALSE(Grammar::is_identifier("null"));
    EXPECT_FALSE(Grammar::is_identifier("true"));
    EXPECT_FALSE(Grammar::is_identifier("false"));
    EXPECT_FALSE(Grammar::is_identifier("Connection"));
    EXPECT_FALSE(Grammar::is_identifier("Context"));
    EXPECT_FALSE(Grammar::is_identifier("Boolean"));
    EXPECT_FALSE(Grammar::is_identifier("Character"));
    EXPECT_FALSE(Grammar::is_identifier("Integer"));
    EXPECT_FALSE(Grammar::is_identifier("Float"));
    EXPECT_FALSE(Grammar::is_identifier("String"));
    EXPECT_FALSE(Grammar::is_identifier("Vector"));
    EXPECT_FALSE(Grammar::is_identifier("Set"));
    EXPECT_FALSE(Grammar::is_identifier("Dictionary"));
    EXPECT_FALSE(Grammar::is_identifier("_id"));
    EXPECT_FALSE(Grammar::is_identifier("2016id"));
    EXPECT_FALSE(Grammar::is_identifier(" id"));
    EXPECT_FALSE(Grammar::is_identifier("id "));
}

TEST(Grammar, is_namespace) {
    EXPECT_TRUE(Grammar::is_namespace("s"));
    EXPECT_TRUE(Grammar::is_namespace("std"));
    EXPECT_TRUE(Grammar::is_namespace("std2016"));
    EXPECT_TRUE(Grammar::is_namespace("std_2016"));
}

TEST(Grammar, is_not_namespace) {
    EXPECT_FALSE(Grammar::is_namespace(""));
    EXPECT_FALSE(Grammar::is_namespace("null"));
    EXPECT_FALSE(Grammar::is_namespace("true"));
    EXPECT_FALSE(Grammar::is_namespace("false"));
    EXPECT_FALSE(Grammar::is_namespace("Connection"));
    EXPECT_FALSE(Grammar::is_namespace("Context"));
    EXPECT_FALSE(Grammar::is_namespace("Boolean"));
    EXPECT_FALSE(Grammar::is_namespace("Character"));
    EXPECT_FALSE(Grammar::is_namespace("Integer"));
    EXPECT_FALSE(Grammar::is_namespace("Float"));
    EXPECT_FALSE(Grammar::is_namespace("String"));
    EXPECT_FALSE(Grammar::is_namespace("Vector"));
    EXPECT_FALSE(Grammar::is_namespace("Set"));
    EXPECT_FALSE(Grammar::is_namespace("Dictionary"));
    EXPECT_FALSE(Grammar::is_namespace("std:"));
    EXPECT_FALSE(Grammar::is_namespace("_std"));
    EXPECT_FALSE(Grammar::is_namespace("2016std"));
    EXPECT_FALSE(Grammar::is_namespace(" std"));
    EXPECT_FALSE(Grammar::is_namespace("std "));
}

TEST(Grammar, is_variable) {
    EXPECT_TRUE(Grammar::is_variable("i"));
    EXPECT_TRUE(Grammar::is_variable("prices"));
    EXPECT_TRUE(Grammar::is_variable("prices2016"));
    EXPECT_TRUE(Grammar::is_variable("prices_2016"));
    EXPECT_TRUE(Grammar::is_variable("std::i"));
    EXPECT_TRUE(Grammar::is_variable("std::prices"));
    EXPECT_TRUE(Grammar::is_variable("std::prices2016"));
    EXPECT_TRUE(Grammar::is_variable("std::prices_2016"));
}

TEST(Grammar, is_not_variable) {
    EXPECT_FALSE(Grammar::is_variable(""));
    EXPECT_FALSE(Grammar::is_variable("null"));
    EXPECT_FALSE(Grammar::is_variable("true"));
    EXPECT_FALSE(Grammar::is_variable("false"));
    EXPECT_FALSE(Grammar::is_variable("Connection"));
    EXPECT_FALSE(Grammar::is_variable("Context"));
    EXPECT_FALSE(Grammar::is_variable("Boolean"));
    EXPECT_FALSE(Grammar::is_variable("Character"));
    EXPECT_FALSE(Grammar::is_variable("Integer"));
    EXPECT_FALSE(Grammar::is_variable("Float"));
    EXPECT_FALSE(Grammar::is_variable("String"));
    EXPECT_FALSE(Grammar::is_variable("Vector"));
    EXPECT_FALSE(Grammar::is_variable("Set"));
    EXPECT_FALSE(Grammar::is_variable("Dictionary"));
    EXPECT_FALSE(Grammar::is_variable("std::"));
    EXPECT_FALSE(Grammar::is_variable("::std"));
    EXPECT_FALSE(Grammar::is_variable("std:::"));
    EXPECT_FALSE(Grammar::is_variable("std:::prices"));
    EXPECT_FALSE(Grammar::is_variable("_prices"));
    EXPECT_FALSE(Grammar::is_variable("std::_prices"));
    EXPECT_FALSE(Grammar::is_variable("2016prices"));
    EXPECT_FALSE(Grammar::is_variable("std::2016prices"));
    EXPECT_FALSE(Grammar::is_variable("std:prices"));
    EXPECT_FALSE(Grammar::is_variable("std_one::std_two::prices"));
    EXPECT_FALSE(Grammar::is_variable(" prices"));
    EXPECT_FALSE(Grammar::is_variable("prices "));
}

TEST(Grammar, is_object) {
    EXPECT_TRUE(Grammar::is_object("Connection"));
    EXPECT_TRUE(Grammar::is_object("Context"));
    EXPECT_TRUE(Grammar::is_object("Boolean"));
    EXPECT_TRUE(Grammar::is_object("Character"));
    EXPECT_TRUE(Grammar::is_object("Integer"));
    EXPECT_TRUE(Grammar::is_object("Float"));
    EXPECT_TRUE(Grammar::is_object("String"));
    EXPECT_TRUE(Grammar::is_object("Vector<Boolean>"));
    EXPECT_TRUE(Grammar::is_object("Set<Float>"));
    EXPECT_TRUE(Grammar::is_object("Dictionary<String, Integer>"));
    EXPECT_TRUE(Grammar::is_object("prices"));
    EXPECT_TRUE(Grammar::is_object("prices2016"));
    EXPECT_TRUE(Grammar::is_object("prices_2016"));
    EXPECT_TRUE(Grammar::is_object("std::prices"));
    EXPECT_TRUE(Grammar::is_object("std::prices2016"));
    EXPECT_TRUE(Grammar::is_object("std::prices_2016"));
}

TEST(Grammar, is_not_object) {
    EXPECT_FALSE(Grammar::is_object(""));
    EXPECT_FALSE(Grammar::is_object("42"));
    EXPECT_FALSE(Grammar::is_object("isEmpty?"));
    EXPECT_FALSE(Grammar::is_object(" prices"));
    EXPECT_FALSE(Grammar::is_object("prices "));
}

TEST(Grammar, is_unary_message) {
    EXPECT_TRUE(Grammar::is_unary_message("get"));
    EXPECT_TRUE(Grammar::is_unary_message("isEmpty?"));
}

TEST(Grammar, is_not_unary_message) {
    EXPECT_FALSE(Grammar::is_unary_message(""));
    EXPECT_FALSE(Grammar::is_unary_message("42"));
    EXPECT_FALSE(Grammar::is_unary_message("get:"));
    EXPECT_FALSE(Grammar::is_unary_message(" get"));
    EXPECT_FALSE(Grammar::is_unary_message("get "));
}

TEST(Grammar, is_keyword_message_token) {
    EXPECT_TRUE(Grammar::is_keyword_message_token("withValue:"));
    EXPECT_TRUE(Grammar::is_keyword_message_token("isEqualTo?"));
}

TEST(Grammar, is_not_keyword_message_token) {
    EXPECT_FALSE(Grammar::is_keyword_message_token(""));
    EXPECT_FALSE(Grammar::is_keyword_message_token("42:"));
    EXPECT_FALSE(Grammar::is_keyword_message_token("true?"));
    EXPECT_FALSE(Grammar::is_keyword_message_token("withValue"));
    EXPECT_FALSE(Grammar::is_keyword_message_token(" withValue:"));
    EXPECT_FALSE(Grammar::is_keyword_message_token("withValue: "));
}

TEST(Grammar, is_keyword_message_argument) {
    EXPECT_TRUE(Grammar::is_keyword_message_argument("42"));
    EXPECT_TRUE(Grammar::is_keyword_message_argument("std"));
    EXPECT_TRUE(Grammar::is_keyword_message_argument("std::prices"));
}

TEST(Grammar, is_not_keyword_message_argument) {
    EXPECT_FALSE(Grammar::is_keyword_message_argument(""));
    EXPECT_FALSE(Grammar::is_keyword_message_argument("Connection"));
    EXPECT_FALSE(Grammar::is_keyword_message_argument("Context"));
    EXPECT_FALSE(Grammar::is_keyword_message_argument("Boolean"));
    EXPECT_FALSE(Grammar::is_keyword_message_argument("Vector"));
    EXPECT_FALSE(Grammar::is_keyword_message_argument("withValue:"));
    EXPECT_FALSE(Grammar::is_keyword_message_argument("isEmpty?"));
    EXPECT_FALSE(Grammar::is_keyword_message_argument(" 42"));
    EXPECT_FALSE(Grammar::is_keyword_message_argument("42 "));
}

TEST(Grammar, is_keyword_message_part) {
    EXPECT_TRUE(Grammar::is_keyword_message_part("withValue: 42"));
    EXPECT_TRUE(Grammar::is_keyword_message_part("create: std::prices"));
    EXPECT_TRUE(Grammar::is_keyword_message_part("deleteVariablesOfNamespace: std"));
}

TEST(Grammar, is_keyword_message_part_corner_cases) {
    EXPECT_TRUE(Grammar::is_keyword_message_part("withValue:42"));
    EXPECT_TRUE(Grammar::is_keyword_message_part("create:   std::prices"));
}

TEST(Grammar, is_not_keyword_message_part) {
    EXPECT_FALSE(Grammar::is_keyword_message_part(""));
    EXPECT_FALSE(Grammar::is_keyword_message_part("get"));
    EXPECT_FALSE(Grammar::is_keyword_message_part("withValue:"));
    EXPECT_FALSE(Grammar::is_keyword_message_part("withValue 42"));
    EXPECT_FALSE(Grammar::is_keyword_message_part("withValue: Integer"));
    EXPECT_FALSE(Grammar::is_keyword_message_part("create: withValue:"));
    EXPECT_FALSE(Grammar::is_keyword_message_part(" withValue: 42"));
    EXPECT_FALSE(Grammar::is_keyword_message_part("withValue: 42 "));
}

TEST(Grammar, is_keyword_message) {
    EXPECT_TRUE(Grammar::is_keyword_message("isEqualTo? 42"));
    EXPECT_TRUE(Grammar::is_keyword_message("create: i withValue: 42"));
}

TEST(Grammar, is_keyword_message_corner_cases) {
    EXPECT_TRUE(Grammar::is_keyword_message("isEqualTo?42"));
    EXPECT_TRUE(Grammar::is_keyword_message("create: i   withValue: 42"));
}

TEST(Grammar, is_not_keyword_message) {
    EXPECT_FALSE(Grammar::is_keyword_message(""));
    EXPECT_FALSE(Grammar::is_keyword_message("get"));
    EXPECT_FALSE(Grammar::is_keyword_message("isEmpty?"));
    EXPECT_FALSE(Grammar::is_keyword_message("withValue 42"));
    EXPECT_FALSE(Grammar::is_keyword_message("create: withValue:"));
    EXPECT_FALSE(Grammar::is_keyword_message("create: i withValue:"));
    EXPECT_FALSE(Grammar::is_keyword_message("create: i 42"));
    EXPECT_FALSE(Grammar::is_keyword_message("i withValue: 42"));
    EXPECT_FALSE(Grammar::is_keyword_message(" create: i withValue: 42"));
    EXPECT_FALSE(Grammar::is_keyword_message("create: i withValue: 42 "));
}

TEST(Grammar, is_statement) {
    EXPECT_TRUE(Grammar::is_statement("Connection close;"));
    EXPECT_TRUE(Grammar::is_statement("Context listVariables;"));
    EXPECT_TRUE(Grammar::is_statement("Integer create: i withValue: 42;"));
    EXPECT_TRUE(Grammar::is_statement("Dictionary<String, Integer> create: ages;"));
    EXPECT_TRUE(Grammar::is_statement("i isEqualTo? 42;"));
}

TEST(Grammar, is_statement_corner_cases) {
    EXPECT_TRUE(Grammar::is_statement("Integer create:i withValue:42;"));
    EXPECT_TRUE(Grammar::is_statement("  Integer  create:  i   withValue:  42  ;  "));
}

TEST(Grammar, is_not_statement) {
    EXPECT_FALSE(Grammar::is_statement(""));
    EXPECT_FALSE(Grammar::is_statement(";"));
    EXPECT_FALSE(Grammar::is_statement("true;"));
    EXPECT_FALSE(Grammar::is_statement("42 get;"));
    EXPECT_FALSE(Grammar::is_statement("Integer create: i"));
    EXPECT_FALSE(Grammar::is_statement("Integer create: ;"));
    EXPECT_FALSE(Grammar::is_statement("create: i withValue: 42;"));
}
