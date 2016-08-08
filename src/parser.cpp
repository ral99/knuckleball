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

#include "exceptions.h"
#include "grammar.h"
#include "parser.h"
#include "str_utils.h"

Parser::Parser(const std::string& input) {
    if (!Grammar::is_statement(input))
        throw EXC_INVALID_STATEMENT;
    int i = 0;
    std::string trimmed_input = str_utils::trim(input);
    trimmed_input = str_utils::rtrim(trimmed_input.substr(0, int(trimmed_input.size()) - 1));
    while (!str_utils::is_space(trimmed_input[i]) || !Grammar::is_actor(trimmed_input.substr(0, i)))
        i++;
    _actor = trimmed_input.substr(0, i);
    std::string message = str_utils::ltrim(trimmed_input.substr(i + 1));
    if (Grammar::is_unary_message(message))
        _message_name = message;
    else {
        int j = 0, k = 0;
        while (j < int(message.size())) {
            while (message[k] != ':' && message[k] != '?')
                k++;
            _message_name += message.substr(j, ++k - j);
            while (str_utils::is_space(message[k]))
                k++;
            j = k;
            while (k < int(message.size()) &&
                  (!str_utils::is_space(message[k]) || !Grammar::is_keyword_message_argument(message.substr(j, k - j))))
                k++;
            _arguments.push_back(message.substr(j, k - j));
            while (k < int(message.size()) && str_utils::is_space(message[k]))
                k++;
            j = k;
        }
    }
}

std::string Parser::actor() const {
    return _actor;
}

std::string Parser::message_name() const {
    return _message_name;
}

std::vector<std::string> Parser::arguments() const {
    return _arguments;
}
