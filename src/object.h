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

#ifndef __OBJECT__
#define __OBJECT__

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

class Object : public std::enable_shared_from_this<Object> {
protected:
    // Attributes:
    std::string _type;
    std::string _name;
    
    // Constructor:
    Object(const std::string& type);
public:
    // Virtual destructor:
    virtual ~Object();

    // Getter methods:
    std::string type() const;
    std::string name() const;

    // Pure virtual methods:
    virtual std::string representation() const = 0;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments) = 0;
};

class ObjectIsEqualToComparator {
public:
    // Functor to compare two Objects.
    bool operator()(const std::shared_ptr<Object>& lhs, const std::shared_ptr<Object>& rhs) const;
};

class ObjectIsLessThanComparator {
public:
    // Functor to compare two Objects.
    bool operator()(const std::shared_ptr<Object>& lhs, const std::shared_ptr<Object>& rhs) const;
};

class ObjectIsLessThanOrEqualToComparator {
public:
    // Functor to compare two Objects.
    bool operator()(const std::shared_ptr<Object>& lhs, const std::shared_ptr<Object>& rhs) const;
};

class ObjectIsGreaterThanComparator {
public:
    // Functor to compare two Objects.
    bool operator()(const std::shared_ptr<Object>& lhs, const std::shared_ptr<Object>& rhs) const;
};

class ObjectIsGreaterThanOrEqualToComparator {
public:
    // Functor to compare two Objects.
    bool operator()(const std::shared_ptr<Object>& lhs, const std::shared_ptr<Object>& rhs) const;
};

class Boolean: public Object {
private:
    // Attributes:
    bool _value;

    // Operators:
    std::string op_get(const std::vector<std::string>& arguments);
    std::string op_set(const std::vector<std::string>& arguments);
    std::string op_isTrue(const std::vector<std::string>& arguments);
    std::string op_isFalse(const std::vector<std::string>& arguments);
public:
    // Constructors:
    Boolean(const std::string& value);
    Boolean(const std::string& message_name, const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~Boolean();

    // Getter method:
    bool value() const;

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class Character: public Object {
private:
    // Attributes:
    char _value;

    // Operators:
    std::string op_get(const std::vector<std::string>& arguments);
    std::string op_set(const std::vector<std::string>& arguments);
    std::string op_isAlphabetic(const std::vector<std::string>& arguments);
    std::string op_isNumeric(const std::vector<std::string>& arguments);
    std::string op_isAlphanumeric(const std::vector<std::string>& arguments);
    std::string op_isSpace(const std::vector<std::string>& arguments);
    std::string op_isEqualTo(const std::vector<std::string>& arguments);
    std::string op_isLessThan(const std::vector<std::string>& arguments);
    std::string op_isLessThanOrEqualTo(const std::vector<std::string>& arguments);
    std::string op_isGreaterThan(const std::vector<std::string>& arguments);
    std::string op_isGreaterThanOrEqualTo(const std::vector<std::string>& arguments);
public:
    // Constructors:
    Character(const std::string& value);
    Character(const std::string& message_name, const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~Character();

    // Getter method:
    char value() const;

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class Integer: public Object {
private:
    // Attributes:
    int _value;

    // Operators:
    std::string op_get(const std::vector<std::string>& arguments);
    std::string op_set(const std::vector<std::string>& arguments);
    std::string op_isEqualTo(const std::vector<std::string>& arguments);
    std::string op_isLessThan(const std::vector<std::string>& arguments);
    std::string op_isLessThanOrEqualTo(const std::vector<std::string>& arguments);
    std::string op_isGreaterThan(const std::vector<std::string>& arguments);
    std::string op_isGreaterThanOrEqualTo(const std::vector<std::string>& arguments);
    std::string op_add(const std::vector<std::string>& arguments);
    std::string op_subtract(const std::vector<std::string>& arguments);
    std::string op_multiplyBy(const std::vector<std::string>& arguments);
    std::string op_divideBy(const std::vector<std::string>& arguments);
public:
    // Constructors:
    Integer(int value);
    Integer(const std::string& value);
    Integer(const std::string& message_name, const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~Integer();

    // Getter method:
    int value() const;

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class Float: public Object {
private:
    // Attributes:
    float _value;

    // Operators:
    std::string op_get(const std::vector<std::string>& arguments);
    std::string op_set(const std::vector<std::string>& arguments);
    std::string op_isEqualTo(const std::vector<std::string>& arguments);
    std::string op_isLessThan(const std::vector<std::string>& arguments);
    std::string op_isLessThanOrEqualTo(const std::vector<std::string>& arguments);
    std::string op_isGreaterThan(const std::vector<std::string>& arguments);
    std::string op_isGreaterThanOrEqualTo(const std::vector<std::string>& arguments);
    std::string op_add(const std::vector<std::string>& arguments);
    std::string op_subtract(const std::vector<std::string>& arguments);
    std::string op_multiplyBy(const std::vector<std::string>& arguments);
    std::string op_divideBy(const std::vector<std::string>& arguments);
public:
    // Constructors:
    Float(float value);
    Float(const std::string& value);
    Float(const std::string& message_name, const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~Float();

    // Getter method:
    float value() const;

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class String: public Object {
private:
    // Attributes:
    std::string _value;

    // Operators:
    std::string op_get(const std::vector<std::string>& arguments);
    std::string op_set(const std::vector<std::string>& arguments);
    std::string op_isEmpty(const std::vector<std::string>& arguments);
    std::string op_startsWith(const std::vector<std::string>& arguments);
    std::string op_endsWith(const std::vector<std::string>& arguments);
    std::string op_isLexicographicallyEqualTo(const std::vector<std::string>& arguments);
    std::string op_isLexicographicallyLessThan(const std::vector<std::string>& arguments);
    std::string op_isLexicographicallyLessThanOrEqualTo(const std::vector<std::string>& arguments);
    std::string op_isLexicographicallyGreaterThan(const std::vector<std::string>& arguments);
    std::string op_isLexicographicallyGreaterThanOrEqualTo(const std::vector<std::string>& arguments);
    std::string op_length(const std::vector<std::string>& arguments);
    std::string op_atIndex(const std::vector<std::string>& arguments);
    std::string op_first(const std::vector<std::string>& arguments);
    std::string op_last(const std::vector<std::string>& arguments);
    std::string op_substringFromIndex(const std::vector<std::string>& arguments);
    std::string op_substringFromIndex_toIndex(const std::vector<std::string>& arguments);
    std::string op_pushBack(const std::vector<std::string>& arguments);
    std::string op_pushFront(const std::vector<std::string>& arguments);
    std::string op_popBack(const std::vector<std::string>& arguments);
    std::string op_popFront(const std::vector<std::string>& arguments);
    std::string op_popAtIndex(const std::vector<std::string>& arguments);
    std::string op_concatenate(const std::vector<std::string>& arguments);
    std::string op_insert_beforeIndex(const std::vector<std::string>& arguments);
    std::string op_eraseFromIndex(const std::vector<std::string>& arguments);
    std::string op_eraseFromIndex_toIndex(const std::vector<std::string>& arguments);
    std::string op_clear(const std::vector<std::string>& arguments);
public:
    // Constructors:
    String(const std::string& value);
    String(const std::string& message_name, const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~String();

    // Getter method:
    std::string value() const;

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class Vector: public Object {
private:
    // Attributes:
    std::string _element_type;
    std::vector<std::shared_ptr<Object>> _value;

    // Operators:
    std::string op_get(const std::vector<std::string>& arguments);
    std::string op_isEmpty(const std::vector<std::string>& arguments);
    std::string op_contains(const std::vector<std::string>& arguments);
    std::string op_contains_fromIndex(const std::vector<std::string>& arguments);
    std::string op_count(const std::vector<std::string>& arguments);
    std::string op_atIndex(const std::vector<std::string>& arguments);
    std::string op_firstIndexOf(const std::vector<std::string>& arguments);
    std::string op_firstIndexOf_fromIndex(const std::vector<std::string>& arguments);
    std::string op_allIndexesOf(const std::vector<std::string>& arguments);
    std::string op_size(const std::vector<std::string>& arguments);
    std::string op_first(const std::vector<std::string>& arguments);
    std::string op_last(const std::vector<std::string>& arguments);
    std::string op_sliceFromIndex(const std::vector<std::string>& arguments);
    std::string op_sliceFromIndex_toIndex(const std::vector<std::string>& arguments);
    std::string op_pushBack(const std::vector<std::string>& arguments);
    std::string op_pushFront(const std::vector<std::string>& arguments);
    std::string op_popBack(const std::vector<std::string>& arguments);
    std::string op_popFront(const std::vector<std::string>& arguments);
    std::string op_popAtIndex(const std::vector<std::string>& arguments);
    std::string op_insert_beforeIndex(const std::vector<std::string>& arguments);
    std::string op_eraseFromIndex(const std::vector<std::string>& arguments);
    std::string op_eraseFromIndex_toIndex(const std::vector<std::string>& arguments);
    std::string op_removeFirst(const std::vector<std::string>& arguments);
    std::string op_removeFirst_fromIndex(const std::vector<std::string>& arguments);
    std::string op_removeAll(const std::vector<std::string>& arguments);
    std::string op_replaceFirst_with(const std::vector<std::string>& arguments);
    std::string op_replaceFirst_fromIndex_with(const std::vector<std::string>& arguments);
    std::string op_replaceAll_with(const std::vector<std::string>& arguments);
    std::string op_sort(const std::vector<std::string>& arguments);
    std::string op_reverse(const std::vector<std::string>& arguments);
    std::string op_clear(const std::vector<std::string>& arguments);
public:
    // Constructor:
    Vector(const std::string& element_type, const std::string& message_name, const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~Vector();

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class Set: public Object {
private:
    // Attributes:
    std::string _element_type;
    std::set<std::shared_ptr<Object>, ObjectIsLessThanComparator> _value;

    // Operators:
    std::string op_get(const std::vector<std::string>& arguments);
    std::string op_isEmpty(const std::vector<std::string>& arguments);
    std::string op_contains(const std::vector<std::string>& arguments);
    std::string op_size(const std::vector<std::string>& arguments);
    std::string op_add(const std::vector<std::string>& arguments);
    std::string op_remove(const std::vector<std::string>& arguments);
    std::string op_clear(const std::vector<std::string>& arguments);
public:
    // Constructor:
    Set(const std::string& element_type, const std::string& message_name, const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~Set();

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class Dictionary: public Object {
private:
    // Attributes:
    std::string _key_type, _value_type;
    std::map<std::shared_ptr<Object>, std::shared_ptr<Object>, ObjectIsLessThanComparator> _value;

    // Operators:
    std::string op_get(const std::vector<std::string>& arguments);
    std::string op_isEmpty(const std::vector<std::string>& arguments);
    std::string op_containsKey(const std::vector<std::string>& arguments);
    std::string op_size(const std::vector<std::string>& arguments);
    std::string op_keys(const std::vector<std::string>& arguments);
    std::string op_values(const std::vector<std::string>& arguments);
    std::string op_getValueForKey(const std::vector<std::string>& arguments);
    std::string op_associateValue_withKey(const std::vector<std::string>& arguments);
    std::string op_removeKey(const std::vector<std::string>& arguments);
    std::string op_clear(const std::vector<std::string>& arguments);
public:
    // Constructor:
    Dictionary(const std::string& key_type, const std::string& value_type, const std::string& message_name,
               const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~Dictionary();

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

#endif
