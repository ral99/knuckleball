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

#ifndef __EXCEPTIONS__
#define __EXCEPTIONS__

#define EXC_INVALID_STATEMENT "SyntaxError: invalid statement."
#define EXC_INVALID_ARGUMENT "RuntimeError: invalid argument."
#define EXC_INVALID_MESSAGE "RuntimeError: invalid message."
#define EXC_VARIABLE_NAME_ALREADY_USED "RuntimeError: variable name already used."
#define EXC_UNEXISTENT_VARIABLE "RuntimeError: name cannot be resolved to a variable."
#define EXC_WRONG_NUMBER_OF_ARGUMENTS "RuntimeError: wrong number of arguments."
#define EXC_INVALID_COMPARISON "RuntimeError: cannot compare these two types."
#define EXC_NOT_AUTHENTICATED "AuthenticationError: not authenticated."
#define EXC_WRONG_PASSWORD "AuthenticationError: wrong password."
#define EXC_UNKNOWN_ERROR "RuntimeError: unknown error."

#endif
