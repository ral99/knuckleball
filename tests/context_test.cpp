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
#include "exceptions.h"

class ContextTest: public ::testing::Test {
protected:
    Context *context;

    virtual void SetUp() {
        context = Context::get_instance("", 3, 0.001, true);
    }
};

//////////////////////////////////////////////////////// Context ///////////////////////////////////////////////////////

TEST_F(ContextTest, Context_listNamespaces) {
    EXPECT_EQ(context->execute("Context listNamespaces;"), "[]");
    context->execute("Boolean create: t withValue: true;");
    context->execute("Character create: std::numeric withValue: '1';");
    context->execute("Integer create: uni::i withValue: 42;");
    context->execute("String create: std::str withValue: \"knuckleball\";");
    EXPECT_EQ(context->execute("Context listNamespaces;"), "[std,uni]");
}

TEST_F(ContextTest, Context_listVariables) {
    EXPECT_EQ(context->execute("Context listVariables;"), "[]");
    context->execute("Boolean create: t withValue: true;");
    context->execute("Character create: numeric withValue: '1';");
    context->execute("Integer create: i withValue: 42;");
    EXPECT_EQ(context->execute("Context listVariables;"), "[i,numeric,t]");
}

TEST_F(ContextTest, Context_listVariablesOfNamespace) {
    EXPECT_EQ(context->execute("Context listVariablesOfNamespace: std;"), "[]");
    context->execute("Boolean create: std::t withValue: true;");
    context->execute("Character create: numeric withValue: '1';");
    context->execute("Integer create: std::i withValue: 42;");
    EXPECT_EQ(context->execute("Context listVariablesOfNamespace: std;"), "[std::i,std::t]");
}

TEST_F(ContextTest, Context_deleteVariable) {
    EXPECT_EQ(context->execute("Context deleteVariable: numeric;"), EXC_UNEXISTENT_VARIABLE);
    context->execute("Boolean create: t withValue: true;");
    context->execute("Character create: numeric withValue: '1';");
    context->execute("Integer create: i withValue: 42;");
    EXPECT_EQ(context->execute("Context deleteVariable: numeric;"), "null");
    EXPECT_EQ(context->execute("Context listVariables;"), "[i,t]");
}

TEST_F(ContextTest, Context_deleteVariablesOfNamespace) {
    EXPECT_EQ(context->execute("Context deleteVariablesOfNamespace: std;"), "null");
    context->execute("Boolean create: std::t withValue: true;");
    context->execute("Character create: numeric withValue: '1';");
    context->execute("Integer create: std::i withValue: 42;");
    EXPECT_EQ(context->execute("Context deleteVariablesOfNamespace: std;"), "null");
    EXPECT_EQ(context->execute("Context listVariables;"), "[numeric]");
}

TEST_F(ContextTest, Context_getFloatPrecision) {
    EXPECT_EQ(context->execute("Context getFloatPrecision;"), "3");
}

TEST_F(ContextTest, Context_setFloatPrecision) {
    EXPECT_EQ(context->execute("Context setFloatPrecision: 42;"), "null");
    EXPECT_EQ(context->execute("Context getFloatPrecision;"), "42");

    // invalid precision:
    EXPECT_EQ(context->execute("Context setFloatPrecision: -42;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Context_getFloatComparisonTolerance) {
    EXPECT_EQ(context->execute("Context getFloatComparisonTolerance;"), "0.001");
}

TEST_F(ContextTest, Context_setFloatComparisonTolerance) {
    EXPECT_EQ(context->execute("Context setFloatComparisonTolerance: 1e-2;"), "null");
    EXPECT_EQ(context->execute("Context getFloatComparisonTolerance;"), "0.010");

    // invalid tolerance:
    EXPECT_EQ(context->execute("Context setFloatComparisonTolerance: -42;"), EXC_INVALID_ARGUMENT);
}

//////////////////////////////////////////////////////// Boolean ///////////////////////////////////////////////////////

TEST_F(ContextTest, Boolean_create_withValue) {
    EXPECT_EQ(context->execute("Boolean create: t withValue: true;"), "null");
    EXPECT_EQ(context->execute("t get;"), "true");
}

TEST_F(ContextTest, Boolean_createIfNotExists_withValue) {
    EXPECT_EQ(context->execute("Boolean createIfNotExists: f withValue: false;"), "null");
    EXPECT_EQ(context->execute("f get;"), "false");
    EXPECT_EQ(context->execute("Boolean createIfNotExists: f withValue: true;"), "null");
    EXPECT_EQ(context->execute("f get;"), "false");
}

TEST_F(ContextTest, Boolean_get) {
    EXPECT_EQ(context->execute("Boolean create: t withValue: true;"), "null");
    EXPECT_EQ(context->execute("t get;"), "true");
}

TEST_F(ContextTest, Boolean_set) {
    EXPECT_EQ(context->execute("Boolean create: f withValue: false;"), "null");
    EXPECT_EQ(context->execute("f set: true;"), "null");
    EXPECT_EQ(context->execute("f get;"), "true");
}

TEST_F(ContextTest, Boolean_isTrue) {
    // true value:
    EXPECT_EQ(context->execute("Boolean create: t withValue: true;"), "null");
    EXPECT_EQ(context->execute("t isTrue?;"), "true");

    // false value:
    EXPECT_EQ(context->execute("Boolean create: f withValue: false;"), "null");
    EXPECT_EQ(context->execute("f isTrue?;"), "false");
}

TEST_F(ContextTest, Boolean_isFalse) {
    // true value:
    EXPECT_EQ(context->execute("Boolean create: t withValue: true;"), "null");
    EXPECT_EQ(context->execute("t isFalse?;"), "false");

    // false value:
    EXPECT_EQ(context->execute("Boolean create: f withValue: false;"), "null");
    EXPECT_EQ(context->execute("f isFalse?;"), "true");
}

/////////////////////////////////////////////////////// Character //////////////////////////////////////////////////////

TEST_F(ContextTest, Character_create_withValue) {
    EXPECT_EQ(context->execute("Character create: alphabetic withValue: 'b';"), "null");
    EXPECT_EQ(context->execute("alphabetic get;"), "'b'");
}

TEST_F(ContextTest, Character_createIfNotExists_withValue) {
    EXPECT_EQ(context->execute("Character createIfNotExists: alphabetic withValue: 'b';"), "null");
    EXPECT_EQ(context->execute("alphabetic get;"), "'b'");
    EXPECT_EQ(context->execute("Character createIfNotExists: alphabetic withValue: 'a';"), "null");
    EXPECT_EQ(context->execute("alphabetic get;"), "'b'");
}

TEST_F(ContextTest, Character_get) {
    EXPECT_EQ(context->execute("Character create: alphabetic withValue: 'b';"), "null");
    EXPECT_EQ(context->execute("alphabetic get;"), "'b'");
}

TEST_F(ContextTest, Character_set) {
    EXPECT_EQ(context->execute("Character create: alphabetic withValue: 'b';"), "null");
    EXPECT_EQ(context->execute("alphabetic set: 'a';"), "null");
    EXPECT_EQ(context->execute("alphabetic get;"), "'a'");
}

TEST_F(ContextTest, Character_isAlphabetic) {
    // alphabetic value:
    EXPECT_EQ(context->execute("Character create: alphabetic withValue: 'b';"), "null");
    EXPECT_EQ(context->execute("alphabetic isAlphabetic?;"), "true");

    // numeric value:
    EXPECT_EQ(context->execute("Character create: numeric withValue: '1';"), "null");
    EXPECT_EQ(context->execute("numeric isAlphabetic?;"), "false");

    // space value:
    EXPECT_EQ(context->execute("Character create: space withValue: ' ';"), "null");
    EXPECT_EQ(context->execute("space isAlphabetic?;"), "false");
}

TEST_F(ContextTest, Character_isNumeric) {
    // alphabetic value:
    EXPECT_EQ(context->execute("Character create: alphabetic withValue: 'b';"), "null");
    EXPECT_EQ(context->execute("alphabetic isNumeric?;"), "false");

    // numeric value:
    EXPECT_EQ(context->execute("Character create: numeric withValue: '1';"), "null");
    EXPECT_EQ(context->execute("numeric isNumeric?;"), "true");

    // space value:
    EXPECT_EQ(context->execute("Character create: space withValue: ' ';"), "null");
    EXPECT_EQ(context->execute("space isNumeric?;"), "false");
}

TEST_F(ContextTest, Character_isAlphanumeric) {
    // alphabetic value:
    EXPECT_EQ(context->execute("Character create: alphabetic withValue: 'b';"), "null");
    EXPECT_EQ(context->execute("alphabetic isAlphanumeric?;"), "true");

    // numeric value:
    EXPECT_EQ(context->execute("Character create: numeric withValue: '1';"), "null");
    EXPECT_EQ(context->execute("numeric isAlphanumeric?;"), "true");

    // space value:
    EXPECT_EQ(context->execute("Character create: space withValue: ' ';"), "null");
    EXPECT_EQ(context->execute("space isAlphanumeric?;"), "false");
}

TEST_F(ContextTest, Character_isSpace) {
    // alphabetic value:
    EXPECT_EQ(context->execute("Character create: alphabetic withValue: 'b';"), "null");
    EXPECT_EQ(context->execute("alphabetic isSpace?;"), "false");

    // numeric value:
    EXPECT_EQ(context->execute("Character create: numeric withValue: '1';"), "null");
    EXPECT_EQ(context->execute("numeric isSpace?;"), "false");

    // space value:
    EXPECT_EQ(context->execute("Character create: space withValue: ' ';"), "null");
    EXPECT_EQ(context->execute("space isSpace?;"), "true");
}

TEST_F(ContextTest, Character_isEqualTo) {
    EXPECT_EQ(context->execute("Character create: alphabetic withValue: 'b';"), "null");
    EXPECT_EQ(context->execute("alphabetic isEqualTo? 'b';"), "true");
    EXPECT_EQ(context->execute("alphabetic isEqualTo? 'a';"), "false");
}

TEST_F(ContextTest, Character_isLessThan) {
    EXPECT_EQ(context->execute("Character create: numeric withValue: '1';"), "null");
    EXPECT_EQ(context->execute("numeric isLessThan? '2';"), "true");
    EXPECT_EQ(context->execute("numeric isLessThan? '1';"), "false");
}

TEST_F(ContextTest, Character_isLessThanOrEqualTo) {
    EXPECT_EQ(context->execute("Character create: numeric withValue: '1';"), "null");
    EXPECT_EQ(context->execute("numeric isLessThanOrEqualTo? '1';"), "true");
    EXPECT_EQ(context->execute("numeric isLessThanOrEqualTo? '0';"), "false");
}

TEST_F(ContextTest, Character_isGreaterThan) {
    EXPECT_EQ(context->execute("Character create: numeric withValue: '1';"), "null");
    EXPECT_EQ(context->execute("numeric isGreaterThan? '0';"), "true");
    EXPECT_EQ(context->execute("numeric isGreaterThan? '1';"), "false");
}

TEST_F(ContextTest, Character_isGreaterThanOrEqualTo) {
    EXPECT_EQ(context->execute("Character create: numeric withValue: '1';"), "null");
    EXPECT_EQ(context->execute("numeric isGreaterThanOrEqualTo? '1';"), "true");
    EXPECT_EQ(context->execute("numeric isGreaterThanOrEqualTo? '2';"), "false");
}

/////////////////////////////////////////////////////// Integer ////////////////////////////////////////////////////////

TEST_F(ContextTest, Integer_create_withValue) {
    // integer value:
    EXPECT_EQ(context->execute("Integer create: i withValue: 42;"), "null");
    EXPECT_EQ(context->execute("i get;"), "42");

    // float value:
    EXPECT_EQ(context->execute("Integer create: f withValue: 42.9;"), "null");
    EXPECT_EQ(context->execute("f get;"), "42");
}

TEST_F(ContextTest, Integer_createIfNotExists_withValue) {
    // integer value:
    EXPECT_EQ(context->execute("Integer createIfNotExists: i withValue: 42;"), "null");
    EXPECT_EQ(context->execute("i get;"), "42");
    EXPECT_EQ(context->execute("Integer createIfNotExists: i withValue: 43;"), "null");
    EXPECT_EQ(context->execute("i get;"), "42");

    // float value:
    EXPECT_EQ(context->execute("Integer createIfNotExists: f withValue: 42.9;"), "null");
    EXPECT_EQ(context->execute("f get;"), "42");
    EXPECT_EQ(context->execute("Integer createIfNotExists: f withValue: 43.9;"), "null");
    EXPECT_EQ(context->execute("f get;"), "42");
}

TEST_F(ContextTest, Integer_get) {
    EXPECT_EQ(context->execute("Integer create: i withValue: 42;"), "null");
    EXPECT_EQ(context->execute("i get;"), "42");
}

TEST_F(ContextTest, Integer_set) {
    // integer value:
    EXPECT_EQ(context->execute("Integer create: i withValue: 42;"), "null");
    EXPECT_EQ(context->execute("i set: 43;"), "null");
    EXPECT_EQ(context->execute("i get;"), "43");

    // float value:
    EXPECT_EQ(context->execute("Integer create: f withValue: 42;"), "null");
    EXPECT_EQ(context->execute("f set: 43.9;"), "null");
    EXPECT_EQ(context->execute("f get;"), "43");
}

TEST_F(ContextTest, Integer_isEqualTo) {
    EXPECT_EQ(context->execute("Integer create: i withValue: 42;"), "null");
    EXPECT_EQ(context->execute("i isEqualTo? 42;"), "true");
    EXPECT_EQ(context->execute("i isEqualTo? 42.0;"), "true");
    EXPECT_EQ(context->execute("i isEqualTo? 43;"), "false");
    EXPECT_EQ(context->execute("i isEqualTo? 42.1;"), "false");
}

TEST_F(ContextTest, Integer_isLessThan) {
    EXPECT_EQ(context->execute("Integer create: i withValue: 42;"), "null");
    EXPECT_EQ(context->execute("i isLessThan? 43;"), "true");
    EXPECT_EQ(context->execute("i isLessThan? 42.1;"), "true");
    EXPECT_EQ(context->execute("i isLessThan? 42;"), "false");
    EXPECT_EQ(context->execute("i isLessThan? 42.0;"), "false");
}

TEST_F(ContextTest, Integer_isLessThanOrEqualTo) {
    EXPECT_EQ(context->execute("Integer create: i withValue: 42;"), "null");
    EXPECT_EQ(context->execute("i isLessThanOrEqualTo? 42;"), "true");
    EXPECT_EQ(context->execute("i isLessThanOrEqualTo? 42.0;"), "true");
    EXPECT_EQ(context->execute("i isLessThanOrEqualTo? 41;"), "false");
    EXPECT_EQ(context->execute("i isLessThanOrEqualTo? 41.9;"), "false");
}

TEST_F(ContextTest, Integer_isGreaterThan) {
    EXPECT_EQ(context->execute("Integer create: i withValue: 42;"), "null");
    EXPECT_EQ(context->execute("i isGreaterThan? 41;"), "true");
    EXPECT_EQ(context->execute("i isGreaterThan? 41.9;"), "true");
    EXPECT_EQ(context->execute("i isGreaterThan? 42;"), "false");
    EXPECT_EQ(context->execute("i isGreaterThan? 42.0;"), "false");
}

TEST_F(ContextTest, Integer_isGreaterThanOrEqualTo) {
    EXPECT_EQ(context->execute("Integer create: i withValue: 42;"), "null");
    EXPECT_EQ(context->execute("i isGreaterThanOrEqualTo? 42;"), "true");
    EXPECT_EQ(context->execute("i isGreaterThanOrEqualTo? 42.0;"), "true");
    EXPECT_EQ(context->execute("i isGreaterThanOrEqualTo? 43;"), "false");
    EXPECT_EQ(context->execute("i isGreaterThanOrEqualTo? 42.1;"), "false");
}

TEST_F(ContextTest, Integer_add) {
    EXPECT_EQ(context->execute("Integer create: i withValue: 42;"), "null");
    EXPECT_EQ(context->execute("i add: 42;"), "null");
    EXPECT_EQ(context->execute("i get;"), "84");
    EXPECT_EQ(context->execute("i add: -42.5;"), "null");
    EXPECT_EQ(context->execute("i get;"), "41");
}

TEST_F(ContextTest, Integer_subtract) {
    EXPECT_EQ(context->execute("Integer create: i withValue: 42;"), "null");
    EXPECT_EQ(context->execute("i subtract: 84;"), "null");
    EXPECT_EQ(context->execute("i get;"), "-42");
    EXPECT_EQ(context->execute("i subtract: -84.5;"), "null");
    EXPECT_EQ(context->execute("i get;"), "42");
}

TEST_F(ContextTest, Integer_multiplyBy) {
    EXPECT_EQ(context->execute("Integer create: i withValue: 20;"), "null");
    EXPECT_EQ(context->execute("i multiplyBy: 2;"), "null");
    EXPECT_EQ(context->execute("i get;"), "40");
    EXPECT_EQ(context->execute("i multiplyBy: -2.5;"), "null");
    EXPECT_EQ(context->execute("i get;"), "-100");
}

TEST_F(ContextTest, Integer_divideBy) {
    EXPECT_EQ(context->execute("Integer create: i withValue: 100;"), "null");
    EXPECT_EQ(context->execute("i divideBy: 2.5;"), "null");
    EXPECT_EQ(context->execute("i get;"), "40");
    EXPECT_EQ(context->execute("i divideBy: -2;"), "null");
    EXPECT_EQ(context->execute("i get;"), "-20");
}

//////////////////////////////////////////////////////// Float /////////////////////////////////////////////////////////

TEST_F(ContextTest, Float_create_withValue) {
    EXPECT_EQ(context->execute("Float create: f withValue: 42.5;"), "null");
    EXPECT_EQ(context->execute("f get;"), "42.500");
}

TEST_F(ContextTest, Float_createIfNotExists_withValue) {
    EXPECT_EQ(context->execute("Float createIfNotExists: f withValue: 42.5;"), "null");
    EXPECT_EQ(context->execute("f get;"), "42.500");
    EXPECT_EQ(context->execute("Float createIfNotExists: f withValue: 43.5;"), "null");
    EXPECT_EQ(context->execute("f get;"), "42.500");
}

TEST_F(ContextTest, Float_get) {
    EXPECT_EQ(context->execute("Float create: f withValue: 42.5;"), "null");
    EXPECT_EQ(context->execute("f get;"), "42.500");
}

TEST_F(ContextTest, Float_set) {
    EXPECT_EQ(context->execute("Float create: f withValue: 42.5;"), "null");
    EXPECT_EQ(context->execute("f set: 43.5;"), "null");
    EXPECT_EQ(context->execute("f get;"), "43.500");
}

TEST_F(ContextTest, Float_isEqualTo) {
    EXPECT_EQ(context->execute("Float create: f withValue: 42.5;"), "null");
    EXPECT_EQ(context->execute("f isEqualTo? 42.5;"), "true");
    EXPECT_EQ(context->execute("f isEqualTo? 42.0;"), "false");
}

TEST_F(ContextTest, Float_isLessThan) {
    EXPECT_EQ(context->execute("Float create: f withValue: 42.5;"), "null");
    EXPECT_EQ(context->execute("f isLessThan? 43.0;"), "true");
    EXPECT_EQ(context->execute("f isLessThan? 42.5;"), "false");
}

TEST_F(ContextTest, Float_isLessThanOrEqualTo) {
    EXPECT_EQ(context->execute("Float create: f withValue: 42.5;"), "null");
    EXPECT_EQ(context->execute("f isLessThanOrEqualTo? 42.5;"), "true");
    EXPECT_EQ(context->execute("f isLessThanOrEqualTo? 42.0;"), "false");
}

TEST_F(ContextTest, Float_isGreaterThan) {
    EXPECT_EQ(context->execute("Float create: f withValue: 42.5;"), "null");
    EXPECT_EQ(context->execute("f isGreaterThan? 42.0;"), "true");
    EXPECT_EQ(context->execute("f isGreaterThan? 42.5;"), "false");
}

TEST_F(ContextTest, Float_isGreaterThanOrEqualTo) {
    EXPECT_EQ(context->execute("Float create: f withValue: 42.5;"), "null");
    EXPECT_EQ(context->execute("f isGreaterThanOrEqualTo? 42.5;"), "true");
    EXPECT_EQ(context->execute("f isGreaterThanOrEqualTo? 43.0;"), "false");
}

TEST_F(ContextTest, Float_add) {
    EXPECT_EQ(context->execute("Float create: f withValue: 42.5;"), "null");
    EXPECT_EQ(context->execute("f add: .5;"), "null");
    EXPECT_EQ(context->execute("f get;"), "43.000");
}

TEST_F(ContextTest, Float_subtract) {
    EXPECT_EQ(context->execute("Float create: f withValue: 42.5;"), "null");
    EXPECT_EQ(context->execute("f subtract: .5;"), "null");
    EXPECT_EQ(context->execute("f get;"), "42.000");
}

TEST_F(ContextTest, Float_multiplyBy) {
    EXPECT_EQ(context->execute("Float create: f withValue: 42.5;"), "null");
    EXPECT_EQ(context->execute("f multiplyBy: 2;"), "null");
    EXPECT_EQ(context->execute("f get;"), "85.000");
}

TEST_F(ContextTest, Float_divideBy) {
    EXPECT_EQ(context->execute("Float create: f withValue: 85;"), "null");
    EXPECT_EQ(context->execute("f divideBy: 2;"), "null");
    EXPECT_EQ(context->execute("f get;"), "42.500");
}

//////////////////////////////////////////////////////// String ////////////////////////////////////////////////////////

TEST_F(ContextTest, String_create) {
    EXPECT_EQ(context->execute("String create: str;"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"\"");
}

TEST_F(ContextTest, String_createIfNotExists) {
    EXPECT_EQ(context->execute("String createIfNotExists: str;"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"\"");
    EXPECT_EQ(context->execute("String createIfNotExists: str;"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"\"");
}

TEST_F(ContextTest, String_create_withValue) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"knuckleball\"");
}

TEST_F(ContextTest, String_createIfNotExists_withValue) {
    EXPECT_EQ(context->execute("String createIfNotExists: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"knuckleball\"");
    EXPECT_EQ(context->execute("String createIfNotExists: str withValue: \"\";"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"knuckleball\"");
}

TEST_F(ContextTest, String_get) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"knuckleball\"");
}

TEST_F(ContextTest, String_set) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str set: \"\";"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"\"");
}

TEST_F(ContextTest, String_isEmpty) {
    // empty string:
    EXPECT_EQ(context->execute("String create: empty_str;"), "null");
    EXPECT_EQ(context->execute("empty_str isEmpty?;"), "true");

    // non-empty string:
    EXPECT_EQ(context->execute("String create: non_empty_str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("non_empty_str isEmpty?;"), "false");
}

TEST_F(ContextTest, String_startsWith) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str startsWith? \"\";"), "true");
    EXPECT_EQ(context->execute("str startsWith? \"knuckle\";"), "true");
    EXPECT_EQ(context->execute("str startsWith? \"knuckleball\";"), "true");
    EXPECT_EQ(context->execute("str startsWith? \"ball\";"), "false");
}

TEST_F(ContextTest, String_endsWith) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str endsWith? \"\";"), "true");
    EXPECT_EQ(context->execute("str endsWith? \"ball\";"), "true");
    EXPECT_EQ(context->execute("str endsWith? \"knuckleball\";"), "true");
    EXPECT_EQ(context->execute("str endsWith? \"knuckle\";"), "false");
}

TEST_F(ContextTest, String_isLexicographicallyEqualTo) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str isLexicographicallyEqualTo? \"knuckleball\";"), "true");
    EXPECT_EQ(context->execute("str isLexicographicallyEqualTo? \"\";"), "false");
}

TEST_F(ContextTest, String_isLexicographicallyLessThan) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str isLexicographicallyLessThan? \"zball\";"), "true");
    EXPECT_EQ(context->execute("str isLexicographicallyLessThan? \"knuckleball\";"), "false");
}

TEST_F(ContextTest, String_isLexicographicallyLessThanOrEqualTo) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str isLexicographicallyLessThanOrEqualTo? \"knuckleball\";"), "true");
    EXPECT_EQ(context->execute("str isLexicographicallyLessThanOrEqualTo? \"\";"), "false");
}

TEST_F(ContextTest, String_isLexicographicallyGreaterThan) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str isLexicographicallyGreaterThan? \"\";"), "true");
    EXPECT_EQ(context->execute("str isLexicographicallyGreaterThan? \"knuckleball\";"), "false");
}

TEST_F(ContextTest, String_isLexicographicallyGreaterThanOrEqualTo) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str isLexicographicallyGreaterThanOrEqualTo? \"knuckleball\";"), "true");
    EXPECT_EQ(context->execute("str isLexicographicallyGreaterThanOrEqualTo? \"zball\";"), "false");
}

TEST_F(ContextTest, String_length) {
    // empty string:
    EXPECT_EQ(context->execute("String create: empty_str withValue: \"\";"), "null");
    EXPECT_EQ(context->execute("empty_str length;"), "0");

    // non-empty string:
    EXPECT_EQ(context->execute("String create: non_empty_str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("non_empty_str length;"), "11");
}

TEST_F(ContextTest, String_atIndex) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str atIndex: 0;"), "'k'");
    EXPECT_EQ(context->execute("str atIndex: 1;"), "'n'");
    EXPECT_EQ(context->execute("str atIndex: 2;"), "'u'");

    // invalid index:
    EXPECT_EQ(context->execute("str atIndex: -1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("str atIndex: 42;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, String_first) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str first: 0;"), "\"\"");
    EXPECT_EQ(context->execute("str first: 7;"), "\"knuckle\"");
    EXPECT_EQ(context->execute("str first: 42;"), "\"knuckleball\"");

    // invalid n:
    EXPECT_EQ(context->execute("str first: -1;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, String_last) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str last: 0;"), "\"\"");
    EXPECT_EQ(context->execute("str last: 4;"), "\"ball\"");
    EXPECT_EQ(context->execute("str last: 42;"), "\"knuckleball\"");

    // invalid n:
    EXPECT_EQ(context->execute("str last: -1;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, String_substringFromIndex) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str substringFromIndex: 0;"), "\"knuckleball\"");
    EXPECT_EQ(context->execute("str substringFromIndex: 7;"), "\"ball\"");
    EXPECT_EQ(context->execute("str substringFromIndex: 11;"), "\"\"");

    // invalid index:
    EXPECT_EQ(context->execute("str substringFromIndex: -1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("str substringFromIndex: 12;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, String_substringFromIndex_toIndex) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str substringFromIndex: 0 toIndex: 0;"), "\"\"");
    EXPECT_EQ(context->execute("str substringFromIndex: 0 toIndex: 7;"), "\"knuckle\"");
    EXPECT_EQ(context->execute("str substringFromIndex: 0 toIndex: 11;"), "\"knuckleball\"");

    // invalid index:
    EXPECT_EQ(context->execute("str substringFromIndex: 4 toIndex: 2;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("str substringFromIndex: -1 toIndex: 2;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, String_pushBack) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knucklebal\";"), "null");
    EXPECT_EQ(context->execute("str pushBack: 'l';"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"knuckleball\"");
}

TEST_F(ContextTest, String_pushFront) {
    EXPECT_EQ(context->execute("String create: str withValue: \"nuckleball\";"), "null");
    EXPECT_EQ(context->execute("str pushFront: 'k';"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"knuckleball\"");
}

TEST_F(ContextTest, String_popBack) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knu\";"), "null");
    EXPECT_EQ(context->execute("str popBack;"), "'u'");
    EXPECT_EQ(context->execute("str popBack;"), "'n'");
    EXPECT_EQ(context->execute("str popBack;"), "'k'");
    EXPECT_EQ(context->execute("str get;"), "\"\"");
    EXPECT_EQ(context->execute("str popBack;"), "null");
}

TEST_F(ContextTest, String_popFront) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knu\";"), "null");
    EXPECT_EQ(context->execute("str popFront;"), "'k'");
    EXPECT_EQ(context->execute("str popFront;"), "'n'");
    EXPECT_EQ(context->execute("str popFront;"), "'u'");
    EXPECT_EQ(context->execute("str get;"), "\"\"");
    EXPECT_EQ(context->execute("str popFront;"), "null");
}

TEST_F(ContextTest, String_popAtIndex) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str popAtIndex: 0;"), "'k'");
    EXPECT_EQ(context->execute("str popAtIndex: 6;"), "'b'");
    EXPECT_EQ(context->execute("str get;"), "\"nuckleall\"");

    // invalid index:
    EXPECT_EQ(context->execute("str popAtIndex: -1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("str popAtIndex: 9;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, String_concatenate) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckle\";"), "null");
    EXPECT_EQ(context->execute("str concatenate: \"ball\";"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"knuckleball\"");
}

TEST_F(ContextTest, String_insert_beforeIndex) {
    EXPECT_EQ(context->execute("String create: str withValue: \"bal\";"), "null");
    EXPECT_EQ(context->execute("str insert: \"l\" beforeIndex: 3;"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"ball\"");
    EXPECT_EQ(context->execute("str insert: \"knuckle\" beforeIndex: 0;"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"knuckleball\"");
    EXPECT_EQ(context->execute("str insert: \" \" beforeIndex: 7;"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"knuckle ball\"");

    // invalid index:
    EXPECT_EQ(context->execute("str insert: \" \" beforeIndex: -1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("str insert: \" \" beforeIndex: 13;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, String_eraseFromIndex) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str eraseFromIndex: 7;"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"knuckle\"");

    // invalid index:
    EXPECT_EQ(context->execute("str eraseFromIndex: 8;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("str eraseFromIndex: -1;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, String_eraseFromIndex_toIndex) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckle ball\";"), "null");
    EXPECT_EQ(context->execute("str eraseFromIndex: 7 toIndex: 8;"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"knuckleball\"");

    // invalid index:
    EXPECT_EQ(context->execute("str eraseFromIndex: 4 toIndex: 2;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("str eraseFromIndex: 4 toIndex: 12;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("str eraseFromIndex: -1 toIndex: 0;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, String_clear) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str clear;"), "null");
    EXPECT_EQ(context->execute("str get;"), "\"\"");
}

//////////////////////////////////////////////////////// Vector ////////////////////////////////////////////////////////

TEST_F(ContextTest, Vector_create) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[]");
}

TEST_F(ContextTest, Vector_createIfNotExists) {
    EXPECT_EQ(context->execute("Vector<Integer> createIfNotExists: prices;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[]");
    EXPECT_EQ(context->execute("Vector<Integer> createIfNotExists: prices;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[]");
}

TEST_F(ContextTest, Vector_get) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[]");
}

TEST_F(ContextTest, Vector_isEmpty) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices isEmpty?;"), "true");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices isEmpty?;"), "false");
}

TEST_F(ContextTest, Vector_contains) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices contains? 42;"), "false");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices contains? 42;"), "true");
}

TEST_F(ContextTest, Vector_contains_fromIndex) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices contains? 42 fromIndex: 0;"), "false");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices contains? 42 fromIndex: 0;"), "true");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices contains? 42 fromIndex: 1;"), "false");

    // invalid index:
    EXPECT_EQ(context->execute("prices contains? 42 fromIndex: 3;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices contains? 42 fromIndex: -1;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_count) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices count: 63;"), "0");
    EXPECT_EQ(context->execute("prices count: 21;"), "1");
    EXPECT_EQ(context->execute("prices count: 42;"), "2");
}

TEST_F(ContextTest, Vector_atIndex) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 63;"), "null");
    EXPECT_EQ(context->execute("prices atIndex: 0;"), "21");
    EXPECT_EQ(context->execute("prices atIndex: 1;"), "42");
    EXPECT_EQ(context->execute("prices atIndex: 2;"), "63");

    // invalid index:
    EXPECT_EQ(context->execute("prices atIndex: -1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices atIndex: 42;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_firstIndexOf) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices firstIndexOf: 42;"), "0");
    EXPECT_EQ(context->execute("prices firstIndexOf: 21;"), "1");
    EXPECT_EQ(context->execute("prices firstIndexOf: 63;"), "-1");
}

TEST_F(ContextTest, Vector_firstIndexOf_fromIndex) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices firstIndexOf: 42 fromIndex: 0;"), "0");
    EXPECT_EQ(context->execute("prices firstIndexOf: 42 fromIndex: 1;"), "2");
    EXPECT_EQ(context->execute("prices firstIndexOf: 42 fromIndex: 3;"), "-1");
    EXPECT_EQ(context->execute("prices firstIndexOf: 63 fromIndex: 0;"), "-1");

    // invalid index:
    EXPECT_EQ(context->execute("prices firstIndexOf: 42 fromIndex: -1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices firstIndexOf: 42 fromIndex: 42;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_allIndexesOf) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices allIndexesOf: 63;"), "[]");
    EXPECT_EQ(context->execute("prices allIndexesOf: 21;"), "[1]");
    EXPECT_EQ(context->execute("prices allIndexesOf: 42;"), "[0,2]");
}

TEST_F(ContextTest, Vector_size) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices size;"), "0");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices size;"), "3");
}

TEST_F(ContextTest, Vector_first) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 63;"), "null");
    EXPECT_EQ(context->execute("prices first: 0;"), "[]");
    EXPECT_EQ(context->execute("prices first: 1;"), "[42]");
    EXPECT_EQ(context->execute("prices first: 2;"), "[42,21]");
    EXPECT_EQ(context->execute("prices first: 3;"), "[42,21,63]");
    EXPECT_EQ(context->execute("prices first: 4;"), "[42,21,63]");

    // invalid n:
    EXPECT_EQ(context->execute("prices first: -1;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_last) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 63;"), "null");
    EXPECT_EQ(context->execute("prices last: 0;"), "[]");
    EXPECT_EQ(context->execute("prices last: 1;"), "[63]");
    EXPECT_EQ(context->execute("prices last: 2;"), "[21,63]");
    EXPECT_EQ(context->execute("prices last: 3;"), "[42,21,63]");
    EXPECT_EQ(context->execute("prices last: 4;"), "[42,21,63]");

    // invalid n:
    EXPECT_EQ(context->execute("prices last: -1;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_sliceFromIndex) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 1;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 2;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 3;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 4;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 5;"), "null");
    EXPECT_EQ(context->execute("prices sliceFromIndex: 0;"), "[1,2,3,4,5]");
    EXPECT_EQ(context->execute("prices sliceFromIndex: 1;"), "[2,3,4,5]");
    EXPECT_EQ(context->execute("prices sliceFromIndex: 4;"), "[5]");
    EXPECT_EQ(context->execute("prices sliceFromIndex: 5;"), "[]");

    // invalid index:
    EXPECT_EQ(context->execute("prices sliceFromIndex: -1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices sliceFromIndex: 42;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_sliceFromIndex_toIndex) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 1;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 2;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 3;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 4;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 5;"), "null");
    EXPECT_EQ(context->execute("prices sliceFromIndex: 0 toIndex: 0;"), "[]");
    EXPECT_EQ(context->execute("prices sliceFromIndex: 0 toIndex: 1;"), "[1]");
    EXPECT_EQ(context->execute("prices sliceFromIndex: 0 toIndex: 5;"), "[1,2,3,4,5]");
    EXPECT_EQ(context->execute("prices sliceFromIndex: 1 toIndex: 3;"), "[2,3]");

    // invalid index:
    EXPECT_EQ(context->execute("prices sliceFromIndex: 4 toIndex: 2;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices sliceFromIndex: -1 toIndex: 3;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices sliceFromIndex: 0 toIndex: 42;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_pushBack) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[42]");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[42,21]");
}

TEST_F(ContextTest, Vector_pushFront) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushFront: 42;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[42]");
    EXPECT_EQ(context->execute("prices pushFront: 21;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[21,42]");
}

TEST_F(ContextTest, Vector_popBack) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 63;"), "null");
    EXPECT_EQ(context->execute("prices popBack;"), "63");
    EXPECT_EQ(context->execute("prices popBack;"), "42");
    EXPECT_EQ(context->execute("prices popBack;"), "21");
    EXPECT_EQ(context->execute("prices popBack;"), "null");
}

TEST_F(ContextTest, Vector_popFront) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 63;"), "null");
    EXPECT_EQ(context->execute("prices popFront;"), "21");
    EXPECT_EQ(context->execute("prices popFront;"), "42");
    EXPECT_EQ(context->execute("prices popFront;"), "63");
    EXPECT_EQ(context->execute("prices popFront;"), "null");
}

TEST_F(ContextTest, Vector_popAtIndex) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 1;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 2;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 3;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 4;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 5;"), "null");
    EXPECT_EQ(context->execute("prices popAtIndex: 0;"), "1");
    EXPECT_EQ(context->execute("prices popAtIndex: 3;"), "5");
    EXPECT_EQ(context->execute("prices get;"), "[2,3,4]");

    // invalid index:
    EXPECT_EQ(context->execute("prices popAtIndex: -1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices popAtIndex: 3;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_insert_beforeIndex) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices insert: 42 beforeIndex: 0;"), "null");
    EXPECT_EQ(context->execute("prices insert: 21 beforeIndex: 0;"), "null");
    EXPECT_EQ(context->execute("prices insert: 63 beforeIndex: 2;"), "null");
    EXPECT_EQ(context->execute("prices insert: 22 beforeIndex: 1;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[21,22,42,63]");

    // invalid index:
    EXPECT_EQ(context->execute("prices insert: 42 beforeIndex: -1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices insert: 42 beforeIndex: 42;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_eraseFromIndex) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 1;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 2;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 3;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 4;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 5;"), "null");
    EXPECT_EQ(context->execute("prices eraseFromIndex: 3;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[1,2,3]");
    EXPECT_EQ(context->execute("prices eraseFromIndex: 0;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[]");

    // invalid index:
    EXPECT_EQ(context->execute("prices eraseFromIndex: -1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices eraseFromIndex: 42;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_eraseFromIndex_toIndex) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 1;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 2;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 3;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 4;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 5;"), "null");
    EXPECT_EQ(context->execute("prices eraseFromIndex: 0 toIndex: 3;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[4,5]");
    EXPECT_EQ(context->execute("prices eraseFromIndex: 1 toIndex: 2;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[4]");

    // invalid index:
    EXPECT_EQ(context->execute("prices eraseFromIndex: 1 toIndex: 0;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices eraseFromIndex: -1 toIndex: 1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices eraseFromIndex: 0 toIndex: 42;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_removeFirst) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices removeFirst: 42;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[21,21,42]");
}

TEST_F(ContextTest, Vector_removeFirstFromIndex) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices removeFirst: 21 fromIndex: 1;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[21,42,42]");

    // invalid index:
    EXPECT_EQ(context->execute("prices removeFirst: 42 fromIndex: -1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices removeFirst: 42 fromIndex: 42;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_removeAll) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices removeAll: 42;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[21,21]");
}

TEST_F(ContextTest, Vector_replaceFirst_with) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices replaceFirst: 42 with: -42;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[21,-42,21,42]");
}

TEST_F(ContextTest, Vector_replaceFirst_fromIndex_with) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices replaceFirst: 42 fromIndex: 2 with: -42;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[21,42,21,-42]");

    // invalid index:
    EXPECT_EQ(context->execute("prices replaceFirst: 42 fromIndex: -1 with: -42;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices replaceFirst: 42 fromIndex: 42 with: -42;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_replaceAll_with) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices replaceAll: 42 with: -42;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[21,-42,21,-42]");
}

TEST_F(ContextTest, Vector_sort) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 3;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 2;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 5;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 1;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 4;"), "null");
    EXPECT_EQ(context->execute("prices sort;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[1,2,3,4,5]");
}

TEST_F(ContextTest, Vector_reverse) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 63;"), "null");
    EXPECT_EQ(context->execute("prices reverse;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[63,42,21]");
}

TEST_F(ContextTest, Vector_clear) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 21;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 42;"), "null");
    EXPECT_EQ(context->execute("prices pushBack: 63;"), "null");
    EXPECT_EQ(context->execute("prices clear;"), "null");
    EXPECT_EQ(context->execute("prices get;"), "[]");
}

///////////////////////////////////////////////////////// Set //////////////////////////////////////////////////////////

TEST_F(ContextTest, Set_create) {
    EXPECT_EQ(context->execute("Set<String> create: ids;"), "null");
    EXPECT_EQ(context->execute("ids get;"), "{}");
}

TEST_F(ContextTest, Set_createIfNotExists) {
    EXPECT_EQ(context->execute("Set<String> createIfNotExists: ids;"), "null");
    EXPECT_EQ(context->execute("ids get;"), "{}");
    EXPECT_EQ(context->execute("Set<String> createIfNotExists: ids;"), "null");
    EXPECT_EQ(context->execute("ids get;"), "{}");
}

TEST_F(ContextTest, Set_get) {
    EXPECT_EQ(context->execute("Set<String> create: ids;"), "null");
    EXPECT_EQ(context->execute("ids get;"), "{}");
}

TEST_F(ContextTest, Set_isEmpty) {
    EXPECT_EQ(context->execute("Set<String> create: ids;"), "null");
    EXPECT_EQ(context->execute("ids isEmpty?;"), "true");
    EXPECT_EQ(context->execute("ids add: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("ids isEmpty?;"), "false");
}

TEST_F(ContextTest, Set_contains) {
    EXPECT_EQ(context->execute("Set<String> create: ids;"), "null");
    EXPECT_EQ(context->execute("ids contains? \"knuckleball\";"), "false");
    EXPECT_EQ(context->execute("ids add: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("ids contains? \"knuckleball\";"), "true");
}

TEST_F(ContextTest, Set_size) {
    EXPECT_EQ(context->execute("Set<String> create: ids;"), "null");
    EXPECT_EQ(context->execute("ids size;"), "0");
    EXPECT_EQ(context->execute("ids add: \"1\";"), "null");
    EXPECT_EQ(context->execute("ids add: \"2\";"), "null");
    EXPECT_EQ(context->execute("ids add: \"3\";"), "null");
    EXPECT_EQ(context->execute("ids size;"), "3");
    EXPECT_EQ(context->execute("ids add: \"1\";"), "null");
    EXPECT_EQ(context->execute("ids size;"), "3");
}

TEST_F(ContextTest, Set_add) {
    EXPECT_EQ(context->execute("Set<String> create: ids;"), "null");
    EXPECT_EQ(context->execute("ids add: \"knuckle\";"), "null");
    EXPECT_EQ(context->execute("ids get;"), "{\"knuckle\"}");
    EXPECT_EQ(context->execute("ids add: \"ball\";"), "null");
    EXPECT_EQ(context->execute("ids get;"), "{\"ball\",\"knuckle\"}");
}

TEST_F(ContextTest, Set_remove) {
    EXPECT_EQ(context->execute("Set<String> create: ids;"), "null");
    EXPECT_EQ(context->execute("ids add: \"knuckle\";"), "null");
    EXPECT_EQ(context->execute("ids add: \"ball\";"), "null");
    EXPECT_EQ(context->execute("ids remove: \"knuckle\";"), "null");
    EXPECT_EQ(context->execute("ids get;"), "{\"ball\"}");
}

TEST_F(ContextTest, Set_clear) {
    EXPECT_EQ(context->execute("Set<String> create: ids;"), "null");
    EXPECT_EQ(context->execute("ids add: \"1\";"), "null");
    EXPECT_EQ(context->execute("ids add: \"2\";"), "null");
    EXPECT_EQ(context->execute("ids add: \"3\";"), "null");
    EXPECT_EQ(context->execute("ids clear;"), "null");
    EXPECT_EQ(context->execute("ids get;"), "{}");
}

////////////////////////////////////////////////////// Dictionary //////////////////////////////////////////////////////

TEST_F(ContextTest, Dictionary_create) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("ages get;"), "()");
}

TEST_F(ContextTest, Dictionary_createIfNotExists) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> createIfNotExists: ages;"), "null");
    EXPECT_EQ(context->execute("ages get;"), "()");
    EXPECT_EQ(context->execute("Dictionary<String, Integer> createIfNotExists: ages;"), "null");
    EXPECT_EQ(context->execute("ages get;"), "()");
}

TEST_F(ContextTest, Dictionary_get) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("ages get;"), "()");
}

TEST_F(ContextTest, Dictionary_isEmpty) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("ages isEmpty?;"), "true");
    EXPECT_EQ(context->execute("ages associateValue: 42 withKey: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("ages isEmpty?;"), "false");
}

TEST_F(ContextTest, Dictionary_containsKey) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("ages containsKey? \"knuckleball\";"), "false");
    EXPECT_EQ(context->execute("ages associateValue: 42 withKey: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("ages containsKey? \"knuckleball\";"), "true");
}

TEST_F(ContextTest, Dictionary_size) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("ages size;"), "0");
    EXPECT_EQ(context->execute("ages associateValue: 21 withKey: \"knuckle\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 42 withKey: \"ball\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 63 withKey: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 84 withKey: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("ages size;"), "3");
}

TEST_F(ContextTest, Dictionary_keys) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 21 withKey: \"knuckle\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 42 withKey: \"ball\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 63 withKey: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("ages keys;"), "[\"ball\",\"knuckle\",\"knuckleball\"]");
}

TEST_F(ContextTest, Dictionary_values) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 21 withKey: \"knuckle\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 42 withKey: \"ball\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 42 withKey: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("ages values;"), "[42,21,42]");
}

TEST_F(ContextTest, Dictionary_getValueForKey) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 21 withKey: \"knuckle\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 42 withKey: \"ball\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 42 withKey: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("ages getValueForKey: \"knuckle\";"), "21");
    EXPECT_EQ(context->execute("ages getValueForKey: \"ball\";"), "42");
    EXPECT_EQ(context->execute("ages getValueForKey: \"knuckleball\";"), "42");
    EXPECT_EQ(context->execute("ages getValueForKey: \"\";"), "null");
}

TEST_F(ContextTest, Dictionary_associateValue_withKey) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 42 withKey: \"knuckle\";"), "null");
    EXPECT_EQ(context->execute("ages get;"), "((\"knuckle\",42))");
    EXPECT_EQ(context->execute("ages associateValue: 21 withKey: \"ball\";"), "null");
    EXPECT_EQ(context->execute("ages get;"), "((\"ball\",21),(\"knuckle\",42))");
}

TEST_F(ContextTest, Dictionary_removeKey) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 21 withKey: \"knuckle\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 42 withKey: \"ball\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 42 withKey: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("ages removeKey: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("ages removeKey: \"42\";"), "null");
    EXPECT_EQ(context->execute("ages get;"), "((\"ball\",42),(\"knuckle\",21))");
}

TEST_F(ContextTest, Dictionary_clear) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 21 withKey: \"knuckle\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 42 withKey: \"ball\";"), "null");
    EXPECT_EQ(context->execute("ages associateValue: 63 withKey: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("ages clear;"), "null");
    EXPECT_EQ(context->execute("ages get;"), "()");
}

///////////////////////////////////////////////////// Syntax errors ////////////////////////////////////////////////////

TEST_F(ContextTest, SyntaxError_for_invalid_object) {
    EXPECT_EQ(context->execute("_prices get;"), EXC_INVALID_STATEMENT);
    EXPECT_EQ(context->execute("2016prices get;"), EXC_INVALID_STATEMENT);
    EXPECT_EQ(context->execute("42 get;"), EXC_INVALID_STATEMENT);
    EXPECT_EQ(context->execute("Vector create: l;"), EXC_INVALID_STATEMENT);
}

TEST_F(ContextTest, SyntaxError_for_invalid_message_name) {
    EXPECT_EQ(context->execute("Boolean create t withValue true;"), EXC_INVALID_STATEMENT);
    EXPECT_EQ(context->execute("t set false;"), EXC_INVALID_STATEMENT);
}

TEST_F(ContextTest, SyntaxError_for_invalid_argument) {
    EXPECT_EQ(context->execute("Boolean create: _t withValue: true;"), EXC_INVALID_STATEMENT);
}

TEST_F(ContextTest, SyntaxError_for_multiple_arguments) {
    EXPECT_EQ(context->execute("Boolean create: t withValue: true false;"), EXC_INVALID_STATEMENT);
}

TEST_F(ContextTest, SyntaxError_for_missing_object) {
    EXPECT_EQ(context->execute("get;"), EXC_INVALID_STATEMENT);
    EXPECT_EQ(context->execute("create: t withValue: true;"), EXC_INVALID_STATEMENT);
}

TEST_F(ContextTest, SyntaxError_for_missing_message) {
    EXPECT_EQ(context->execute("t;"), EXC_INVALID_STATEMENT);
    EXPECT_EQ(context->execute("Boolean;"), EXC_INVALID_STATEMENT);
}

TEST_F(ContextTest, SyntaxError_for_missing_argument) {
    EXPECT_EQ(context->execute("t set: ;"), EXC_INVALID_STATEMENT);
    EXPECT_EQ(context->execute("Boolean create: t withValue: ;"), EXC_INVALID_STATEMENT);
    EXPECT_EQ(context->execute("Boolean create: withValue: true;"), EXC_INVALID_STATEMENT);
}

TEST_F(ContextTest, SyntaxError_for_missing_semicolon) {
    EXPECT_EQ(context->execute("t get"), EXC_INVALID_STATEMENT);
    EXPECT_EQ(context->execute("Boolean create: t withValue: true"), EXC_INVALID_STATEMENT);
}

//////////////////////////////////////////////////// Runtime errors ////////////////////////////////////////////////////

TEST_F(ContextTest, RuntimeError_for_unexistent_variable) {
    EXPECT_EQ(context->execute("t get;"), EXC_UNEXISTENT_VARIABLE);
    EXPECT_EQ(context->execute("i add: 1;"), EXC_UNEXISTENT_VARIABLE);
}

TEST_F(ContextTest, RuntimeError_for_invalid_message) {
    EXPECT_EQ(context->execute("Boolean create: t;"), EXC_INVALID_MESSAGE);
    EXPECT_EQ(context->execute("Character create: c;"), EXC_INVALID_MESSAGE);
    EXPECT_EQ(context->execute("Integer create: i;"), EXC_INVALID_MESSAGE);
    EXPECT_EQ(context->execute("Float create: f;"), EXC_INVALID_MESSAGE);
}

TEST_F(ContextTest, RuntimeError_for_wrong_number_of_arguments) {
    // Boolean
    EXPECT_EQ(context->execute("Boolean create: t withValue: true;"), "null");
    EXPECT_EQ(context->execute("t isTrue? 42;"), EXC_WRONG_NUMBER_OF_ARGUMENTS);
    EXPECT_EQ(context->execute("t isFalse? 42;"), EXC_WRONG_NUMBER_OF_ARGUMENTS);

    // Character
    EXPECT_EQ(context->execute("Character create: space withValue: ' ';"), "null");
    EXPECT_EQ(context->execute("space isAlphabetic? 42;"), EXC_WRONG_NUMBER_OF_ARGUMENTS);
    EXPECT_EQ(context->execute("space isNumeric? 42;"), EXC_WRONG_NUMBER_OF_ARGUMENTS);
    EXPECT_EQ(context->execute("space isAlphanumeric? 42;"), EXC_WRONG_NUMBER_OF_ARGUMENTS);
    EXPECT_EQ(context->execute("space isSpace? 42;"), EXC_WRONG_NUMBER_OF_ARGUMENTS);

    // String
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("str isEmpty? 42;"), EXC_WRONG_NUMBER_OF_ARGUMENTS);

    // Vector
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("prices isEmpty? 42;"), EXC_WRONG_NUMBER_OF_ARGUMENTS);

    // Set
    EXPECT_EQ(context->execute("Set<String> create: ids;"), "null");
    EXPECT_EQ(context->execute("ids isEmpty? 42;"), EXC_WRONG_NUMBER_OF_ARGUMENTS);

    // Dictionary
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("ages isEmpty? 42;"), EXC_WRONG_NUMBER_OF_ARGUMENTS);
}

TEST_F(ContextTest, RuntimeError_for_variable_name_already_used) {
    // Boolean
    EXPECT_EQ(context->execute("Boolean create: t withValue: true;"), "null");
    EXPECT_EQ(context->execute("Boolean create: t withValue: false;"), EXC_VARIABLE_NAME_ALREADY_USED);
    EXPECT_EQ(context->execute("t get;"), "true");

    // Character
    EXPECT_EQ(context->execute("Character create: numeric withValue: '0';"), "null");
    EXPECT_EQ(context->execute("Character create: numeric withValue: '1';"), EXC_VARIABLE_NAME_ALREADY_USED);
    EXPECT_EQ(context->execute("numeric get;"), "'0'");

    // Integer
    EXPECT_EQ(context->execute("Integer create: i withValue: 42;"), "null");
    EXPECT_EQ(context->execute("Integer create: i withValue: 43;"), EXC_VARIABLE_NAME_ALREADY_USED);
    EXPECT_EQ(context->execute("i get;"), "42");

    // Float
    EXPECT_EQ(context->execute("Float create: f withValue: 42.0;"), "null");
    EXPECT_EQ(context->execute("Float create: f withValue: 43.0;"), EXC_VARIABLE_NAME_ALREADY_USED);
    EXPECT_EQ(context->execute("f get;"), "42.000");

    // String
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");
    EXPECT_EQ(context->execute("String create: str;"), EXC_VARIABLE_NAME_ALREADY_USED);
    EXPECT_EQ(context->execute("String create: str withValue: \"\";"), EXC_VARIABLE_NAME_ALREADY_USED);
    EXPECT_EQ(context->execute("str get;"), "\"knuckleball\"");

    // Vector
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), EXC_VARIABLE_NAME_ALREADY_USED);

    // Set
    EXPECT_EQ(context->execute("Set<String> create: ids;"), "null");
    EXPECT_EQ(context->execute("Set<String> create: ids;"), EXC_VARIABLE_NAME_ALREADY_USED);

    // Dictionary
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), EXC_VARIABLE_NAME_ALREADY_USED);
}

TEST_F(ContextTest, Boolean_RuntimeError_for_invalid_argument) {
    EXPECT_EQ(context->execute("Boolean create: t withValue: true;"), "null");

    // create:withValue:
    EXPECT_EQ(context->execute("Boolean create: t withValue: 1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("Boolean create: 42 withValue: true;"), EXC_INVALID_ARGUMENT);

    // createIfNotExists:withValue:
    EXPECT_EQ(context->execute("Boolean createIfNotExists: t withValue: 1;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("Boolean createIfNotExists: 42 withValue: true;"), EXC_INVALID_ARGUMENT);

    // set:
    EXPECT_EQ(context->execute("t set: 0;"), EXC_INVALID_ARGUMENT);
}
    
TEST_F(ContextTest, Character_RuntimeError_for_invalid_argument) {
    EXPECT_EQ(context->execute("Character create: numeric withValue: '0';"), "null");

    // create:withValue:
    EXPECT_EQ(context->execute("Character create: numeric withValue: \"0\";"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("Character create: 42 withValue: '0';"), EXC_INVALID_ARGUMENT);
    
    // createIfNotExists:withValue:
    EXPECT_EQ(context->execute("Character createIfNotExists: numeric withValue: \"0\";"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("Character createIfNotExists: 42 withValue: '0';"), EXC_INVALID_ARGUMENT);
    
    // set:
    EXPECT_EQ(context->execute("numeric set: 1;"), EXC_INVALID_ARGUMENT);

    // isEqualTo?
    EXPECT_EQ(context->execute("numeric isEqualTo? 0;"), EXC_INVALID_ARGUMENT);
    
    // isLessThan?
    EXPECT_EQ(context->execute("numeric isLessThan? \"0\";"), EXC_INVALID_ARGUMENT);
    
    // isLessThanOrEqualTo?
    EXPECT_EQ(context->execute("numeric isLessThanOrEqualTo? \"0\";"), EXC_INVALID_ARGUMENT);
    
    // isGreaterThan?
    EXPECT_EQ(context->execute("numeric isGreaterThan? 0;"), EXC_INVALID_ARGUMENT);
    
    // isGreaterThanOrEqualTo?
    EXPECT_EQ(context->execute("numeric isGreaterThanOrEqualTo? 0;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Integer_RuntimeError_for_invalid_argument) {
    EXPECT_EQ(context->execute("Integer create: i withValue: 42;"), "null");

    // create:withValue:
    EXPECT_EQ(context->execute("Integer create: i withValue: \"42\";"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("Integer create: \"i\" withValue: 42;"), EXC_INVALID_ARGUMENT);
    
    // createIfNotExists:withValue:
    EXPECT_EQ(context->execute("Integer createIfNotExists: i withValue: \"42\";"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("Integer createIfNotExists: \"i\" withValue: 42;"), EXC_INVALID_ARGUMENT);
    
    // set:
    EXPECT_EQ(context->execute("i set: '0';"), EXC_INVALID_ARGUMENT);

    // isEqualTo?
    EXPECT_EQ(context->execute("i isEqualTo? \"42\";"), EXC_INVALID_ARGUMENT);
    
    // isLessThan?
    EXPECT_EQ(context->execute("i isLessThan? false;"), EXC_INVALID_ARGUMENT);
    
    // isLessThanOrEqualTo?
    EXPECT_EQ(context->execute("i isLessThanOrEqualTo? true;"), EXC_INVALID_ARGUMENT);
    
    // isGreaterThan?
    EXPECT_EQ(context->execute("i isGreaterThan? 'a';"), EXC_INVALID_ARGUMENT);
    
    // isGreaterThanOrEqualTo?
    EXPECT_EQ(context->execute("i isGreaterThanOrEqualTo? \"42\";"), EXC_INVALID_ARGUMENT);

    // add:
    EXPECT_EQ(context->execute("i add: true;"), EXC_INVALID_ARGUMENT);
    
    // subtract:
    EXPECT_EQ(context->execute("i subtract: \"42\";"), EXC_INVALID_ARGUMENT);

    // multiplyBy:
    EXPECT_EQ(context->execute("i multiplyBy: \"2.5\";"), EXC_INVALID_ARGUMENT);

    // divideBy:
    EXPECT_EQ(context->execute("i divideBy: '2';"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Float_RuntimeError_for_invalid_argument) {
    EXPECT_EQ(context->execute("Float create: f withValue: 42.0;"), "null");

    // create:withValue:
    EXPECT_EQ(context->execute("Float create: f withValue: \"42\";"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("Float create: \"f\" withValue: 42.0;"), EXC_INVALID_ARGUMENT);
    
    // createIfNotExists:withValue:
    EXPECT_EQ(context->execute("Float createIfNotExists: f withValue: \"42\";"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("Float createIfNotExists: \"f\" withValue: 42.0;"), EXC_INVALID_ARGUMENT);
    
    // set:
    EXPECT_EQ(context->execute("f set: '0';"), EXC_INVALID_ARGUMENT);

    // isEqualTo?
    EXPECT_EQ(context->execute("f isEqualTo? \"42\";"), EXC_INVALID_ARGUMENT);
    
    // isLessThan?
    EXPECT_EQ(context->execute("f isLessThan? '0';"), EXC_INVALID_ARGUMENT);
    
    // isLessThanOrEqualTo?
    EXPECT_EQ(context->execute("f isLessThanOrEqualTo? true;"), EXC_INVALID_ARGUMENT);
    
    // isGreaterThan?
    EXPECT_EQ(context->execute("f isGreaterThan? \"0\";"), EXC_INVALID_ARGUMENT);
    
    // isGreaterThanOrEqualTo?
    EXPECT_EQ(context->execute("f isGreaterThanOrEqualTo? false;"), EXC_INVALID_ARGUMENT);

    // add:
    EXPECT_EQ(context->execute("f add: \"knuckleball\";"), EXC_INVALID_ARGUMENT);
    
    // subtract:
    EXPECT_EQ(context->execute("f subtract: i;"), EXC_INVALID_ARGUMENT);

    // multiplyBy:
    EXPECT_EQ(context->execute("f multiplyBy: true;"), EXC_INVALID_ARGUMENT);

    // divideBy:
    EXPECT_EQ(context->execute("f divideBy: false;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, String_RuntimeError_for_invalid_argument) {
    EXPECT_EQ(context->execute("String create: str withValue: \"knuckleball\";"), "null");

    // create:
    EXPECT_EQ(context->execute("String create: 42;"), EXC_INVALID_ARGUMENT);
    
    // createIfNotExists:
    EXPECT_EQ(context->execute("String createIfNotExists: 42;"), EXC_INVALID_ARGUMENT);

    // create:withValue:
    EXPECT_EQ(context->execute("String create: str withValue: 42;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("String create: 42 withValue: \"knuckleball\";"), EXC_INVALID_ARGUMENT);
    
    // createIfNotExists:withValue:
    EXPECT_EQ(context->execute("String createIfNotExists: str withValue: 42;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("String createIfNotExists: 42 withValue: \"knuckleball\";"), EXC_INVALID_ARGUMENT);

    // set:
    EXPECT_EQ(context->execute("str set: 42;"), EXC_INVALID_ARGUMENT);

    // startsWith?
    EXPECT_EQ(context->execute("str startsWith? 42;"), EXC_INVALID_ARGUMENT);

    // endsWith?
    EXPECT_EQ(context->execute("str endsWith? 42;"), EXC_INVALID_ARGUMENT);

    // isLexicographicallyEqualTo?
    EXPECT_EQ(context->execute("str isLexicographicallyEqualTo? 42;"), EXC_INVALID_ARGUMENT);

    // isLexicographicallyLessThan?
    EXPECT_EQ(context->execute("str isLexicographicallyLessThan? 42;"), EXC_INVALID_ARGUMENT);

    // isLexicographicallyLessThanOrEqualTo?
    EXPECT_EQ(context->execute("str isLexicographicallyLessThanOrEqualTo? 42;"), EXC_INVALID_ARGUMENT);

    // isLexicographicallyGreaterThan?
    EXPECT_EQ(context->execute("str isLexicographicallyGreaterThan? 42;"), EXC_INVALID_ARGUMENT);

    // isLexicographicallyGreaterThanOrEqualTo?
    EXPECT_EQ(context->execute("str isLexicographicallyGreaterThanOrEqualTo? 42;"), EXC_INVALID_ARGUMENT);

    // atIndex:
    EXPECT_EQ(context->execute("str atIndex: true;"), EXC_INVALID_ARGUMENT);

    // first:
    EXPECT_EQ(context->execute("str first: \"42\";"), EXC_INVALID_ARGUMENT);

    // last:
    EXPECT_EQ(context->execute("str last: \"42\";"), EXC_INVALID_ARGUMENT);

    // substringFromIndex:
    EXPECT_EQ(context->execute("str substringFromIndex: 'a';"), EXC_INVALID_ARGUMENT);

    // substringFromIndex:toIndex:
    EXPECT_EQ(context->execute("str substringFromIndex: true toIndex: 43;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("str substringFromIndex: 42 toIndex: false;"), EXC_INVALID_ARGUMENT);

    // pushBack:
    EXPECT_EQ(context->execute("str pushBack: 42;"), EXC_INVALID_ARGUMENT);

    // pushFront:
    EXPECT_EQ(context->execute("str pushFront: 42;"), EXC_INVALID_ARGUMENT);

    // concatenate:
    EXPECT_EQ(context->execute("str concatenate: 42;"), EXC_INVALID_ARGUMENT);

    // insert:beforeIndex:
    EXPECT_EQ(context->execute("str insert: 42 beforeIndex: 0;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("str insert: \"knuckleball\" beforeIndex: '4';"), EXC_INVALID_ARGUMENT);

    // popAtIndex:
    EXPECT_EQ(context->execute("str popAtIndex: \"0\";"), EXC_INVALID_ARGUMENT);

    // eraseFromIndex:
    EXPECT_EQ(context->execute("str eraseFromIndex: \"42\";"), EXC_INVALID_ARGUMENT);

    // eraseFromIndex:toIndex:
    EXPECT_EQ(context->execute("str eraseFromIndex: \"42\" toIndex: 43;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("str eraseFromIndex: 42 toIndex: \"43\";"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Vector_RuntimeError_for_invalid_argument) {
    EXPECT_EQ(context->execute("Vector<Integer> create: prices;"), "null");

    // create:
    EXPECT_EQ(context->execute("Vector<Integer> create: 42;"), EXC_INVALID_ARGUMENT);
    
    // createIfNotExists:
    EXPECT_EQ(context->execute("Vector<Integer> createIfNotExists: 42;"), EXC_INVALID_ARGUMENT);

    // contains?
    EXPECT_EQ(context->execute("prices contains? true;"), EXC_INVALID_ARGUMENT);

    // contains?fromIndex:
    EXPECT_EQ(context->execute("prices contains? true fromIndex: 0;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices contains? 42 fromIndex: \"0\";"), EXC_INVALID_ARGUMENT);

    // count:
    EXPECT_EQ(context->execute("prices count: true;"), EXC_INVALID_ARGUMENT);

    // atIndex:
    EXPECT_EQ(context->execute("prices atIndex: \"42\";"), EXC_INVALID_ARGUMENT);

    // firstIndexOf:
    EXPECT_EQ(context->execute("prices firstIndexOf: \"42\";"), EXC_INVALID_ARGUMENT);

    // firstIndexOf:fromIndex:
    EXPECT_EQ(context->execute("prices firstIndexOf: \"42\" fromIndex: 0;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices firstIndexOf: 42 fromIndex: true;"), EXC_INVALID_ARGUMENT);

    // allIndexesOf:
    EXPECT_EQ(context->execute("prices allIndexesOf: \"42\";"), EXC_INVALID_ARGUMENT);
    
    // first:
    EXPECT_EQ(context->execute("prices first: \"42\";"), EXC_INVALID_ARGUMENT);

    // last:
    EXPECT_EQ(context->execute("prices last: \"42\";"), EXC_INVALID_ARGUMENT);

    // sliceFromIndex:
    EXPECT_EQ(context->execute("prices sliceFromIndex: \"42\";"), EXC_INVALID_ARGUMENT);

    // sliceFromIndex:toIndex:
    EXPECT_EQ(context->execute("prices sliceFromIndex: \"42\" toIndex: 42;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices sliceFromIndex: 42 toIndex: \"42\";"), EXC_INVALID_ARGUMENT);

    // pushBack:
    EXPECT_EQ(context->execute("prices pushBack: true;"), EXC_INVALID_ARGUMENT);

    // pushFront:
    EXPECT_EQ(context->execute("prices pushFront: true;"), EXC_INVALID_ARGUMENT);

    // insert:beforeIndex:
    EXPECT_EQ(context->execute("prices insert: \"42\" beforeIndex: 0;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices insert: 42 beforeIndex: \"0\";"), EXC_INVALID_ARGUMENT);

    // popAtIndex:
    EXPECT_EQ(context->execute("prices popAtIndex: \"0\";"), EXC_INVALID_ARGUMENT);
    
    // erasefromIndex:
    EXPECT_EQ(context->execute("prices eraseFromIndex: \"42\";"), EXC_INVALID_ARGUMENT);

    // erasefromIndex:toIndex:
    EXPECT_EQ(context->execute("prices eraseFromIndex: 0 toIndex: \"42\";"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices eraseFromIndex: \"0\" toIndex: 42;"), EXC_INVALID_ARGUMENT);

    // removeFirst:
    EXPECT_EQ(context->execute("prices removeFirst: \"42\";"), EXC_INVALID_ARGUMENT);

    // removeFirst:fromIndex:
    EXPECT_EQ(context->execute("prices removeFirst: \"42\" fromIndex: 0;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices removeFirst: 42 fromIndex: \"0\";"), EXC_INVALID_ARGUMENT);

    // removeAll:
    EXPECT_EQ(context->execute("prices removeAll: \"42\";"), EXC_INVALID_ARGUMENT);

    // replaceFirst:with:
    EXPECT_EQ(context->execute("prices replaceFirst: \"42\" with: 42;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices replaceFirst: 42 with: \"42\";"), EXC_INVALID_ARGUMENT);
    
    // replaceFirst:fromIndex:with:
    EXPECT_EQ(context->execute("prices replaceFirst: \"42\" fromIndex: 0 with: 42;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices replaceFirst: 42 fromIndex: \"0\" with: 42;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices replaceFirst: 42 fromIndex: 0 with: \"42\";"), EXC_INVALID_ARGUMENT);

    // replaceAll:with:
    EXPECT_EQ(context->execute("prices replaceAll: \"42\" with: 42;"), EXC_INVALID_ARGUMENT);
    EXPECT_EQ(context->execute("prices replaceAll: 42 with: \"42\";"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Set_RuntimeError_for_invalid_argument) {
    EXPECT_EQ(context->execute("Set<String> create: ids;"), "null");

    // create:
    EXPECT_EQ(context->execute("Set<String> create: 42;"), EXC_INVALID_ARGUMENT);
    
    // createIfNotExists:
    EXPECT_EQ(context->execute("Set<String> createIfNotExists: 42;"), EXC_INVALID_ARGUMENT);

    // contains?
    EXPECT_EQ(context->execute("ids contains? true;"), EXC_INVALID_ARGUMENT);

    // add:
    EXPECT_EQ(context->execute("ids add: 42;"), EXC_INVALID_ARGUMENT);

    // remove:
    EXPECT_EQ(context->execute("ids remove: 42;"), EXC_INVALID_ARGUMENT);
}

TEST_F(ContextTest, Dictionary_RuntimeError_for_invalid_argument) {
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: ages;"), "null");

    // create:
    EXPECT_EQ(context->execute("Dictionary<String, Integer> create: 42;"), EXC_INVALID_ARGUMENT);
    
    // createIfNotExists:
    EXPECT_EQ(context->execute("Dictionary<String, Integer> createIfNotExists: 42;"), EXC_INVALID_ARGUMENT);

    // containsKey?
    EXPECT_EQ(context->execute("ages containsKey? true;"), EXC_INVALID_ARGUMENT);

    // getValueForKey:
    EXPECT_EQ(context->execute("ages getValueForKey: 42;"), EXC_INVALID_ARGUMENT);

    // associateValue:withKey:
    EXPECT_EQ(context->execute("ages associateValue: \"knuckleball\" withKey: 42;"), EXC_INVALID_ARGUMENT);

    // removeKey:
    EXPECT_EQ(context->execute("ages removeKey: 42;"), EXC_INVALID_ARGUMENT);
}

///////////////////////////////////////////////////// Corner cases /////////////////////////////////////////////////////

TEST_F(ContextTest, corner_cases_with_white_spaces) {
    // few spaces
    EXPECT_EQ(context->execute("Boolean create:t withValue:true;"), "null");
    EXPECT_EQ(context->execute("t get;"), "true");

    // lots of spaces
    EXPECT_EQ(context->execute("  Integer  create:  i  withValue:  42  ;  "), "null");
    EXPECT_EQ(context->execute("i get;"), "42");
}
