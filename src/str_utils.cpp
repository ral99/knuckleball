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

#include <cctype>

#include "str_utils.h"

namespace str_utils {

std::string ltrim(const std::string& str) {
    int i = 0;
    while (i < int(str.size()) && isspace(str[i]))
        i++;
    return str.substr(i);
}

std::string rtrim(const std::string& str) {
    int i = int(str.size()) - 1;
    while (i >= 0 && isspace(str[i]))
        i--;
    return str.substr(0, i + 1);
}

std::string trim(const std::string& str) {
    return rtrim(ltrim(str));
}

std::string escape(const std::string& str, char c) {
    std::string escaped_str;
    for (int i = 0; i < int(str.size()); i++) {
        if (str[i] == c)
            escaped_str += "\\";
        escaped_str += std::string(1, str[i]);
    }
    return escaped_str;
}

std::string unescape(const std::string& str, char c) {
    std::string unescaped_str;
    for (int i = 0; i < int(str.size()); i++) {
        if (i < int(str.size()) - 1 && str[i] == '\\' && str[i + 1] == c)
            continue;
        unescaped_str += std::string(1, str[i]);
    }
    return unescaped_str;
}

bool is_space(char c) {
    return isspace(c);
}

bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_numeric(char c) {
    return (c >= '0' && c <= '9');
}

bool is_alphanumeric(char c) {
    return is_alpha(c) || is_numeric(c);
}

bool starts_with(const std::string& str, const std::string& prefix) {
    if (str.size() < prefix.size())
        return false;
    for (int i = 0; i < int(prefix.size()); i++)
        if (str[i] != prefix[i])
            return false;
    return true;
}

bool ends_with(const std::string& str, const std::string& suffix) {
    if (str.size() < suffix.size())
        return false;
    for (int i = 0; i < int(suffix.size()); i++)
        if (str[int(str.size()) - i - 1] != suffix[int(suffix.size()) - i - 1])
            return false;
    return true;
}

std::string remove_spaces(const std::string& str) {
    std::string str_without_spaces;
    for (int i = 0; i < int(str.size()); i++) {
        if (is_space(str[i]))
            continue;
        str_without_spaces += std::string(1, str[i]);
    }
    return str_without_spaces;
}

}
