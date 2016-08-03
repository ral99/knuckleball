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

#ifndef __CONTEXT__
#define __CONTEXT__

#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "object.h"
#include "parser.h"
#include "server.h"

class Context {
private:
    // Singleton instance:
    static Context *_instance;

    // Attributes:
    std::ofstream _logfile;
    int _float_precision;
    float _float_comparison_tolerance;
    std::map<std::string, std::shared_ptr<Object>> _variables;

    // Constructor:
    Context(const std::string& logfile_name, int float_precision, float float_comparison_tolerance);

    // Input processing methods:
    std::string execute_in_type(const Parser& parser);
    std::string execute_in_context(const Parser& parser);
    std::string execute_in_variable(const Parser& parser);

    // Operators:
    std::string op_listVariables(const std::vector<std::string>& arguments);
    std::string op_listVariablesOfNamespace(const std::vector<std::string>& arguments);
    std::string op_deleteVariable(const std::vector<std::string>& arguments);
    std::string op_deleteVariablesOfNamespace(const std::vector<std::string>& arguments);
    std::string op_getFloatPrecision(const std::vector<std::string>& arguments);
    std::string op_setFloatPrecision(const std::vector<std::string>& arguments);
    std::string op_getFloatComparisonTolerance(const std::vector<std::string>& arguments);
    std::string op_setFloatComparisonTolerance(const std::vector<std::string>& arguments);

    // Delete copy constructor and assignment operator:
    Context(const Context& other) = delete;
    Context& operator=(const Context& other) = delete;
public:
    // Singleton getter:
    static Context* get_instance(const std::string& logfile_name, int float_precision,
                                 float float_comparison_tolerance);
    static Context* get_instance();

    // Destructor:
    ~Context();

    // Decimal precision to be used to format floating-point values.
    int get_float_precision();
    void set_float_precision(int float_precision);

    // Tolerance to be used to compare floating-point values.
    float get_float_comparison_tolerance();
    void set_float_comparison_tolerance(float float_comparison_tolerance);

    // Execute the input string and return a string or throw an exception.
    std::string execute(const std::string& input, std::shared_ptr<Session> session=nullptr);
};

#endif
