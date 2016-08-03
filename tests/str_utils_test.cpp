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

#include "str_utils.h"

TEST(str_utils, ltrim) {
    EXPECT_EQ(str_utils::ltrim(""), "");
    EXPECT_EQ(str_utils::ltrim(" "), "");
    EXPECT_EQ(str_utils::ltrim("  "), "");
    EXPECT_EQ(str_utils::ltrim("Integer"), "Integer");
    EXPECT_EQ(str_utils::ltrim(" Integer"), "Integer");
    EXPECT_EQ(str_utils::ltrim("Integer "), "Integer ");
    EXPECT_EQ(str_utils::ltrim(" Integer "), "Integer ");
}

TEST(str_utils, rtrim) {
    EXPECT_EQ(str_utils::rtrim(""), "");
    EXPECT_EQ(str_utils::rtrim(" "), "");
    EXPECT_EQ(str_utils::rtrim("  "), "");
    EXPECT_EQ(str_utils::rtrim("Integer"), "Integer");
    EXPECT_EQ(str_utils::rtrim(" Integer"), " Integer");
    EXPECT_EQ(str_utils::rtrim("Integer "), "Integer");
    EXPECT_EQ(str_utils::rtrim(" Integer "), " Integer");
}

TEST(str_utils, trim) {
    EXPECT_EQ(str_utils::trim(""), "");
    EXPECT_EQ(str_utils::trim(" "), "");
    EXPECT_EQ(str_utils::trim("  "), "");
    EXPECT_EQ(str_utils::trim("Integer"), "Integer");
    EXPECT_EQ(str_utils::trim(" Integer"), "Integer");
    EXPECT_EQ(str_utils::trim("Integer "), "Integer");
    EXPECT_EQ(str_utils::trim(" Integer "), "Integer");
}

TEST(str_utils, escape) {
    EXPECT_EQ(str_utils::escape("", 'a'), "");
    EXPECT_EQ(str_utils::escape("knuckleball", 'l'), "knuck\\leba\\l\\l");
    EXPECT_EQ(str_utils::escape("\\knuckleball", 'l'), "\\knuck\\leba\\l\\l");
}

TEST(str_utils, unescape) {
    EXPECT_EQ(str_utils::unescape("", 'a'), "");
    EXPECT_EQ(str_utils::unescape("\\knuck\\leba\\l\\l", 'l'), "\\knuckleball");
    EXPECT_EQ(str_utils::unescape("\\knuck\\leba\\l\\l", 'k'), "knuck\\leba\\l\\l");
}

TEST(str_utils, is_space) {
    EXPECT_TRUE(str_utils::is_space(' '));
    EXPECT_TRUE(str_utils::is_space('\n'));
    EXPECT_TRUE(str_utils::is_space('\t'));
}

TEST(str_utils, is_not_space) {
    EXPECT_FALSE(str_utils::is_space('0'));
    EXPECT_FALSE(str_utils::is_space('9'));
    EXPECT_FALSE(str_utils::is_space('a'));
    EXPECT_FALSE(str_utils::is_space('z'));
    EXPECT_FALSE(str_utils::is_space('A'));
    EXPECT_FALSE(str_utils::is_space('Z'));
    EXPECT_FALSE(str_utils::is_space(','));
}

TEST(str_utils, is_alpha) {
    EXPECT_TRUE(str_utils::is_alpha('a'));
    EXPECT_TRUE(str_utils::is_alpha('z'));
    EXPECT_TRUE(str_utils::is_alpha('A'));
    EXPECT_TRUE(str_utils::is_alpha('Z'));
}

TEST(str_utils, is_not_alpha) {
    EXPECT_FALSE(str_utils::is_alpha(' '));
    EXPECT_FALSE(str_utils::is_alpha('0'));
    EXPECT_FALSE(str_utils::is_alpha('9'));
    EXPECT_FALSE(str_utils::is_alpha(','));
}

TEST(str_utils, is_numeric) {
    EXPECT_TRUE(str_utils::is_numeric('0'));
    EXPECT_TRUE(str_utils::is_numeric('9'));
}

TEST(str_utils, is_not_numeric) {
    EXPECT_FALSE(str_utils::is_numeric(' '));
    EXPECT_FALSE(str_utils::is_numeric('a'));
    EXPECT_FALSE(str_utils::is_numeric('z'));
    EXPECT_FALSE(str_utils::is_numeric('A'));
    EXPECT_FALSE(str_utils::is_numeric('Z'));
    EXPECT_FALSE(str_utils::is_numeric(','));
}

TEST(str_utils, is_alphanumeric) {
    EXPECT_TRUE(str_utils::is_alphanumeric('a'));
    EXPECT_TRUE(str_utils::is_alphanumeric('z'));
    EXPECT_TRUE(str_utils::is_alphanumeric('A'));
    EXPECT_TRUE(str_utils::is_alphanumeric('Z'));
    EXPECT_TRUE(str_utils::is_alphanumeric('0'));
    EXPECT_TRUE(str_utils::is_alphanumeric('9'));
}

TEST(str_utils, is_not_alphanumeric) {
    EXPECT_FALSE(str_utils::is_alphanumeric(' '));
    EXPECT_FALSE(str_utils::is_alphanumeric(','));
}

TEST(str_utils, starts_with) {
    EXPECT_TRUE(str_utils::starts_with("", ""));
    EXPECT_TRUE(str_utils::starts_with("knuckleball", ""));
    EXPECT_TRUE(str_utils::starts_with("knuckleball", "knuckleball"));
    EXPECT_TRUE(str_utils::starts_with("knuckleball", "knuckle"));
}

TEST(str_utils, do_not_starts_with) {
    EXPECT_FALSE(str_utils::starts_with("", "knuckleball"));
    EXPECT_FALSE(str_utils::starts_with("knuckleball", "ball"));
}

TEST(str_utils, ends_with) {
    EXPECT_TRUE(str_utils::ends_with("", ""));
    EXPECT_TRUE(str_utils::ends_with("knuckleball", ""));
    EXPECT_TRUE(str_utils::ends_with("knuckleball", "knuckleball"));
    EXPECT_TRUE(str_utils::ends_with("knuckleball", "ball"));
}

TEST(str_utils, do_not_ends_with) {
    EXPECT_FALSE(str_utils::ends_with("", "knuckleball"));
    EXPECT_FALSE(str_utils::ends_with("knuckleball", "knuckle"));
}

TEST(str_utils, remove_spaces) {
    EXPECT_EQ(str_utils::remove_spaces(""), "");
    EXPECT_EQ(str_utils::remove_spaces(" "), "");
    EXPECT_EQ(str_utils::remove_spaces("knuckleball"), "knuckleball");
    EXPECT_EQ(str_utils::remove_spaces(" knuckle\nball\t"), "knuckleball");
}
