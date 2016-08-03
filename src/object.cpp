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
#include "object.h"
#include "str_utils.h"

/////////////////////////////////////////////////////// Auxiliar ///////////////////////////////////////////////////////
    
std::shared_ptr<Object> get_shared_object(const std::string& type, const std::string& value) {
    std::shared_ptr<Object> object;
    if (type == "Boolean")
        object = std::make_shared<Boolean>(value);
    if (type == "Character")
        object = std::make_shared<Character>(value);
    if (type == "Integer")
        object = std::make_shared<Integer>(value);
    if (type == "Float")
        object = std::make_shared<Float>(value);
    if (type == "String")
        object = std::make_shared<String>(value);
    return object;
}

std::string vector_representation(const std::vector<std::shared_ptr<Object>>& vector) {
    std::string elements_str;
    for (auto it = vector.begin(); it != vector.end(); it++) {
        if (it != vector.begin())
            elements_str += ",";
        elements_str += (*it)->representation();
    }
    return "[" + elements_str + "]";
}

//////////////////////////////////////////////////////// Object ////////////////////////////////////////////////////////

Object::Object(const std::string& type) : _type(type) {
}

Object::~Object() {
}

std::string Object::type() const {
    return _type;
}

std::string Object::name() const {
    return _name;
}

////////////////////////////////////////////// ObjectIsEqualToComparator ///////////////////////////////////////////////

bool ObjectIsEqualToComparator::operator() (const std::shared_ptr<Object>& lhs,
                                            const std::shared_ptr<Object>& rhs) const {
    return !ObjectIsLessThanComparator()(lhs, rhs) && !ObjectIsGreaterThanComparator()(lhs, rhs);
}

////////////////////////////////////////////// ObjectIsLessThanComparator //////////////////////////////////////////////

bool ObjectIsLessThanComparator::operator()(const std::shared_ptr<Object>& lhs,
                                            const std::shared_ptr<Object>& rhs) const {
    const std::string lhs_type = lhs->type();
    const std::string rhs_type = rhs->type();
    float eps = Context::get_instance()->get_float_comparison_tolerance();
    if (lhs_type == "Boolean" && rhs_type == "Boolean")
        return static_cast<const Boolean&>(*lhs).value() < static_cast<const Boolean&>(*rhs).value();
    if (lhs_type == "Character" && rhs_type == "Character")
        return static_cast<const Character&>(*lhs).value() < static_cast<const Character&>(*rhs).value();
    if (lhs_type == "Integer" && rhs_type == "Integer")
        return static_cast<const Integer&>(*lhs).value() < static_cast<const Integer&>(*rhs).value();
    if (lhs_type == "Integer" && rhs_type == "Float")
        return float(static_cast<const Integer&>(*lhs).value()) - static_cast<const Float&>(*rhs).value() < -eps;
    if (lhs_type == "Float" && rhs_type == "Float")
        return static_cast<const Float&>(*lhs).value() - static_cast<const Float&>(*rhs).value() < -eps;
    if (lhs_type == "Float" && rhs_type == "Integer")
        return static_cast<const Float&>(*lhs).value() - float(static_cast<const Integer&>(*rhs).value()) < -eps;
    if (lhs_type == "String" && rhs_type == "String")
        return static_cast<const String&>(*lhs).value() < static_cast<const String&>(*rhs).value();
    throw EXC_INVALID_COMPARISON;
}

////////////////////////////////////////// ObjectIsLessThanOrEqualToComparator /////////////////////////////////////////

bool ObjectIsLessThanOrEqualToComparator::operator()(const std::shared_ptr<Object>& lhs,
                                                     const std::shared_ptr<Object>& rhs) const {
    return ObjectIsLessThanComparator()(lhs, rhs) || ObjectIsEqualToComparator()(lhs, rhs);
}

//////////////////////////////////////////// ObjectIsGreaterThanComparator /////////////////////////////////////////////

bool ObjectIsGreaterThanComparator::operator()(const std::shared_ptr<Object>& lhs,
                                               const std::shared_ptr<Object>& rhs) const {
    const std::string lhs_type = lhs->type();
    const std::string rhs_type = rhs->type();
    float eps = Context::get_instance()->get_float_comparison_tolerance();
    if (lhs_type == "Boolean" && rhs_type == "Boolean")
        return static_cast<const Boolean&>(*lhs).value() > static_cast<const Boolean&>(*rhs).value();
    if (lhs_type == "Character" && rhs_type == "Character")
        return static_cast<const Character&>(*lhs).value() > static_cast<const Character&>(*rhs).value();
    if (lhs_type == "Integer" && rhs_type == "Integer")
        return static_cast<const Integer&>(*lhs).value() > static_cast<const Integer&>(*rhs).value();
    if (lhs_type == "Integer" && rhs_type == "Float")
        return float(static_cast<const Integer&>(*lhs).value()) - static_cast<const Float&>(*rhs).value() > eps;
    if (lhs_type == "Float" && rhs_type == "Float")
        return static_cast<const Float&>(*lhs).value() - static_cast<const Float&>(*rhs).value() > eps;
    if (lhs_type == "Float" && rhs_type == "Integer")
        return static_cast<const Float&>(*lhs).value() - float(static_cast<const Integer&>(*rhs).value()) > eps;
    if (lhs_type == "String" && rhs_type == "String")
        return static_cast<const String&>(*lhs).value() > static_cast<const String&>(*rhs).value();
    throw EXC_INVALID_COMPARISON;
}

//////////////////////////////////////// ObjectIsGreaterThanOrEqualToComparator ////////////////////////////////////////

bool ObjectIsGreaterThanOrEqualToComparator::operator()(const std::shared_ptr<Object>& lhs,
                                                        const std::shared_ptr<Object>& rhs) const {
    return ObjectIsGreaterThanComparator()(lhs, rhs) || ObjectIsEqualToComparator()(lhs, rhs);
}

/////////////////////////////////////////////////////// Boolean ////////////////////////////////////////////////////////

Boolean::Boolean(const std::string& value) : Object("Boolean") {
    if (!Grammar::is_boolean_value(value))
        throw EXC_INVALID_ARGUMENT;
    _value = (value == "true");
}

Boolean::Boolean(const std::string& message_name, const std::vector<std::string>& arguments) : Object("Boolean") {
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

Boolean::~Boolean() {
}

bool Boolean::value() const {
    return _value;
}

std::string Boolean::representation() const {
    return (_value == true) ? "true" : "false";
}

std::string Boolean::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
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

std::string Boolean::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string Boolean::op_set(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_boolean_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value = (arguments[0] == "true");
    return "null";
}

std::string Boolean::op_isTrue(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (_value == true) ? "true" : "false";
}

std::string Boolean::op_isFalse(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (_value == false) ? "true" : "false";
}

////////////////////////////////////////////////////// Character ///////////////////////////////////////////////////////

Character::Character(const std::string& value) : Object("Character") {
    if (!Grammar::is_character_value(value))
        throw EXC_INVALID_ARGUMENT;
    _value = value[1];
}

Character::Character(const std::string& message_name, const std::vector<std::string>& arguments) : Object("Character") {
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

Character::~Character() {
}

char Character::value() const {
    return _value;
}

std::string Character::representation() const {
    return "'" + std::string(1, _value) + "'";
}

std::string Character::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
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

std::string Character::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string Character::op_set(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value = arguments[0][1];
    return "null";
}

std::string Character::op_isAlphabetic(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (str_utils::is_alpha(_value)) ? "true" : "false";
}

std::string Character::op_isNumeric(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (str_utils::is_numeric(_value)) ? "true" : "false";
}

std::string Character::op_isAlphanumeric(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (str_utils::is_alphanumeric(_value)) ? "true" : "false";
}

std::string Character::op_isSpace(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (str_utils::is_space(_value)) ? "true" : "false";
}

std::string Character::op_isEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsEqualToComparator()(shared_from_this(), std::make_shared<Character>(arguments[0])) ?
           "true" : "false";
}

std::string Character::op_isLessThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsLessThanComparator()(shared_from_this(), std::make_shared<Character>(arguments[0])) ?
           "true" : "false";
}

std::string Character::op_isLessThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsLessThanOrEqualToComparator()(shared_from_this(), std::make_shared<Character>(arguments[0])) ?
           "true" : "false";
}

std::string Character::op_isGreaterThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsGreaterThanComparator()(shared_from_this(), std::make_shared<Character>(arguments[0])) ?
           "true" : "false";
}

std::string Character::op_isGreaterThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsGreaterThanOrEqualToComparator()(shared_from_this(), std::make_shared<Character>(arguments[0])) ?
           "true" : "false";
}

/////////////////////////////////////////////////////// Integer ////////////////////////////////////////////////////////

Integer::Integer(int value) : Object("Integer") {
    _value = value;
}

Integer::Integer(const std::string& value) : Object("Integer") {
    if (!Grammar::is_integer_value(value) && !Grammar::is_float_value(value))
        throw EXC_INVALID_ARGUMENT;
    _value = (Grammar::is_integer_value(value)) ? atoi(value.c_str()) : atof(value.c_str());
}

Integer::Integer(const std::string& message_name, const std::vector<std::string>& arguments) : Object("Integer") {
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

Integer::~Integer() {
}

int Integer::value() const {
    return _value;
}

std::string Integer::representation() const {
    return std::to_string(_value);
}

std::string Integer::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
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

std::string Integer::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string Integer::op_set(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value = (Grammar::is_integer_value(arguments[0])) ? atoi(arguments[0].c_str()) : atof(arguments[0].c_str());
    return "null";
}

std::string Integer::op_isEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    if (Grammar::is_integer_value(arguments[0]))
        return ObjectIsEqualToComparator()(shared_from_this(), std::make_shared<Integer>(arguments[0])) ?
               "true" : "false";
    return ObjectIsEqualToComparator()(shared_from_this(), std::make_shared<Float>(arguments[0])) ? "true" : "false";
}

std::string Integer::op_isLessThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    if (Grammar::is_integer_value(arguments[0]))
        return ObjectIsLessThanComparator()(shared_from_this(), std::make_shared<Integer>(arguments[0])) ?
               "true" : "false";
    return ObjectIsLessThanComparator()(shared_from_this(), std::make_shared<Float>(arguments[0])) ? "true" : "false";
}

std::string Integer::op_isLessThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    if (Grammar::is_integer_value(arguments[0]))
        return ObjectIsLessThanOrEqualToComparator()(shared_from_this(), std::make_shared<Integer>(arguments[0])) ?
               "true" : "false";
    return ObjectIsLessThanOrEqualToComparator()(shared_from_this(), std::make_shared<Float>(arguments[0])) ?
           "true" : "false";
}

std::string Integer::op_isGreaterThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    if (Grammar::is_integer_value(arguments[0]))
        return ObjectIsGreaterThanComparator()(shared_from_this(), std::make_shared<Integer>(arguments[0])) ?
               "true" : "false";
    return ObjectIsGreaterThanComparator()(shared_from_this(), std::make_shared<Float>(arguments[0])) ?
           "true" : "false";
}

std::string Integer::op_isGreaterThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    if (Grammar::is_integer_value(arguments[0]))
        return ObjectIsGreaterThanOrEqualToComparator()(shared_from_this(), std::make_shared<Integer>(arguments[0])) ?
               "true" : "false";
    return ObjectIsGreaterThanOrEqualToComparator()(shared_from_this(), std::make_shared<Float>(arguments[0])) ?
           "true" : "false";
}

std::string Integer::op_add(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT; 
    _value += (Grammar::is_integer_value(arguments[0])) ? atoi(arguments[0].c_str()) : atof(arguments[0].c_str());
    return "null";
}

std::string Integer::op_subtract(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value -= (Grammar::is_integer_value(arguments[0])) ? atoi(arguments[0].c_str()) : atof(arguments[0].c_str());
    return "null";
}

std::string Integer::op_multiplyBy(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT; 
    _value *= (Grammar::is_integer_value(arguments[0])) ? atoi(arguments[0].c_str()) : atof(arguments[0].c_str());
    return "null";
}

std::string Integer::op_divideBy(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_integer_value(arguments[0]) && !Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT; 
    _value /= (Grammar::is_integer_value(arguments[0])) ? atoi(arguments[0].c_str()) : atof(arguments[0].c_str());
    return "null";
}

//////////////////////////////////////////////////////// Float /////////////////////////////////////////////////////////

Float::Float(float value) : Object("Float") {
    _value = value;
}

Float::Float(const std::string& value) : Object("Float") {
    if (!Grammar::is_float_value(value))
        throw EXC_INVALID_ARGUMENT;
    _value = atof(value.c_str());
}

Float::Float(const std::string& message_name, const std::vector<std::string>& arguments) : Object("Float") {
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

Float::~Float() {
}

float Float::value() const {
    return _value;
}

std::string Float::representation() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(Context::get_instance()->get_float_precision()) << _value;
    return oss.str();
}

std::string Float::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
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

std::string Float::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string Float::op_set(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value = atof(arguments[0].c_str());
    return "null";
}

std::string Float::op_isEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsEqualToComparator()(shared_from_this(), std::make_shared<Float>(arguments[0])) ? "true" : "false";
}

std::string Float::op_isLessThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsLessThanComparator()(shared_from_this(), std::make_shared<Float>(arguments[0])) ? "true" : "false";
}

std::string Float::op_isLessThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsLessThanOrEqualToComparator()(shared_from_this(), std::make_shared<Float>(arguments[0])) ?
           "true" : "false";
}

std::string Float::op_isGreaterThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsGreaterThanComparator()(shared_from_this(), std::make_shared<Float>(arguments[0])) ?
           "true" : "false";
}

std::string Float::op_isGreaterThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsGreaterThanOrEqualToComparator()(shared_from_this(), std::make_shared<Float>(arguments[0])) ?
           "true" : "false";
}

std::string Float::op_add(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT; 
    _value += atof(arguments[0].c_str());
    return "null";
}

std::string Float::op_subtract(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value -= atof(arguments[0].c_str());
    return "null";
}

std::string Float::op_multiplyBy(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT; 
    _value *= atof(arguments[0].c_str());
    return "null";
}

std::string Float::op_divideBy(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_float_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT; 
    _value /= atof(arguments[0].c_str());
    return "null";
}

//////////////////////////////////////////////////////// String ////////////////////////////////////////////////////////

String::String(const std::string& value) : Object("String") {
    if (!Grammar::is_string_value(value))
        throw EXC_INVALID_ARGUMENT;
    _value = str_utils::unescape(value.substr(1, int(value.size()) - 2), '"');
}

String::String(const std::string& message_name, const std::vector<std::string>& arguments) : Object("String") {
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

String::~String() {
}

std::string String::value() const {
    return _value;
}

std::string String::representation() const {
    return "\"" + str_utils::escape(_value, '"') + "\"";
}

std::string String::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
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

std::string String::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string String::op_set(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value = str_utils::unescape(arguments[0].substr(1, int(arguments[0].size()) - 2), '"');
    return "null";
}

std::string String::op_isEmpty(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (_value.size() == 0) ? "true" : "false";
}

std::string String::op_startsWith(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return str_utils::starts_with(_value, String(arguments[0]).value()) ? "true" : "false";
}

std::string String::op_endsWith(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return str_utils::ends_with(_value, String(arguments[0]).value()) ? "true" : "false";
}

std::string String::op_isLexicographicallyEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsEqualToComparator()(shared_from_this(), std::make_shared<String>(arguments[0])) ? "true" : "false";
}

std::string String::op_isLexicographicallyLessThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsLessThanComparator()(shared_from_this(), std::make_shared<String>(arguments[0])) ? "true" : "false";
}

std::string String::op_isLexicographicallyLessThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsLessThanOrEqualToComparator()(shared_from_this(), std::make_shared<String>(arguments[0])) ?
           "true" : "false";
}

std::string String::op_isLexicographicallyGreaterThan(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsGreaterThanComparator()(shared_from_this(), std::make_shared<String>(arguments[0])) ?
           "true" : "false";
}

std::string String::op_isLexicographicallyGreaterThanOrEqualTo(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    return ObjectIsGreaterThanOrEqualToComparator()(shared_from_this(), std::make_shared<String>(arguments[0])) ?
           "true" : "false";
}

std::string String::op_length(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return std::to_string(int(_value.size()));
}

std::string String::op_atIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int index = Integer(arguments[0]).value();
    if (index < 0 || index >= int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    return "'" + std::string(1, _value[index]) + "'";
}

std::string String::op_first(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int n = Integer(arguments[0]).value();
    if (n < 0)
        throw EXC_INVALID_ARGUMENT;
    n = std::min(n, int(_value.size()));
    return "\"" + str_utils::escape(_value.substr(0, n), '"') + "\"";
}

std::string String::op_last(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int n = Integer(arguments[0]).value();
    if (n < 0)
        throw EXC_INVALID_ARGUMENT;
    n = std::min(n, int(_value.size()));
    return "\"" + str_utils::escape(_value.substr(int(_value.size()) - n), '"') + "\"";
}

std::string String::op_substringFromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = Integer(arguments[0]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    return "\"" + str_utils::escape(_value.substr(from_index), '"') + "\"";
}

std::string String::op_substringFromIndex_toIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = Integer(arguments[0]).value();
    int to_index = Integer(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index < 0 || to_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index - from_index < 0)
        throw EXC_INVALID_ARGUMENT;
    return "\"" + str_utils::escape(_value.substr(from_index, to_index - from_index), '"') + "\"";
}

std::string String::op_pushBack(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value += std::string(1, arguments[0][1]);
    return "null";
}

std::string String::op_pushFront(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_character_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value = std::string(1, arguments[0][1]) + _value;
    return "null";
}

std::string String::op_popBack(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (_value.size() == 0)
        return "null";
    char back = _value[int(_value.size()) - 1];
    _value = _value.substr(0, int(_value.size()) - 1);
    return "'" + std::string(1, back) + "'";
}

std::string String::op_popFront(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (_value.size() == 0)
        return "null";
    char front = _value[0];
    _value = _value.substr(1, int(_value.size()) - 1);
    return "'" + std::string(1, front) + "'";
}

std::string String::op_popAtIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int index = Integer(arguments[0]).value();
    if (index < 0 || index >= int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    char c = _value[index];
    _value.erase(_value.begin() + index);
    return "'" + std::string(1, c) + "'";
}

std::string String::op_concatenate(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    _value += String(arguments[0]).value();
    return "null";
}

std::string String::op_insert_beforeIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (!Grammar::is_string_value(arguments[0]))
        throw EXC_INVALID_ARGUMENT;
    int index = Integer(arguments[1]).value();
    if (index < 0 || index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    _value.insert(index, String(arguments[0]).value());
    return "null";
}

std::string String::op_eraseFromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = Integer(arguments[0]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    _value.erase(from_index);
    return "null";
}

std::string String::op_eraseFromIndex_toIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = Integer(arguments[0]).value();
    int to_index = Integer(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index < 0 || to_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index - from_index < 0)
        throw EXC_INVALID_ARGUMENT;
    _value.erase(from_index, to_index - from_index);
    return "null";
}

std::string String::op_clear(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    _value.clear();
    return "null";
}

//////////////////////////////////////////////////////// Vector ////////////////////////////////////////////////////////

Vector::Vector(const std::string& element_type, const std::string& message_name,
               const std::vector<std::string>& arguments) : Object("Vector") {
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

Vector::~Vector() {
}

std::string Vector::representation() const {
    return vector_representation(_value);
}

std::string Vector::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
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

std::string Vector::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string Vector::op_isEmpty(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return (_value.size() == 0) ? "true" : "false";
}

std::string Vector::op_contains(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    for (auto it = _value.begin(); it != _value.end(); it++)
        if (ObjectIsEqualToComparator()(*it, object))
            return "true";
    return "false";
}

std::string Vector::op_contains_fromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    int from_index = Integer(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    for (auto it = _value.begin() + from_index; it != _value.end(); it++)
        if (ObjectIsEqualToComparator()(*it, object))
            return "true";
    return "false";
}

std::string Vector::op_count(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    int count = 0;
    for (auto it = _value.begin(); it != _value.end(); it++)
        if (ObjectIsEqualToComparator()(*it, object))
            count++;
    return Integer(count).representation();
}

std::string Vector::op_atIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int index = Integer(arguments[0]).value();
    if (index < 0 || index >= int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    return _value[index]->representation();
}

std::string Vector::op_firstIndexOf(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    for (auto it = _value.begin(); it != _value.end(); it++)
        if (ObjectIsEqualToComparator()(*it, object))
            return Integer(it - _value.begin()).representation();
    return "-1";
}

std::string Vector::op_firstIndexOf_fromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    int from_index = Integer(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    for (auto it = _value.begin() + from_index; it != _value.end(); it++)
        if (ObjectIsEqualToComparator()(*it, object))
            return Integer(it - _value.begin()).representation();
    return "-1";
}

std::string Vector::op_allIndexesOf(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    std::string indexes_str;
    for (auto it = _value.begin(); it != _value.end(); it++)
        if (ObjectIsEqualToComparator()(*it, object)) {
            if (indexes_str.size() > 0)
                indexes_str += ",";
            indexes_str += std::to_string(it - _value.begin());
        }
    return "[" + indexes_str + "]";
}

std::string Vector::op_size(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return Integer(int(_value.size())).representation();
}

std::string Vector::op_first(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int n = Integer(arguments[0]).value();
    if (n < 0)
        throw EXC_INVALID_ARGUMENT;
    n = std::min(n, int(_value.size()));
    return vector_representation(std::vector<std::shared_ptr<Object>>(_value.begin(), _value.begin() + n));
}

std::string Vector::op_last(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int n = Integer(arguments[0]).value();
    if (n < 0)
        throw EXC_INVALID_ARGUMENT;
    n = std::min(n, int(_value.size()));
    return vector_representation(std::vector<std::shared_ptr<Object>>(_value.end() - n, _value.end()));
}

std::string Vector::op_sliceFromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int index = Integer(arguments[0]).value();
    if (index < 0 || index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    return vector_representation(std::vector<std::shared_ptr<Object>>(_value.begin() + index, _value.end()));
}

std::string Vector::op_sliceFromIndex_toIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = Integer(arguments[0]).value();
    int to_index = Integer(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index < 0 || to_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index - from_index < 0)
        throw EXC_INVALID_ARGUMENT;
    return vector_representation(std::vector<std::shared_ptr<Object>>(_value.begin() + from_index,
                                                                      _value.begin() + to_index));
}

std::string Vector::op_pushBack(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    _value.push_back(object);
    return "null";
}

std::string Vector::op_pushFront(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    _value.insert(_value.begin(), object);
    return "null";
}

std::string Vector::op_popBack(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (_value.size() == 0)
        return "null";
    std::shared_ptr<Object> object = _value[int(_value.size()) - 1];
    _value.pop_back();
    return object->representation();
}

std::string Vector::op_popFront(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    if (_value.size() == 0)
        return "null";
    std::shared_ptr<Object> object = _value[0];
    _value.erase(_value.begin());
    return object->representation();
}

std::string Vector::op_popAtIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int index = Integer(arguments[0]).value();
    if (index < 0 || index >= int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    std::shared_ptr<Object> object = _value[index];
    _value.erase(_value.begin() + index);
    return object->representation();
}

std::string Vector::op_insert_beforeIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    int index = Integer(arguments[1]).value();
    if (index < 0 || index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    _value.insert(_value.begin() + index, object);
    return "null";
}

std::string Vector::op_eraseFromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = Integer(arguments[0]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    _value.erase(_value.begin() + from_index, _value.end());
    return "null";
}

std::string Vector::op_eraseFromIndex_toIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    int from_index = Integer(arguments[0]).value();
    int to_index = Integer(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index < 0 || to_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    if (to_index - from_index < 0)
        throw EXC_INVALID_ARGUMENT;
    _value.erase(_value.begin() + from_index, _value.begin() + to_index);
    return "null";
}

std::string Vector::op_removeFirst(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    for (auto it = _value.begin(); it != _value.end(); it++)
        if (ObjectIsEqualToComparator()(*it, object)) {
            _value.erase(it);
            break;
        }
    return "null";
}

std::string Vector::op_removeFirst_fromIndex(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    int from_index = Integer(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    for (auto it = _value.begin() + from_index; it != _value.end(); it++)
        if (ObjectIsEqualToComparator()(*it, object)) {
            _value.erase(it);
            break;
        }
    return "null";
}

std::string Vector::op_removeAll(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    for (int i = 0; i < int(_value.size()); i++)
        if (ObjectIsEqualToComparator()(_value[i], object)) {
            _value.erase(_value.begin() + i);
            i--;
        }
    return "null";
}

std::string Vector::op_replaceFirst_with(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    std::shared_ptr<Object> temporary = get_shared_object(_element_type, arguments[1]);
    for (int i = 0; i < int(_value.size()); i++)
        if (ObjectIsEqualToComparator()(_value[i], object)) {
            _value[i] = get_shared_object(_element_type, arguments[1]);
            break;
        }
    return "null";
}

std::string Vector::op_replaceFirst_fromIndex_with(const std::vector<std::string>& arguments) {
    if (arguments.size() != 3)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    std::shared_ptr<Object> temporary = get_shared_object(_element_type, arguments[2]);
    int from_index = Integer(arguments[1]).value();
    if (from_index < 0 || from_index > int(_value.size()))
        throw EXC_INVALID_ARGUMENT;
    for (int i = from_index; i < int(_value.size()); i++)
        if (ObjectIsEqualToComparator()(_value[i], object)) {
            _value[i] = get_shared_object(_element_type, arguments[2]);
            break;
        }
    return "null";
}

std::string Vector::op_replaceAll_with(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    std::shared_ptr<Object> temporary = get_shared_object(_element_type, arguments[1]);
    for (int i = 0; i < int(_value.size()); i++)
        if (ObjectIsEqualToComparator()(_value[i], object))
            _value[i] = get_shared_object(_element_type, arguments[1]);
    return "null";
}

std::string Vector::op_sort(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::sort(_value.begin(), _value.end(), ObjectIsLessThanComparator());
    return "null";
}

std::string Vector::op_reverse(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::reverse(_value.begin(), _value.end());
    return "null";
}

std::string Vector::op_clear(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    _value.clear();
    return "null";
}

////////////////////////////////////////////////////////// Set /////////////////////////////////////////////////////////

Set::Set(const std::string& element_type, const std::string& message_name,
         const std::vector<std::string>& arguments) : Object("Set") {
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

Set::~Set() {
}

std::string Set::representation() const {
    std::string elements_str;
    for (auto it = _value.begin(); it != _value.end(); it++) {
        if (it != _value.begin())
            elements_str += ",";
        elements_str += (*it)->representation();
    }
    return "{" + elements_str + "}";
}

std::string Set::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
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

std::string Set::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string Set::op_isEmpty(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return _value.empty() ? "true" : "false";
}

std::string Set::op_contains(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    return (_value.find(object) != _value.end()) ? "true" : "false";
}

std::string Set::op_size(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return Integer(int(_value.size())).representation();
}

std::string Set::op_add(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    _value.insert(object);
    return "null";
}

std::string Set::op_remove(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> object = get_shared_object(_element_type, arguments[0]);
    _value.erase(object);
    return "null";
}

std::string Set::op_clear(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    _value.clear();
    return "null";
}

////////////////////////////////////////////////////// Dictionary //////////////////////////////////////////////////////

Dictionary::Dictionary(const std::string& key_type, const std::string& value_type, const std::string& message_name,
                       const std::vector<std::string>& arguments) : Object("Dictionary") {
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

Dictionary::~Dictionary() {
}

std::string Dictionary::representation() const {
    std::string elements_str;
    for (auto it = _value.begin(); it != _value.end(); it++) {
        if (it != _value.begin())
            elements_str += ",";
        elements_str += "(" + it->first->representation() + "," + it->second->representation() + ")";
    }
    return "(" + elements_str + ")";
}

std::string Dictionary::receive(const std::string& message_name, const std::vector<std::string>& arguments) {
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

std::string Dictionary::op_get(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return representation();
}

std::string Dictionary::op_isEmpty(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return _value.empty() ? "true" : "false";
}

std::string Dictionary::op_containsKey(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> key = get_shared_object(_key_type, arguments[0]);
    return (_value.find(key) != _value.end()) ? "true" : "false";
}

std::string Dictionary::op_size(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    return Integer(int(_value.size())).representation();
}

std::string Dictionary::op_keys(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::vector<std::shared_ptr<Object>> keys;
    for (auto it = _value.begin(); it != _value.end(); it++)
        keys.push_back(it->first);
    return vector_representation(keys);
}

std::string Dictionary::op_values(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::vector<std::shared_ptr<Object>> values;
    for (auto it = _value.begin(); it != _value.end(); it++)
        values.push_back(it->second);
    return vector_representation(values);
}

std::string Dictionary::op_getValueForKey(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> key = get_shared_object(_key_type, arguments[0]);
    auto it = _value.find(key);
    if (it != _value.end())
        return it->second->representation();
    return "null";
}

std::string Dictionary::op_associateValue_withKey(const std::vector<std::string>& arguments) {
    if (arguments.size() != 2)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> value = get_shared_object(_value_type, arguments[0]);
    std::shared_ptr<Object> key = get_shared_object(_key_type, arguments[1]);
    _value[key] = value;
    return "null";
}

std::string Dictionary::op_removeKey(const std::vector<std::string>& arguments) {
    if (arguments.size() != 1)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    std::shared_ptr<Object> key = get_shared_object(_key_type, arguments[0]);
    _value.erase(key);
    return "null";
}

std::string Dictionary::op_clear(const std::vector<std::string>& arguments) {
    if (arguments.size() != 0)
        throw EXC_WRONG_NUMBER_OF_ARGUMENTS;
    _value.clear();
    return "null";
}
