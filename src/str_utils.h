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

#ifndef __STR_UTILS__
#define __STR_UTILS__

#include <string>

namespace str_utils {

// Return the string trimmed from the left.
std::string ltrim(const std::string& str);

// Return the string trimmed from the right.
std::string rtrim(const std::string& str);

// Return the string trimmed from both ends.
std::string trim(const std::string& str);

// Return the string with character c escaped.
std::string escape(const std::string& str, char c);

// Return the string with character c unescaped.
std::string unescape(const std::string& str, char c);

// Check whether c is a space character.
bool is_space(char c);

// Check whether c is an alphabetic character.
bool is_alpha(char c);

// Check whether c is a numeric character.
bool is_numeric(char c);

// Check whether c is an alphabetic or numeric character.
bool is_alphanumeric(char c);

// Check whether the string is prefixed by prefix.
bool starts_with(const std::string& str, const std::string& prefix);

// Check whether the string is suffixed by suffix.
bool ends_with(const std::string& str, const std::string& suffix);

// Return the string with its spaces removed.
std::string remove_spaces(const std::string& str);

}

#endif
