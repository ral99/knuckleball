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

#include <ctime>
#include <iostream>

#include "context.h"
#include "exceptions.h"
#include "grammar.h"
#include "str_utils.h"

// Initialize the singleton instance.
Context* Context::_instance = NULL;

Context* Context::get_instance(const std::string& logfile_name, int float_precision, float float_comparison_tolerance) {
    delete _instance;
    _instance = new Context(logfile_name, float_precision, float_comparison_tolerance);
    return _instance;
}

Context* Context::get_instance() {
    return _instance;
}

Context::Context(const std::string& logfile_name, int float_precision, float float_comparison_tolerance) :
    _float_precision(float_precision),
    _float_comparison_tolerance(float_comparison_tolerance) {
    if (logfile_name != "")
        _logfile.open(logfile_name, std::ios::out | std::ios::app);
}

Context::~Context() {
    if (_logfile.is_open())
        _logfile.close();
    _instance = NULL;
}

int Context::get_float_precision() {
    return _float_precision;
}

void Context::set_float_precision(int float_precision) {
    _float_precision = float_precision;
}

float Context::get_float_comparison_tolerance() {
    return _float_comparison_tolerance;
}

void Context::set_float_comparison_tolerance(float float_comparison_tolerance) {
    _float_comparison_tolerance = float_comparison_tolerance;
}

std::string Context::execute_in_type(const Parser& parser) {
    std::shared_ptr<Object> variable;
    std::string type = str_utils::remove_spaces(parser.object());
    std::string message_name = parser.message_name();
    if (type == "Boolean")
        variable = std::make_shared<Boolean>(message_name, parser.arguments());
    else if (type == "Character")
        variable = std::make_shared<Character>(message_name, parser.arguments());
    else if (type == "Integer")
        variable = std::make_shared<Integer>(message_name, parser.arguments());
    else if (type == "Float")
        variable = std::make_shared<Float>(message_name, parser.arguments());
    else if (type == "String")
        variable = std::make_shared<String>(message_name, parser.arguments());
    else if (str_utils::starts_with(type, "Vector"))
        variable = std::make_shared<Vector>(type.substr(7, int(type.size()) - 8), message_name, parser.arguments());
    else if (str_utils::starts_with(type, "Set"))
        variable = std::make_shared<Set>(type.substr(4, int(type.size()) - 5), message_name, parser.arguments());
    else if (str_utils::starts_with(type, "Dictionary")) {
        std::string types_of_dictionary = type.substr(11, int(type.size()) - 12);
        for (int i = 0; i < int(types_of_dictionary.size()); i++)
            if (types_of_dictionary[i] == ',')
                variable = std::make_shared<Dictionary>(types_of_dictionary.substr(0, i),
                                                        types_of_dictionary.substr(i + 1),
                                                        message_name, parser.arguments());
    }
    if (_variables.find(variable->name()) == _variables.end()) {
        _variables[variable->name()] = variable;
        return "null";
    }
    if (str_utils::starts_with(message_name, "createIfNotExists:"))
        return "null";
    throw EXC_VARIABLE_NAME_ALREADY_USED;
} 

std::string Context::execute_in_context(const Parser& parser) {
    std::string message_name = parser.message_name();
    if (message_name == "listVariables")
        return op_listVariables(parser.arguments());
    else if (message_name == "listVariablesOfNamespace:")
        return op_listVariablesOfNamespace(parser.arguments());
    else if (message_name == "deleteVariable:")
        return op_deleteVariable(parser.arguments());
    else if (message_name == "deleteVariablesOfNamespace:")
        return op_deleteVariablesOfNamespace(parser.arguments());
    else if (message_name == "getFloatPrecision")
        return op_getFloatPrecision(parser.arguments());
    else if (message_name == "setFloatPrecision:")
        return op_setFloatPrecision(parser.arguments());
    else if (message_name == "getFloatComparisonTolerance")
        return op_getFloatComparisonTolerance(parser.arguments());
    else if (message_name == "setFloatComparisonTolerance:")
        return op_setFloatComparisonTolerance(parser.arguments());
    throw EXC_INVALID_MESSAGE;
}

std::string Context::execute_in_variable(const Parser& parser) {
    auto variable = _variables.find(parser.object());
    if (variable == _variables.end())
        throw EXC_UNEXISTENT_VARIABLE;
    return variable->second->receive(parser.message_name(), parser.arguments());
}

std::string Context::execute(const std::string& input, std::shared_ptr<Session> session) {
    std::string output;
    try {
        Parser parser(input);
        std::string object = parser.object();
        if (Grammar::is_type(object))
            output = execute_in_type(parser);
        else if (Grammar::is_context(object))
            output = execute_in_context(parser);
        else if (Grammar::is_variable(object))
            output = execute_in_variable(parser);
        else if (Grammar::is_connection(object))
            output = session->receive(parser.message_name(), parser.arguments());
    }
    catch (const char* exception) {
        output = std::string(exception);
    }
    catch (...) {
        output = std::string(EXC_UNKNOWN_ERROR);
    }
    char timestamp[32];
    time_t now = time(NULL);
    strftime(timestamp, sizeof(timestamp), "[%F %T]", localtime(&now));
    if (_logfile.is_open())
        _logfile << timestamp << " " << str_utils::trim(input) << " -> " << output << std::endl;
    else
        std::cout << timestamp << " " << str_utils::trim(input) << " -> " << output << std::endl;
    return output;
}

std::string Context::op_listVariables(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::string elements_str;
    for (auto it = _variables.begin(); it != _variables.end(); it++) {
        if (it != _variables.begin())
            elements_str += ",";
        elements_str += it->first;
    }
    return "[" + elements_str + "]";
}

std::string Context::op_listVariablesOfNamespace(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_namespace(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    std::string elements_str;
    for (auto it = _variables.begin(); it != _variables.end(); it++) {
        if (!str_utils::starts_with(it->first, arguments[0] + "::"))
            continue;
        if (elements_str.size() > 0)
            elements_str += ",";
        elements_str += it->first;
    }
    return "[" + elements_str + "]";
}

std::string Context::op_deleteVariable(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_variable(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    for (auto it = _variables.begin(); it != _variables.end(); it++)
        if (it->first == arguments[0]) {
            _variables.erase(it);
            return "null";
        }
    throw EXC_UNEXISTENT_VARIABLE;
}

std::string Context::op_deleteVariablesOfNamespace(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_namespace(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    std::vector<std::map<std::string, std::shared_ptr<Object>>::iterator> variables_to_be_deleted;
    for (auto it = _variables.begin(); it != _variables.end(); it++)
        if (str_utils::starts_with(it->first, arguments[0] + "::"))
            variables_to_be_deleted.push_back(it);
    for (auto it = variables_to_be_deleted.begin(); it != variables_to_be_deleted.end(); it++)
        _variables.erase(*it);
    return "null";
}

std::string Context::op_getFloatPrecision(const std::vector<std::string>& arguments) {
    return Integer(_float_precision).representation();
}

std::string Context::op_setFloatPrecision(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int float_precision = Integer(arguments[0]).value();
    if (float_precision <= 0)
        throw EXC_INVALID_ARGUMENT;
    _float_precision = float_precision;
    return "null";
}

std::string Context::op_getFloatComparisonTolerance(const std::vector<std::string>& arguments) {
    return Float(_float_comparison_tolerance).representation();
}

std::string Context::op_setFloatComparisonTolerance(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    float float_comparison_tolerance = Float(arguments[0]).value();
    if (float_comparison_tolerance < 0)
        throw EXC_INVALID_ARGUMENT;
    _float_comparison_tolerance = float_comparison_tolerance;
    return "null";
}
