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

#include "grammar.h"
#include "str_utils.h"

std::set<std::string> Grammar::RESERVED_WORDS {
    "true",
    "false",
    "Connection",
    "Context",
    "Boolean",
    "Character",
    "Integer",
    "Float",
    "String",
    "Vector",
    "Set",
    "Dictionary"
};

bool Grammar::is_spaces(const std::string& str) {
    return str_utils::ltrim(str).size() == 0;
}

bool Grammar::is_boolean_type(const std::string& str) {
    return str == "Boolean";
}

bool Grammar::is_boolean_value(const std::string& str) {
    return str == "true" || str == "false";
}

bool Grammar::is_character_type(const std::string& str) {
    return str == "Character";
}

bool Grammar::is_character_value(const std::string& str) {
    if (str.size() != 3)
        return false;
    return str[0] == '\'' && str[2] == '\'';
}

bool Grammar::is_integer_type(const std::string& str) {
    return str == "Integer";
}

bool Grammar::is_integer_value(const std::string& str) {
    if (str.size() == 0)
        return false;
    if (!str_utils::is_numeric(str[0]) && str[0] != '+' && str[0] != '-')
        return false;
    for (int i = 1; i < int(str.size()); i++)
        if (!str_utils::is_numeric(str[i]))
            return false;
    return str_utils::is_numeric(str[0]) || str.size() > 1;
}

bool Grammar::is_float_type(const std::string& str) {
    return str == "Float";
}

bool Grammar::is_float_value(const std::string& str) {
    if (str.size() == 0)
        return false;
    int count_point = 0, count_numeric = 0;
    for (int i = (str[0] == '+' || str[0] == '-') ? 1 : 0; i < int(str.size()); i++) {
        if (!str_utils::is_numeric(str[i]) && str[i] != '.') {
            if (str[i] != 'e')
                return false;
            return count_point <= 1 && count_numeric > 0 && is_integer_value(str.substr(i + 1));
        }
        if (str[i] == '.')
            count_point++;
        if (str_utils::is_numeric(str[i]))
            count_numeric++;
    }
    return count_point <= 1 && count_numeric > 0;
}

bool Grammar::is_string_type(const std::string& str) {
    return str == "String";
}

bool Grammar::is_string_value(const std::string& str) {
    if (str.size() < 2)
        return false;
    if (str[0] != '"' || str[int(str.size()) - 1] != '"')
        return false;
    for (int i = 1; i < int(str.size()) - 1; i++)
        if (str[i] == '"')
            for (int j = i - 1; j >= 0; j--)
                if (str[j] != '\\') {
                    if ((i - j) % 2 == 1)
                        return false;
                    break;
                }
    for (int i = int(str.size()) - 2; i > 0; i--)
        if (str[i] != '\\')
            return (int(str.size()) - 1 - i) % 2 == 1;
    return str.size() % 2 == 0;
}

bool Grammar::is_primitive_type(const std::string& str) {
    return is_boolean_type(str) || is_character_type(str) || is_integer_type(str) || is_float_type(str) ||
           is_string_type(str);
}

bool Grammar::is_primitive_value(const std::string& str) {
    return is_boolean_value(str) || is_character_value(str) || is_integer_value(str) || is_float_value(str) ||
           is_string_value(str);
}

bool Grammar::is_vector_type(const std::string& str) {
    if (!str_utils::starts_with(str, "Vector<") || !str_utils::ends_with(str, ">"))
        return false;
    return is_primitive_type(str_utils::trim(str.substr(7, int(str.size()) - 8)));
}

bool Grammar::is_set_type(const std::string& str) {
    if (!str_utils::starts_with(str, "Set<") || !str_utils::ends_with(str, ">"))
        return false;
    return is_primitive_type(str_utils::trim(str.substr(4, int(str.size()) - 5)));
}

bool Grammar::is_dictionary_type(const std::string& str) {
    if (!str_utils::starts_with(str, "Dictionary<") || !str_utils::ends_with(str, ">"))
        return false;
    std::string types_of_dictionary = str.substr(11, int(str.size()) - 12);
    for (int i = 0; i < int(types_of_dictionary.size()); i++)
        if (types_of_dictionary[i] == ',')
            return is_primitive_type(str_utils::trim(types_of_dictionary.substr(0, i))) &&
                   is_primitive_type(str_utils::trim(types_of_dictionary.substr(i + 1)));
    return false;
}

bool Grammar::is_type(const std::string& str) {
    return is_primitive_type(str) || is_vector_type(str) || is_set_type(str) || is_dictionary_type(str);
}

bool Grammar::is_connection(const std::string& str) {
    return str == "Connection";
}

bool Grammar::is_context(const std::string& str) {
    return str == "Context";
}

bool Grammar::is_reserved_word(const std::string& str) {
    return RESERVED_WORDS.find(str) != RESERVED_WORDS.end();
}

bool Grammar::is_identifier(const std::string& str) {
    if (str.size() == 0 || !str_utils::is_alpha(str[0]))
        return false;
    for (int i = 1; i < int(str.size()); i++)
        if (!str_utils::is_alphanumeric(str[i]) && str[i] != '_')
            return false;
    return !is_reserved_word(str);
}

bool Grammar::is_namespace(const std::string& str) {
    return is_identifier(str);
}

bool Grammar::is_variable(const std::string& str) {
    if (str.size() == 0)
        return false;
    for (int i = 0; i < int(str.size()) - 1; i++)
        if (str[i] == ':' && str[i + 1] == ':')
            return is_namespace(str.substr(0, i)) && is_identifier(str.substr(i + 2));
    return is_identifier(str);
}

bool Grammar::is_object(const std::string& str) {
    return is_connection(str) || is_context(str) || is_type(str) || is_variable(str);
}

bool Grammar::is_unary_message(const std::string& str) {
    return str.size() > 0 &&
           (is_identifier(str) || (str[int(str.size()) - 1] == '?' && is_identifier(str.substr(0, int(str.size()) - 1))));
}

bool Grammar::is_keyword_message_token(const std::string& str) {
    return str.size() > 0 && (str[int(str.size()) - 1] == ':' || str[int(str.size()) - 1] == '?') &&
           is_identifier(str.substr(0, int(str.size()) - 1));
}

bool Grammar::is_keyword_message_argument(const std::string& str) {
    return is_primitive_value(str) || is_namespace(str) || is_variable(str);
}

bool Grammar::is_keyword_message_part(const std::string& str) {
    for (int i = 0; i < int(str.size()); i++)
        if (str[i] == ':' || str[i] == '?')
            return is_keyword_message_token(str.substr(0, i + 1)) &&
                   is_keyword_message_argument(str_utils::ltrim(str.substr(i + 1)));
    return false;
}

bool Grammar::is_keyword_message(const std::string& str) {
    if (is_keyword_message_part(str))
        return true;
    for (int i = 0; i < int(str.size()); i++)
        if (str_utils::is_space(str[i]) && is_keyword_message_part(str.substr(0, i)) &&
            is_keyword_message(str_utils::ltrim(str.substr(i + 1))))
            return true;
    return false;
}

bool Grammar::is_statement(const std::string& str) {
    std::string trimmed_str = str_utils::trim(str);
    if (trimmed_str.size() == 0 || trimmed_str[int(trimmed_str.size()) - 1] != ';')
        return false;
    trimmed_str = str_utils::rtrim(trimmed_str.substr(0, int(trimmed_str.size()) - 1));
    for (int i = 0; i < int(trimmed_str.size()); i++)
        if (str_utils::is_space(trimmed_str[i]) && is_object(trimmed_str.substr(0, i))) {
            std::string object = trimmed_str.substr(0, i);
            std::string message = str_utils::ltrim(trimmed_str.substr(i + 1));
            return is_object(object) && (is_unary_message(message) || is_keyword_message(message));
        }
    return false;
}
