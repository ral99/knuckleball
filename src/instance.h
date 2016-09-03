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

#ifndef KNUCKLEBALL_INSTANCE_H
#define KNUCKLEBALL_INSTANCE_H

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

class Instance : public std::enable_shared_from_this<Instance> {
protected:
    // Attributes:
    std::string _type;
    std::string _name;
    
    // Constructor:
    Instance(const std::string& type);
public:
    // Virtual destructor:
    virtual ~Instance();

    // Getter methods:
    std::string type() const;
    std::string name() const;

    // Pure virtual methods:
    virtual std::string representation() const = 0;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments) = 0;
};

class InstanceIsEqualToComparator {
public:
    // Functor to compare two Instances.
    bool operator()(const std::shared_ptr<Instance>& lhs, const std::shared_ptr<Instance>& rhs) const;
};

class InstanceIsLessThanComparator {
public:
    // Functor to compare two Instances.
    bool operator()(const std::shared_ptr<Instance>& lhs, const std::shared_ptr<Instance>& rhs) const;
};

class InstanceIsLessThanOrEqualToComparator {
public:
    // Functor to compare two Instances.
    bool operator()(const std::shared_ptr<Instance>& lhs, const std::shared_ptr<Instance>& rhs) const;
};

class InstanceIsGreaterThanComparator {
public:
    // Functor to compare two Instances.
    bool operator()(const std::shared_ptr<Instance>& lhs, const std::shared_ptr<Instance>& rhs) const;
};

class InstanceIsGreaterThanOrEqualToComparator {
public:
    // Functor to compare two Instances.
    bool operator()(const std::shared_ptr<Instance>& lhs, const std::shared_ptr<Instance>& rhs) const;
};

class BooleanInstance: public Instance {
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
    BooleanInstance(const std::string& value);
    BooleanInstance(const std::string& message_name, const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~BooleanInstance();

    // Getter method:
    bool value() const;

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class CharacterInstance: public Instance {
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
    CharacterInstance(const std::string& value);
    CharacterInstance(const std::string& message_name, const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~CharacterInstance();

    // Getter method:
    char value() const;

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class IntegerInstance: public Instance {
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
    IntegerInstance(int value);
    IntegerInstance(const std::string& value);
    IntegerInstance(const std::string& message_name, const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~IntegerInstance();

    // Getter method:
    int value() const;

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class FloatInstance: public Instance {
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
    FloatInstance(float value);
    FloatInstance(const std::string& value);
    FloatInstance(const std::string& message_name, const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~FloatInstance();

    // Getter method:
    float value() const;

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class StringInstance: public Instance {
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
    StringInstance(const std::string& value);
    StringInstance(const std::string& message_name, const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~StringInstance();

    // Getter method:
    std::string value() const;

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class VectorInstance: public Instance {
private:
    // Attributes:
    std::string _element_type;
    std::vector<std::shared_ptr<Instance>> _value;

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
    VectorInstance(const std::string& element_type, const std::string& message_name,
                   const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~VectorInstance();

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class SetInstance: public Instance {
private:
    // Attributes:
    std::string _element_type;
    std::set<std::shared_ptr<Instance>, InstanceIsLessThanComparator> _value;

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
    SetInstance(const std::string& element_type, const std::string& message_name,
                const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~SetInstance();

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

class DictionaryInstance: public Instance {
private:
    // Attributes:
    std::string _key_type, _value_type;
    std::map<std::shared_ptr<Instance>, std::shared_ptr<Instance>, InstanceIsLessThanComparator> _value;

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
    DictionaryInstance(const std::string& key_type, const std::string& value_type, const std::string& message_name,
                       const std::vector<std::string>& arguments);

    // Virtual destructor:
    virtual ~DictionaryInstance();

    // Implementation of pure virtual methods:
    virtual std::string representation() const;
    virtual std::string receive(const std::string& message_name, const std::vector<std::string>& arguments);
};

#endif
