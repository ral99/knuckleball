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

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#include "context.h"
#include "exceptions.h"
#include "grammar.h"
#include "instance.h"
#include "str_utils.h"

/////////////////////////////////////////////////////// Auxiliar ///////////////////////////////////////////////////////
    
std::shared_ptr<Instance> get_shared_instance(const std::string& type, const std::string& value) {
    std::shared_ptr<Instance> instance;
    if (type == "Boolean")
        instance = std::make_shared<BooleanInstance>(value);
    if (type == "Character")
        instance = std::make_shared<CharacterInstance>(value);
    if (type == "Integer")
        instance = std::make_shared<IntegerInstance>(value);
    if (type == "Float")
        instance = std::make_shared<FloatInstance>(value);
    if (type == "String")
        instance = std::make_shared<StringInstance>(value);
    return instance;
}

std::string vector_representation(const std::vector<std::shared_ptr<Instance>>& vector) {
    std::string elements_str;
    for (auto it = vector.begin(); it != vector.end(); it++) {
        if (it != vector.begin())
            elements_str += ",";
        elements_str += (*it)->representation();
    }
    return "[" + elements_str + "]";
}

/////////////////////////////////////////////////////// Instance ///////////////////////////////////////////////////////

Instance::Instance(const std::string& type) : _type(type) {
}

Instance::~Instance() {
}

std::string Instance::type() const {
    return _type;
}

std::string Instance::name() const {
    return _name;
}

///////////////////////////////////////////// InstanceIsEqualToComparator //////////////////////////////////////////////

bool InstanceIsEqualToComparator::operator() (const std::shared_ptr<Instance>& lhs,
                                              const std::shared_ptr<Instance>& rhs) const {
    return !InstanceIsLessThanComparator()(lhs, rhs) && !InstanceIsGreaterThanComparator()(lhs, rhs);
}

///////////////////////////////////////////// InstanceIsLessThanComparator /////////////////////////////////////////////

bool InstanceIsLessThanComparator::operator()(const std::shared_ptr<Instance>& lhs,
                                              const std::shared_ptr<Instance>& rhs) const {
    const std::string lhs_type = lhs->type();
    const std::string rhs_type = rhs->type();
    float eps = Context::get_instance()->get_float_comparison_tolerance();
    if (lhs_type == "Boolean" && rhs_type == "Boolean")
        return static_cast<const BooleanInstance&>(*lhs).value() < static_cast<const BooleanInstance&>(*rhs).value();
    if (lhs_type == "Character" && rhs_type == "Character")
        return static_cast<const CharacterInstance&>(*lhs).value() <
               static_cast<const CharacterInstance&>(*rhs).value();
    if (lhs_type == "Integer" && rhs_type == "Integer")
        return static_cast<const IntegerInstance&>(*lhs).value() < static_cast<const IntegerInstance&>(*rhs).value();
    if (lhs_type == "Integer" && rhs_type == "Float")
        return float(static_cast<const IntegerInstance&>(*lhs).value()) -
               static_cast<const FloatInstance&>(*rhs).value() < -eps;
    if (lhs_type == "Float" && rhs_type == "Float")
        return static_cast<const FloatInstance&>(*lhs).value() - static_cast<const FloatInstance&>(*rhs).value() < -eps;
    if (lhs_type == "Float" && rhs_type == "Integer")
        return static_cast<const FloatInstance&>(*lhs).value() -
               float(static_cast<const IntegerInstance&>(*rhs).value()) < -eps;
    if (lhs_type == "String" && rhs_type == "String")
        return static_cast<const StringInstance&>(*lhs).value() < static_cast<const StringInstance&>(*rhs).value();
    throw EXC_INVALID_COMPARISON;
}

///////////////////////////////////////// InstanceIsLessThanOrEqualToComparator ////////////////////////////////////////

bool InstanceIsLessThanOrEqualToComparator::operator()(const std::shared_ptr<Instance>& lhs,
                                                       const std::shared_ptr<Instance>& rhs) const {
    return InstanceIsLessThanComparator()(lhs, rhs) || InstanceIsEqualToComparator()(lhs, rhs);
}

/////////////////////////////////////////// InstanceIsGreaterThanComparator ////////////////////////////////////////////

bool InstanceIsGreaterThanComparator::operator()(const std::shared_ptr<Instance>& lhs,
                                                 const std::shared_ptr<Instance>& rhs) const {
    const std::string lhs_type = lhs->type();
    const std::string rhs_type = rhs->type();
    float eps = Context::get_instance()->get_float_comparison_tolerance();
    if (lhs_type == "Boolean" && rhs_type == "Boolean")
        return static_cast<const BooleanInstance&>(*lhs).value() > static_cast<const BooleanInstance&>(*rhs).value();
    if (lhs_type == "Character" && rhs_type == "Character")
        return static_cast<const CharacterInstance&>(*lhs).value() >
               static_cast<const CharacterInstance&>(*rhs).value();
    if (lhs_type == "Integer" && rhs_type == "Integer")
        return static_cast<const IntegerInstance&>(*lhs).value() > static_cast<const IntegerInstance&>(*rhs).value();
    if (lhs_type == "Integer" && rhs_type == "Float")
        return float(static_cast<const IntegerInstance&>(*lhs).value()) -
               static_cast<const FloatInstance&>(*rhs).value() > eps;
    if (lhs_type == "Float" && rhs_type == "Float")
        return static_cast<const FloatInstance&>(*lhs).value() - static_cast<const FloatInstance&>(*rhs).value() > eps;
    if (lhs_type == "Float" && rhs_type == "Integer")
        return static_cast<const FloatInstance&>(*lhs).value() -
               float(static_cast<const IntegerInstance&>(*rhs).value()) > eps;
    if (lhs_type == "String" && rhs_type == "String")
        return static_cast<const StringInstance&>(*lhs).value() > static_cast<const StringInstance&>(*rhs).value();
    throw EXC_INVALID_COMPARISON;
}

/////////////////////////////////////// InstanceIsGreaterThanOrEqualToComparator ///////////////////////////////////////

bool InstanceIsGreaterThanOrEqualToComparator::operator()(const std::shared_ptr<Instance>& lhs,
                                                          const std::shared_ptr<Instance>& rhs) const {
    return InstanceIsGreaterThanComparator()(lhs, rhs) || InstanceIsEqualToComparator()(lhs, rhs);
}

/////////////////////////////////////////////////// BooleanInstance ////////////////////////////////////////////////////

BooleanInstance::BooleanInstance(const std::string& value) : Instance("Boolean") {
    if (!Grammar::is_boolean_value(value))
        throw EXC_INVALID_ARGUMENT;
    _value = (value == "true");
}

BooleanInstance::BooleanInstance(const std::string& message_name, const std::vector<std::string>& arguments) :
    Instance("Boolean") {
    if (message_name == "create:withValue:" || message_name == "createIfNotExists:withValue:") {
        if (arguments.size() != 2)
            throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
        if (!Grammar::is_variable(arguments[0]) || !Grammar::is_boolean_value(arguments[1]))
            throw EXC_INVALID_ARGUMENT;
        _name = arguments[0];
        _value = (arguments[1] == "true");
    }
    else
        throw EXC_INVALID_MESSAGE;
}

BooleanInstance::~BooleanInstance() {
}

bool BooleanInstance::value() const {
    return _value;
}

std::string BooleanInstance::representation() const {
    return (_value == true) ? "true" : "false";
}

std::string BooleanInstance::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
    if (message_name == "get")
        return op_get(arguments);
    if (message_name == "set:")
        return op_set(arguments);
    if (message_name == "isTrue?")
        return op_isTrue(arguments);
    if (message_name == "isFalse?")
        return op_isFalse(arguments);
    throw EXC_INVALID_MESSAGE;
}

std::string BooleanInstance::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string BooleanInstance::op_set(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_boolean_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value = (arguments[0] == "true");
    return "null";
}

std::string BooleanInstance::op_isTrue(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (_value == true) ? "true" : "false";
}

std::string BooleanInstance::op_isFalse(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (_value == false) ? "true" : "false";
}

////////////////////////////////////////////////// CharacterInstance ///////////////////////////////////////////////////

CharacterInstance::CharacterInstance(const std::string& value) : Instance("Character") {
    if (!Grammar::is_character_value(value))
        throw EXC_INVALID_ARGUMENT;
    _value = value[1];
}

CharacterInstance::CharacterInstance(const std::string& message_name, const std::vector<std::string>& arguments) :
    Instance("Character") {
    if (message_name == "create:withValue:" || message_name == "createIfNotExists:withValue:") {
        if (arguments.size() != 2)
            throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
        if (!Grammar::is_variable(arguments[0]) || !Grammar::is_character_value(arguments[1]))
            throw EXC_INVALID_ARGUMENT;
        _name = arguments[0];
        _value = arguments[1][1];
    }
    else
        throw EXC_INVALID_MESSAGE;
}

CharacterInstance::~CharacterInstance() {
}

char CharacterInstance::value() const {
    return _value;
}

std::string CharacterInstance::representation() const {
    return "'" + std::string(1, _value) + "'";
}

std::string CharacterInstance::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
    if (message_name == "get")
        return op_get(arguments);
    if (message_name == "set:")
        return op_set(arguments);
    if (message_name == "isAlphabetic?")
        return op_isAlphabetic(arguments);
    if (message_name == "isNumeric?")
        return op_isNumeric(arguments);
    if (message_name == "isAlphanumeric?")
        return op_isAlphanumeric(arguments);
    if (message_name == "isSpace?")
        return op_isSpace(arguments);
    if (message_name == "isEqualTo?")
        return op_isEqualTo(arguments);
    if (message_name == "isLessThan?")
        return op_isLessThan(arguments);
    if (message_name == "isLessThanOrEqualTo?")
        return op_isLessThanOrEqualTo(arguments);
    if (message_name == "isGreaterThan?")
        return op_isGreaterThan(arguments);
    if (message_name == "isGreaterThanOrEqualTo?")
        return op_isGreaterThanOrEqualTo(arguments);
    throw EXC_INVALID_MESSAGE;
}

std::string CharacterInstance::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string CharacterInstance::op_set(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value = arguments[0][1];
    return "null";
}

std::string CharacterInstance::op_isAlphabetic(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (str_utils::is_alpha(_value)) ? "true" : "false";
}

std::string CharacterInstance::op_isNumeric(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (str_utils::is_numeric(_value)) ? "true" : "false";
}

std::string CharacterInstance::op_isAlphanumeric(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (str_utils::is_alphanumeric(_value)) ? "true" : "false";
}

std::string CharacterInstance::op_isSpace(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (str_utils::is_space(_value)) ? "true" : "false";
}

std::string CharacterInstance::op_isEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsEqualToComparator()(shared_from_this(), std::make_shared<CharacterInstance>(arguments[0])) ?
           "true" : "false";
}

std::string CharacterInstance::op_isLessThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsLessThanComparator()(shared_from_this(), std::make_shared<CharacterInstance>(arguments[0])) ?
           "true" : "false";
}

std::string CharacterInstance::op_isLessThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsLessThanOrEqualToComparator()(shared_from_this(),
                                                   std::make_shared<CharacterInstance>(arguments[0])) ?
           "true" : "false";
}

std::string CharacterInstance::op_isGreaterThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsGreaterThanComparator()(shared_from_this(), std::make_shared<CharacterInstance>(arguments[0])) ?
           "true" : "false";
}

std::string CharacterInstance::op_isGreaterThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsGreaterThanOrEqualToComparator()(shared_from_this(),
                                                      std::make_shared<CharacterInstance>(arguments[0])) ?
           "true" : "false";
}

/////////////////////////////////////////////////// IntegerInstance ////////////////////////////////////////////////////

IntegerInstance::IntegerInstance(int value) : Instance("Integer") {
    _value = value;
}

IntegerInstance::IntegerInstance(const std::string& value) : Instance("Integer") {
    if (!Grammar::is_integer_value(value) && !Grammar::is_float_value(value))
        throw EXC_INVALID_ARGUMENT;
    _value = (Grammar::is_integer_value(value)) ? atoi(value.c_str()) : atof(value.c_str());
}

IntegerInstance::IntegerInstance(const std::string& message_name, const std::vector<std::string>& arguments) :
    Instance("Integer") {
    if (message_name == "create:withValue:" || message_name == "createIfNotExists:withValue:") {
        if (arguments.size() != 2)
            throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
        if (!Grammar::is_variable(arguments[0]) ||
           (!Grammar::is_integer_value(arguments[1]) && !Grammar::is_float_value(arguments[1])))
            throw EXC_INVALID_ARGUMENT;
        _name = arguments[0];
        _value = (Grammar::is_integer_value(arguments[1])) ? atoi(arguments[1].c_str()) : atof(arguments[1].c_str());
    }
    else
        throw EXC_INVALID_MESSAGE;
}

IntegerInstance::~IntegerInstance() {
}

int IntegerInstance::value() const {
    return _value;
}

std::string IntegerInstance::representation() const {
    return std::to_string(_value);
}

std::string IntegerInstance::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
    if (message_name == "get")
        return op_get(arguments);
    if (message_name == "set:")
        return op_set(arguments);
    if (message_name == "isEqualTo?")
        return op_isEqualTo(arguments);
    if (message_name == "isLessThan?")
        return op_isLessThan(arguments);
    if (message_name == "isLessThanOrEqualTo?")
        return op_isLessThanOrEqualTo(arguments);
    if (message_name == "isGreaterThan?")
        return op_isGreaterThan(arguments);
    if (message_name == "isGreaterThanOrEqualTo?")
        return op_isGreaterThanOrEqualTo(arguments);
    if (message_name == "add:")
        return op_add(arguments);
    if (message_name == "subtract:")
        return op_subtract(arguments);
    if (message_name == "multiplyBy:")
        return op_multiplyBy(arguments);
    if (message_name == "divideBy:")
        return op_divideBy(arguments);
    throw EXC_INVALID_MESSAGE;
}

std::string IntegerInstance::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string IntegerInstance::op_set(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value = (Grammar::is_integer_value(arguments[0])) ? atoi(arguments[0].c_str()) : atof(arguments[0].c_str());
    return "null";
}

std::string IntegerInstance::op_isEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    if (Grammar::is_integer_value(arguments[0]))
        return InstanceIsEqualToComparator()(shared_from_this(), std::make_shared<IntegerInstance>(arguments[0])) ?
               "true" : "false";
    return InstanceIsEqualToComparator()(shared_from_this(), std::make_shared<FloatInstance>(arguments[0])) ?
           "true" : "false";
}

std::string IntegerInstance::op_isLessThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    if (Grammar::is_integer_value(arguments[0]))
        return InstanceIsLessThanComparator()(shared_from_this(), std::make_shared<IntegerInstance>(arguments[0])) ?
               "true" : "false";
    return InstanceIsLessThanComparator()(shared_from_this(), std::make_shared<FloatInstance>(arguments[0])) ?
           "true" : "false";
}

std::string IntegerInstance::op_isLessThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    if (Grammar::is_integer_value(arguments[0]))
        return InstanceIsLessThanOrEqualToComparator()(shared_from_this(),
                                                       std::make_shared<IntegerInstance>(arguments[0])) ?
               "true" : "false";
    return InstanceIsLessThanOrEqualToComparator()(shared_from_this(), std::make_shared<FloatInstance>(arguments[0])) ?
           "true" : "false";
}

std::string IntegerInstance::op_isGreaterThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    if (Grammar::is_integer_value(arguments[0]))
        return InstanceIsGreaterThanComparator()(shared_from_this(), std::make_shared<IntegerInstance>(arguments[0])) ?
               "true" : "false";
    return InstanceIsGreaterThanComparator()(shared_from_this(), std::make_shared<FloatInstance>(arguments[0])) ?
           "true" : "false";
}

std::string IntegerInstance::op_isGreaterThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    if (Grammar::is_integer_value(arguments[0]))
        return InstanceIsGreaterThanOrEqualToComparator()(shared_from_this(),
                                                          std::make_shared<IntegerInstance>(arguments[0])) ?
               "true" : "false";
    return InstanceIsGreaterThanOrEqualToComparator()(shared_from_this(),
                                                      std::make_shared<FloatInstance>(arguments[0])) ? "true" : "false";
}

std::string IntegerInstance::op_add(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT; 
    _value += (Grammar::is_integer_value(arguments[0])) ? atoi(arguments[0].c_str()) : atof(arguments[0].c_str());
    return "null";
}

std::string IntegerInstance::op_subtract(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value -= (Grammar::is_integer_value(arguments[0])) ? atoi(arguments[0].c_str()) : atof(arguments[0].c_str());
    return "null";
}

std::string IntegerInstance::op_multiplyBy(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT; 
    _value *= (Grammar::is_integer_value(arguments[0])) ? atoi(arguments[0].c_str()) : atof(arguments[0].c_str());
    return "null";
}

std::string IntegerInstance::op_divideBy(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT; 
    _value /= (Grammar::is_integer_value(arguments[0])) ? atoi(arguments[0].c_str()) : atof(arguments[0].c_str());
    return "null";
}

//////////////////////////////////////////////////// FloatInstance /////////////////////////////////////////////////////

FloatInstance::FloatInstance(float value) : Instance("Float") {
    _value = value;
}

FloatInstance::FloatInstance(const std::string& value) : Instance("Float") {
    if (!Grammar::is_float_value(value))
        throw EXC_INVALID_ARGUMENT;
    _value = atof(value.c_str());
}

FloatInstance::FloatInstance(const std::string& message_name, const std::vector<std::string>& arguments) :
    Instance("Float") {
    if (message_name == "create:withValue:" || message_name == "createIfNotExists:withValue:") {
        if (arguments.size() != 2)
            throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
        if (!Grammar::is_variable(arguments[0]) || !Grammar::is_float_value(arguments[1]))
            throw EXC_INVALID_ARGUMENT;
        _name = arguments[0];
        _value = atof(arguments[1].c_str());
    }
    else
        throw EXC_INVALID_MESSAGE;
}

FloatInstance::~FloatInstance() {
}

float FloatInstance::value() const {
    return _value;
}

std::string FloatInstance::representation() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(Context::get_instance()->get_float_precision()) << _value;
    return oss.str();
}

std::string FloatInstance::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
    if (message_name == "get")
        return op_get(arguments);
    if (message_name == "set:")
        return op_set(arguments);
    if (message_name == "isEqualTo?")
        return op_isEqualTo(arguments);
    if (message_name == "isLessThan?")
        return op_isLessThan(arguments);
    if (message_name == "isLessThanOrEqualTo?")
        return op_isLessThanOrEqualTo(arguments);
    if (message_name == "isGreaterThan?")
        return op_isGreaterThan(arguments);
    if (message_name == "isGreaterThanOrEqualTo?")
        return op_isGreaterThanOrEqualTo(arguments);
    if (message_name == "add:")
        return op_add(arguments);
    if (message_name == "subtract:")
        return op_subtract(arguments);
    if (message_name == "multiplyBy:")
        return op_multiplyBy(arguments);
    if (message_name == "divideBy:")
        return op_divideBy(arguments);
    throw EXC_INVALID_MESSAGE;
}

std::string FloatInstance::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string FloatInstance::op_set(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value = atof(arguments[0].c_str());
    return "null";
}

std::string FloatInstance::op_isEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsEqualToComparator()(shared_from_this(), std::make_shared<FloatInstance>(arguments[0])) ?
           "true" : "false";
}

std::string FloatInstance::op_isLessThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsLessThanComparator()(shared_from_this(), std::make_shared<FloatInstance>(arguments[0])) ?
           "true" : "false";
}

std::string FloatInstance::op_isLessThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsLessThanOrEqualToComparator()(shared_from_this(), std::make_shared<FloatInstance>(arguments[0])) ?
           "true" : "false";
}

std::string FloatInstance::op_isGreaterThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsGreaterThanComparator()(shared_from_this(), std::make_shared<FloatInstance>(arguments[0])) ?
           "true" : "false";
}

std::string FloatInstance::op_isGreaterThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsGreaterThanOrEqualToComparator()(shared_from_this(),
                                                      std::make_shared<FloatInstance>(arguments[0])) ? "true" : "false";
}

std::string FloatInstance::op_add(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT; 
    _value += atof(arguments[0].c_str());
    return "null";
}

std::string FloatInstance::op_subtract(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value -= atof(arguments[0].c_str());
    return "null";
}

std::string FloatInstance::op_multiplyBy(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT; 
    _value *= atof(arguments[0].c_str());
    return "null";
}

std::string FloatInstance::op_divideBy(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT; 
    _value /= atof(arguments[0].c_str());
    return "null";
}

//////////////////////////////////////////////////// StringInstance ////////////////////////////////////////////////////

StringInstance::StringInstance(const std::string& value) : Instance("String") {
    if (!Grammar::is_string_value(value))
        throw EXC_INVALID_ARGUMENT;
    _value = str_utils::unescape(value.substr(1, int(value.size()) - 2), '"');
}

StringInstance::StringInstance(const std::string& message_name, const std::vector<std::string>& arguments) :
    Instance("String") {
    if (message_name == "create:" || message_name == "createIfNotExists:") {
        if (arguments.size() != 1)
            throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
        if (!Grammar::is_variable(arguments[0]))
            throw EXC_INVALID_ARGUMENT;
        _name = arguments[0];
        _value = "";
    }
    else if (message_name == "create:withValue:" || message_name == "createIfNotExists:withValue:") {
        if (arguments.size() != 2)
            throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
        if (!Grammar::is_variable(arguments[0]) || !Grammar::is_string_value(arguments[1]))
            throw EXC_INVALID_ARGUMENT;
        _name = arguments[0];
        _value = str_utils::unescape(arguments[1].substr(1, int(arguments[1].size()) - 2), '"');
    }
    else
        throw EXC_INVALID_MESSAGE;
}

StringInstance::~StringInstance() {
}

std::string StringInstance::value() const {
    return _value;
}

std::string StringInstance::representation() const {
    return "\"" + str_utils::escape(_value, '"') + "\"";
}

std::string StringInstance::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
    if (message_name == "get")
        return op_get(arguments);
    if (message_name == "set:")
        return op_set(arguments);
    if (message_name == "isEmpty?")
        return op_isEmpty(arguments);
    if (message_name == "startsWith?")
        return op_startsWith(arguments);
    if (message_name == "endsWith?")
        return op_endsWith(arguments);
    if (message_name == "isLexicographicallyEqualTo?")
        return op_isLexicographicallyEqualTo(arguments);
    if (message_name == "isLexicographicallyLessThan?")
        return op_isLexicographicallyLessThan(arguments);
    if (message_name == "isLexicographicallyLessThanOrEqualTo?")
        return op_isLexicographicallyLessThanOrEqualTo(arguments);
    if (message_name == "isLexicographicallyGreaterThan?")
        return op_isLexicographicallyGreaterThan(arguments);
    if (message_name == "isLexicographicallyGreaterThanOrEqualTo?")
        return op_isLexicographicallyGreaterThanOrEqualTo(arguments);
    if (message_name == "length")
        return op_length(arguments);
    if (message_name == "atIndex:")
        return op_atIndex(arguments);
    if (message_name == "first:")
        return op_first(arguments);
    if (message_name == "last:")
        return op_last(arguments);
    if (message_name == "substringFromIndex:")
        return op_substringFromIndex(arguments);
    if (message_name == "substringFromIndex:toIndex:")
        return op_substringFromIndex_toIndex(arguments);
    if (message_name == "pushBack:")
        return op_pushBack(arguments);
    if (message_name == "pushFront:")
        return op_pushFront(arguments);
    if (message_name == "popBack")
        return op_popBack(arguments);
    if (message_name == "popFront")
        return op_popFront(arguments);
    if (message_name == "popAtIndex:")
        return op_popAtIndex(arguments);
    if (message_name == "concatenate:")
        return op_concatenate(arguments);
    if (message_name == "insert:beforeIndex:")
        return op_insert_beforeIndex(arguments);
    if (message_name == "eraseFromIndex:")
        return op_eraseFromIndex(arguments);
    if (message_name == "eraseFromIndex:toIndex:")
        return op_eraseFromIndex_toIndex(arguments);
    if (message_name == "clear")
        return op_clear(arguments);
    throw EXC_INVALID_MESSAGE;
}

std::string StringInstance::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string StringInstance::op_set(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value = str_utils::unescape(arguments[0].substr(1, int(arguments[0].size()) - 2), '"');
    return "null";
}

std::string StringInstance::op_isEmpty(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (_value.size() == 0) ? "true" : "false";
}

std::string StringInstance::op_startsWith(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return str_utils::starts_with(_value, StringInstance(arguments[0]).value()) ? "true" : "false";
}

std::string StringInstance::op_endsWith(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return str_utils::ends_with(_value, StringInstance(arguments[0]).value()) ? "true" : "false";
}

std::string StringInstance::op_isLexicographicallyEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsEqualToComparator()(shared_from_this(), std::make_shared<StringInstance>(arguments[0])) ?
           "true" : "false";
}

std::string StringInstance::op_isLexicographicallyLessThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsLessThanComparator()(shared_from_this(), std::make_shared<StringInstance>(arguments[0])) ?
           "true" : "false";
}

std::string StringInstance::op_isLexicographicallyLessThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsLessThanOrEqualToComparator()(shared_from_this(), std::make_shared<StringInstance>(arguments[0])) ?
           "true" : "false";
}

std::string StringInstance::op_isLexicographicallyGreaterThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsGreaterThanComparator()(shared_from_this(), std::make_shared<StringInstance>(arguments[0])) ?
           "true" : "false";
}

std::string StringInstance::op_isLexicographicallyGreaterThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return InstanceIsGreaterThanOrEqualToComparator()(shared_from_this(),
                                                      std::make_shared<StringInstance>(arguments[0])) ?
           "true" : "false";
}

std::string StringInstance::op_length(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return std::to_string(int(_value.size()));
}

std::string StringInstance::op_atIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int index = IntegerInstance(arguments[0]).value();
    if (index < 0 || index >= int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    return "'" + std::string(1, _value[index]) + "'";
}

std::string StringInstance::op_first(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int n = IntegerInstance(arguments[0]).value();
    if (n < 0)
        throw EXC_INVALID_ARGUMENT;
    n = std::min(n, int(_value.size()));
    return "\"" + str_utils::escape(_value.substr(0, n), '"') + "\"";
}

std::string StringInstance::op_last(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int n = IntegerInstance(arguments[0]).value();
    if (n < 0)
        throw EXC_INVALID_ARGUMENT;
    n = std::min(n, int(_value.size()));
    return "\"" + str_utils::escape(_value.substr(int(_value.size()) - n), '"') + "\"";
}

std::string StringInstance::op_substringFromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = IntegerInstance(arguments[0]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    return "\"" + str_utils::escape(_value.substr(from_index), '"') + "\"";
}

std::string StringInstance::op_substringFromIndex_toIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = IntegerInstance(arguments[0]).value();
    int to_index = IntegerInstance(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index < 0 || to_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index - from_index < 0)
        throw EXC_INVALID_ARGUMENT;
    return "\"" + str_utils::escape(_value.substr(from_index, to_index - from_index), '"') + "\"";
}

std::string StringInstance::op_pushBack(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value += std::string(1, arguments[0][1]);
    return "null";
}

std::string StringInstance::op_pushFront(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value = std::string(1, arguments[0][1]) + _value;
    return "null";
}

std::string StringInstance::op_popBack(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (_value.size() == 0)
        return "null";
    char back = _value[int(_value.size()) - 1];
    _value = _value.substr(0, int(_value.size()) - 1);
    return "'" + std::string(1, back) + "'";
}

std::string StringInstance::op_popFront(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (_value.size() == 0)
        return "null";
    char front = _value[0];
    _value = _value.substr(1, int(_value.size()) - 1);
    return "'" + std::string(1, front) + "'";
}

std::string StringInstance::op_popAtIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int index = IntegerInstance(arguments[0]).value();
    if (index < 0 || index >= int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    char c = _value[index];
    _value.erase(_value.begin() + index);
    return "'" + std::string(1, c) + "'";
}

std::string StringInstance::op_concatenate(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value += StringInstance(arguments[0]).value();
    return "null";
}

std::string StringInstance::op_insert_beforeIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    int index = IntegerInstance(arguments[1]).value();
    if (index < 0 || index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    _value.insert(index, StringInstance(arguments[0]).value());
    return "null";
}

std::string StringInstance::op_eraseFromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = IntegerInstance(arguments[0]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    _value.erase(from_index);
    return "null";
}

std::string StringInstance::op_eraseFromIndex_toIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = IntegerInstance(arguments[0]).value();
    int to_index = IntegerInstance(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index < 0 || to_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index - from_index < 0)
        throw EXC_INVALID_ARGUMENT;
    _value.erase(from_index, to_index - from_index);
    return "null";
}

std::string StringInstance::op_clear(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    _value.clear();
    return "null";
}

//////////////////////////////////////////////////// VectorInstance ////////////////////////////////////////////////////

VectorInstance::VectorInstance(const std::string& element_type, const std::string& message_name,
                               const std::vector<std::string>& arguments) : Instance("Vector") {
    _element_type = element_type;
    if (message_name == "create:" || message_name == "createIfNotExists:") {
        if (arguments.size() != 1)
            throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
        if (!Grammar::is_variable(arguments[0]))
            throw EXC_INVALID_ARGUMENT;
        _name = arguments[0];
    }
    else
        throw EXC_INVALID_MESSAGE;
}

VectorInstance::~VectorInstance() {
}

std::string VectorInstance::representation() const {
    return vector_representation(_value);
}

std::string VectorInstance::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
    if (message_name == "get")
        return op_get(arguments);
    if (message_name == "isEmpty?")
        return op_isEmpty(arguments);
    if (message_name == "contains?")
        return op_contains(arguments);
    if (message_name == "contains?fromIndex:")
        return op_contains_fromIndex(arguments);
    if (message_name == "count:")
        return op_count(arguments);
    if (message_name == "atIndex:")
        return op_atIndex(arguments);
    if (message_name == "firstIndexOf:")
        return op_firstIndexOf(arguments);
    if (message_name == "firstIndexOf:fromIndex:")
        return op_firstIndexOf_fromIndex(arguments);
    if (message_name == "allIndexesOf:")
        return op_allIndexesOf(arguments);
    if (message_name == "size")
        return op_size(arguments);
    if (message_name == "first:")
        return op_first(arguments);
    if (message_name == "last:")
        return op_last(arguments);
    if (message_name == "sliceFromIndex:")
        return op_sliceFromIndex(arguments);
    if (message_name == "sliceFromIndex:toIndex:")
        return op_sliceFromIndex_toIndex(arguments);
    if (message_name == "pushBack:")
        return op_pushBack(arguments);
    if (message_name == "pushFront:")
        return op_pushFront(arguments);
    if (message_name == "popBack")
        return op_popBack(arguments);
    if (message_name == "popFront")
        return op_popFront(arguments);
    if (message_name == "popAtIndex:")
        return op_popAtIndex(arguments);
    if (message_name == "insert:beforeIndex:")
        return op_insert_beforeIndex(arguments);
    if (message_name == "eraseFromIndex:")
        return op_eraseFromIndex(arguments);
    if (message_name == "eraseFromIndex:toIndex:")
        return op_eraseFromIndex_toIndex(arguments);
    if (message_name == "removeFirst:")
        return op_removeFirst(arguments);
    if (message_name == "removeFirst:fromIndex:")
        return op_removeFirst_fromIndex(arguments);
    if (message_name == "removeAll:")
        return op_removeAll(arguments);
    if (message_name == "replaceFirst:with:")
        return op_replaceFirst_with(arguments);
    if (message_name == "replaceFirst:fromIndex:with:")
        return op_replaceFirst_fromIndex_with(arguments);
    if (message_name == "replaceAll:with:")
        return op_replaceAll_with(arguments);
    if (message_name == "sort")
        return op_sort(arguments);
    if (message_name == "reverse")
        return op_reverse(arguments);
    if (message_name == "clear")
        return op_clear(arguments);
    throw EXC_INVALID_MESSAGE;
}

std::string VectorInstance::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string VectorInstance::op_isEmpty(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (_value.size() == 0) ? "true" : "false";
}

std::string VectorInstance::op_contains(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    for (auto it = _value.begin(); it != _value.end(); it++)
        if (InstanceIsEqualToComparator()(*it, instance))
            return "true";
    return "false";
}

std::string VectorInstance::op_contains_fromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    int from_index = IntegerInstance(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    for (auto it = _value.begin() + from_index; it != _value.end(); it++)
        if (InstanceIsEqualToComparator()(*it, instance))
            return "true";
    return "false";
}

std::string VectorInstance::op_count(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    int count = 0;
    for (auto it = _value.begin(); it != _value.end(); it++)
        if (InstanceIsEqualToComparator()(*it, instance))
            count++;
    return IntegerInstance(count).representation();
}

std::string VectorInstance::op_atIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int index = IntegerInstance(arguments[0]).value();
    if (index < 0 || index >= int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    return _value[index]->representation();
}

std::string VectorInstance::op_firstIndexOf(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    for (auto it = _value.begin(); it != _value.end(); it++)
        if (InstanceIsEqualToComparator()(*it, instance))
            return IntegerInstance(it - _value.begin()).representation();
    return "-1";
}

std::string VectorInstance::op_firstIndexOf_fromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    int from_index = IntegerInstance(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    for (auto it = _value.begin() + from_index; it != _value.end(); it++)
        if (InstanceIsEqualToComparator()(*it, instance))
            return IntegerInstance(it - _value.begin()).representation();
    return "-1";
}

std::string VectorInstance::op_allIndexesOf(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    std::string indexes_str;
    for (auto it = _value.begin(); it != _value.end(); it++)
        if (InstanceIsEqualToComparator()(*it, instance)) {
            if (indexes_str.size() > 0)
                indexes_str += ",";
            indexes_str += std::to_string(it - _value.begin());
        }
    return "[" + indexes_str + "]";
}

std::string VectorInstance::op_size(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return IntegerInstance(int(_value.size())).representation();
}

std::string VectorInstance::op_first(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int n = IntegerInstance(arguments[0]).value();
    if (n < 0)
        throw EXC_INVALID_ARGUMENT;
    n = std::min(n, int(_value.size()));
    return vector_representation(std::vector<std::shared_ptr<Instance>>(_value.begin(), _value.begin() + n));
}

std::string VectorInstance::op_last(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int n = IntegerInstance(arguments[0]).value();
    if (n < 0)
        throw EXC_INVALID_ARGUMENT;
    n = std::min(n, int(_value.size()));
    return vector_representation(std::vector<std::shared_ptr<Instance>>(_value.end() - n, _value.end()));
}

std::string VectorInstance::op_sliceFromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int index = IntegerInstance(arguments[0]).value();
    if (index < 0 || index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    return vector_representation(std::vector<std::shared_ptr<Instance>>(_value.begin() + index, _value.end()));
}

std::string VectorInstance::op_sliceFromIndex_toIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = IntegerInstance(arguments[0]).value();
    int to_index = IntegerInstance(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index < 0 || to_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index - from_index < 0)
        throw EXC_INVALID_ARGUMENT;
    return vector_representation(std::vector<std::shared_ptr<Instance>>(_value.begin() + from_index,
                                                                        _value.begin() + to_index));
}

std::string VectorInstance::op_pushBack(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    _value.push_back(instance);
    return "null";
}

std::string VectorInstance::op_pushFront(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    _value.insert(_value.begin(), instance);
    return "null";
}

std::string VectorInstance::op_popBack(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (_value.size() == 0)
        return "null";
    std::shared_ptr<Instance> instance = _value[int(_value.size()) - 1];
    _value.pop_back();
    return instance->representation();
}

std::string VectorInstance::op_popFront(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (_value.size() == 0)
        return "null";
    std::shared_ptr<Instance> instance = _value[0];
    _value.erase(_value.begin());
    return instance->representation();
}

std::string VectorInstance::op_popAtIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int index = IntegerInstance(arguments[0]).value();
    if (index < 0 || index >= int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    std::shared_ptr<Instance> instance = _value[index];
    _value.erase(_value.begin() + index);
    return instance->representation();
}

std::string VectorInstance::op_insert_beforeIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    int index = IntegerInstance(arguments[1]).value();
    if (index < 0 || index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    _value.insert(_value.begin() + index, instance);
    return "null";
}

std::string VectorInstance::op_eraseFromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = IntegerInstance(arguments[0]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    _value.erase(_value.begin() + from_index, _value.end());
    return "null";
}

std::string VectorInstance::op_eraseFromIndex_toIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = IntegerInstance(arguments[0]).value();
    int to_index = IntegerInstance(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index < 0 || to_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index - from_index < 0)
        throw EXC_INVALID_ARGUMENT;
    _value.erase(_value.begin() + from_index, _value.begin() + to_index);
    return "null";
}

std::string VectorInstance::op_removeFirst(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    for (auto it = _value.begin(); it != _value.end(); it++)
        if (InstanceIsEqualToComparator()(*it, instance)) {
            _value.erase(it);
            break;
        }
    return "null";
}

std::string VectorInstance::op_removeFirst_fromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    int from_index = IntegerInstance(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    for (auto it = _value.begin() + from_index; it != _value.end(); it++)
        if (InstanceIsEqualToComparator()(*it, instance)) {
            _value.erase(it);
            break;
        }
    return "null";
}

std::string VectorInstance::op_removeAll(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    for (int i = 0; i < int(_value.size()); i++)
        if (InstanceIsEqualToComparator()(_value[i], instance)) {
            _value.erase(_value.begin() + i);
            i--;
        }
    return "null";
}

std::string VectorInstance::op_replaceFirst_with(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    std::shared_ptr<Instance> temporary = get_shared_instance(_element_type, arguments[1]);
    for (int i = 0; i < int(_value.size()); i++)
        if (InstanceIsEqualToComparator()(_value[i], instance)) {
            _value[i] = get_shared_instance(_element_type, arguments[1]);
            break;
        }
    return "null";
}

std::string VectorInstance::op_replaceFirst_fromIndex_with(const std::vector<std::string>& arguments) {
    if (arguments.size() != 3)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    std::shared_ptr<Instance> temporary = get_shared_instance(_element_type, arguments[2]);
    int from_index = IntegerInstance(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    for (int i = from_index; i < int(_value.size()); i++)
        if (InstanceIsEqualToComparator()(_value[i], instance)) {
            _value[i] = get_shared_instance(_element_type, arguments[2]);
            break;
        }
    return "null";
}

std::string VectorInstance::op_replaceAll_with(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    std::shared_ptr<Instance> temporary = get_shared_instance(_element_type, arguments[1]);
    for (int i = 0; i < int(_value.size()); i++)
        if (InstanceIsEqualToComparator()(_value[i], instance))
            _value[i] = get_shared_instance(_element_type, arguments[1]);
    return "null";
}

std::string VectorInstance::op_sort(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::sort(_value.begin(), _value.end(), InstanceIsLessThanComparator());
    return "null";
}

std::string VectorInstance::op_reverse(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::reverse(_value.begin(), _value.end());
    return "null";
}

std::string VectorInstance::op_clear(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    _value.clear();
    return "null";
}

////////////////////////////////////////////////////// SetInstance /////////////////////////////////////////////////////

SetInstance::SetInstance(const std::string& element_type, const std::string& message_name,
                         const std::vector<std::string>& arguments) : Instance("Set") {
    _element_type = element_type;
    if (message_name == "create:" || message_name == "createIfNotExists:") {
        if (arguments.size() != 1)
            throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
        if (!Grammar::is_variable(arguments[0]))
            throw EXC_INVALID_ARGUMENT;
        _name = arguments[0];
    }
    else
        throw EXC_INVALID_MESSAGE;
}

SetInstance::~SetInstance() {
}

std::string SetInstance::representation() const {
    std::string elements_str;
    for (auto it = _value.begin(); it != _value.end(); it++) {
        if (it != _value.begin())
            elements_str += ",";
        elements_str += (*it)->representation();
    }
    return "{" + elements_str + "}";
}

std::string SetInstance::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
    if (message_name == "get")
        return op_get(arguments);
    if (message_name == "isEmpty?")
        return op_isEmpty(arguments);
    if (message_name == "contains?")
        return op_contains(arguments);
    if (message_name == "size")
        return op_size(arguments);
    if (message_name == "add:")
        return op_add(arguments);
    if (message_name == "remove:")
        return op_remove(arguments);
    if (message_name == "clear")
        return op_clear(arguments);
    throw EXC_INVALID_MESSAGE;
}

std::string SetInstance::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string SetInstance::op_isEmpty(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return _value.empty() ? "true" : "false";
}

std::string SetInstance::op_contains(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    return (_value.find(instance) != _value.end()) ? "true" : "false";
}

std::string SetInstance::op_size(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return IntegerInstance(int(_value.size())).representation();
}

std::string SetInstance::op_add(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    _value.insert(instance);
    return "null";
}

std::string SetInstance::op_remove(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> instance = get_shared_instance(_element_type, arguments[0]);
    _value.erase(instance);
    return "null";
}

std::string SetInstance::op_clear(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    _value.clear();
    return "null";
}

////////////////////////////////////////////////// DictionaryInstance //////////////////////////////////////////////////

DictionaryInstance::DictionaryInstance(const std::string& key_type, const std::string& value_type,
                                       const std::string& message_name, const std::vector<std::string>& arguments) :
    Instance("Dictionary") {
    _key_type = key_type;
    _value_type = value_type;
    if (message_name == "create:" || message_name == "createIfNotExists:") {
        if (arguments.size() != 1)
            throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
        if (!Grammar::is_variable(arguments[0]))
            throw EXC_INVALID_ARGUMENT;
        _name = arguments[0];
    }
    else
        throw EXC_INVALID_MESSAGE;
}

DictionaryInstance::~DictionaryInstance() {
}

std::string DictionaryInstance::representation() const {
    std::string elements_str;
    for (auto it = _value.begin(); it != _value.end(); it++) {
        if (it != _value.begin())
            elements_str += ",";
        elements_str += "(" + it->first->representation() + "," + it->second->representation() + ")";
    }
    return "(" + elements_str + ")";
}

std::string DictionaryInstance::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
    if (message_name == "get")
        return op_get(arguments);
    if (message_name == "isEmpty?")
        return op_isEmpty(arguments);
    if (message_name == "containsKey?")
        return op_containsKey(arguments);
    if (message_name == "size")
        return op_size(arguments);
    if (message_name == "keys")
        return op_keys(arguments);
    if (message_name == "values")
        return op_values(arguments);
    if (message_name == "getValueForKey:")
        return op_getValueForKey(arguments);
    if (message_name == "associateValue:withKey:")
        return op_associateValue_withKey(arguments);
    if (message_name == "removeKey:")
        return op_removeKey(arguments);
    if (message_name == "clear")
        return op_clear(arguments);
    throw EXC_INVALID_MESSAGE;
}

std::string DictionaryInstance::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string DictionaryInstance::op_isEmpty(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return _value.empty() ? "true" : "false";
}

std::string DictionaryInstance::op_containsKey(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> key = get_shared_instance(_key_type, arguments[0]);
    return (_value.find(key) != _value.end()) ? "true" : "false";
}

std::string DictionaryInstance::op_size(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return IntegerInstance(int(_value.size())).representation();
}

std::string DictionaryInstance::op_keys(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::vector<std::shared_ptr<Instance>> keys;
    for (auto it = _value.begin(); it != _value.end(); it++)
        keys.push_back(it->first);
    return vector_representation(keys);
}

std::string DictionaryInstance::op_values(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::vector<std::shared_ptr<Instance>> values;
    for (auto it = _value.begin(); it != _value.end(); it++)
        values.push_back(it->second);
    return vector_representation(values);
}

std::string DictionaryInstance::op_getValueForKey(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> key = get_shared_instance(_key_type, arguments[0]);
    auto it = _value.find(key);
    if (it != _value.end())
        return it->second->representation();
    return "null";
}

std::string DictionaryInstance::op_associateValue_withKey(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> value = get_shared_instance(_value_type, arguments[0]);
    std::shared_ptr<Instance> key = get_shared_instance(_key_type, arguments[1]);
    _value[key] = value;
    return "null";
}

std::string DictionaryInstance::op_removeKey(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Instance> key = get_shared_instance(_key_type, arguments[0]);
    _value.erase(key);
    return "null";
}

std::string DictionaryInstance::op_clear(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    _value.clear();
    return "null";
}
