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

#ifndef __GRAMMAR__
#define __GRAMMAR__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// GRAMMAR RULES /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// <spaces> --> \s*
// <boolean-type> --> Boolean
// <boolean-value> --> true | false
// <character-type> --> Character
// <character-value> --> '.'
// <integer-type> --> Integer
// <integer-value> --> [\+-]?[0-9]+
// <float-type> --> Float
// <float-value> --> [\+-]?([0-9]+ | [0-9]*\.[0-9]+ | [0-9]+\.[0-9]*)(e<integer-value>)?
// <string-type> --> String
// <string-value> --> ".*"
// <primitive-type> --> <boolean-type> | <character-type> | <integer-type> | <float-type> | <string-type>
// <primitive-value> --> <boolean-value> | <character-value> | <integer-value> | <float-value> | <string-value>
// <vector-type> --> Vector\<<spaces><primitive-type><spaces>\>
// <set-type> --> Set\<<spaces><primitive-type><spaces>\>
// <dictionary-type> --> Dictionary\<<spaces><primitive-type><spaces>,<spaces><primitive-type><spaces>\>
// <type> --> <primitive-type> | <vector-type> | <set-type> | <dictionary-type>
// <connection> --> Connection
// <context> --> Context
// <reserved-word> --> true | false | Connection | Context | Boolean | Character | Integer | Float | String | Vector |
//                     Set | Dictionary
// <identifier> --> [a-zA-Z][a-zA-Z0-9_]* - <reserved_word>
// <namespace> --> <identifier>
// <variable> --> (<namespace>::)?<identifier>
// <object> --> <connection> | <context> | <type> | <variable>
// <unary-message> --> <identifier>\??
// <keyword-message-token> --> <identifier>[:\?]{1}
// <keyword-message-argument> --> <primitive-value> | <namespace> | <variable>
// <keyword-message-part> --> <keyword-message-token><spaces><keyword-message-argument>
// <keyword-message> --> <keyword-message-part>(\s<spaces><keyword-message>)?
// <statement> --> <spaces><object>\s<spaces>(<unary-message> | <keyword-message>)<spaces>;<spaces>
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <set>
#include <string>

namespace Grammar {

// Set of reserved words.
extern std::set<std::string> RESERVED_WORDS;

// Grammar rule for <spaces>.
bool is_spaces(const std::string& str);

// Grammar rule for <boolean-type>.
bool is_boolean_type(const std::string& str);

// Grammar rule for <boolean-value>.
bool is_boolean_value(const std::string& str);

// Grammar rule for <character-type>.
bool is_character_type(const std::string& str);

// Grammar rule for <character-value>.
bool is_character_value(const std::string& str);

// Grammar rule for <integer-type>.
bool is_integer_type(const std::string& str);

// Grammar rule for <integer-value>.
bool is_integer_value(const std::string& str);

// Grammar rule for <float-type>.
bool is_float_type(const std::string& str);

// Grammar rule for <float-value>.
bool is_float_value(const std::string& str);

// Grammar rule for <string-type>.
bool is_string_type(const std::string& str);

// Grammar rule for <string-value>.
bool is_string_value(const std::string& str);

// Grammar rule for <primitive-type>.
bool is_primitive_type(const std::string& str);

// Grammar rule for <primitive-value>.
bool is_primitive_value(const std::string& str);

// Grammar rule for <vector-type>.
bool is_vector_type(const std::string& str);

// Grammar rule for <set-type>.
bool is_set_type(const std::string& str);

// Grammar rule for <dictionary-type>.
bool is_dictionary_type(const std::string& str);

// Grammar rule for <type>.
bool is_type(const std::string& str);

// Grammar rule for <connection>.
bool is_connection(const std::string& str);

// Grammar rule for <context>.
bool is_context(const std::string& str);

// Grammar rule for <reserved-word>.
bool is_reserved_word(const std::string& str);

// Grammar rule for <identifier>.
bool is_identifier(const std::string& str);

// Grammar rule for <namespace>.
bool is_namespace(const std::string& str);

// Grammar rule for <variable>.
bool is_variable(const std::string& str);

// Grammar rule for <object>.
bool is_object(const std::string& str);

// Grammar rule for <unary-message>.
bool is_unary_message(const std::string& str);

// Grammar rule for <keyword-message-token>.
bool is_keyword_message_token(const std::string& str);

// Grammar rule for <keyword-message-argument>.
bool is_keyword_message_argument(const std::string& str);

// Grammar rule for <keyword-message-part>.
bool is_keyword_message_part(const std::string& str);

// Grammar rule for <keyword-message>.
bool is_keyword_message(const std::string& str);

// Grammar rule for <statement>.
bool is_statement(const std::string& str);

}

#endif
