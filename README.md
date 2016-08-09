# Knuckleball
Knuckleball is an in-memory data structure server. It provides a language with a syntax similar to that of Smalltalk to create and manipulate simple data types like booleans, characters, integers, floats and strings, and containers like vectors, sets and dictionaries. Knuckleball is implemented as a single-threaded and asynchronous server, so it can handle multiple incoming connections and concurrency is not an issue. It can be used as cache or message broker. Usage as database is not advised by now since data is not being persisted on disk yet.

**Table of Contents**
* [Building](#building)
  * [Dependencies](#dependencies)
  * [Compiling](#compiling)
  * [Testing](#testing)
* [Getting Started](#getting-started)
* [Reference](#reference)
  * [Boolean](#boolean)
  * [Character](#character)
  * [Integer](#integer)
  * [Float](#float)
  * [String](#string)
  * [Vector](#vector)
  * [Set](#set)
  * [Dictionary](#dictionary)
* [Future work](#future-work)

## Building
Knuckleball is being tested on Linux and OS X. It may be compiled on other systems, perhaps with minor modifications.

### Dependencies
* [Boost](http://www.boost.org/) >= 1.58: on Ubuntu you should install the package `libboost-all-dev`, and on OS X you should install the port `boost`.
* [Google Test](https://github.com/google/googletest) >= 1.7.0: this framework is only used for testing, so it is not required. On Ubuntu you should install the package `libgtest-dev` and follow these [steps](http://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/), and on OS X you should install the port `gtest`.

### Compiling
```
$ cmake CMakeLists.txt
$ make
```

### Testing
```
$ make test
```

## Getting started
To run the server on port 8001 of localhost, type:
```
$ cd build
$ ./knuckleball --bind 127.0.0.1 --port 8001
```

To list all the options available, type:
```
$ ./knuckleball --help
```

You can use `telnet` to play with Knuckleball. A Python [client](https://github.com/ral99/knuckleball-py) is also available.

The following example shows how to instantiate and manipulate an integer:
```
$ telnet 127.0.0.1 8001
Integer create: i withValue: 42;
> null
i add: 8;
> null
i get;
> 50
```

The following example shows how to instantiate and manipulate a vector of integers declared inside a namespace called `mynamespace`:
```
$ telnet 127.0.0.1 8001
Vector<Integer> create: mynamespace::points;
> null
mynamespace::points pushBack: 3;
> null
mynamespace::points pushBack: 7;
> null
mynamespace::points pushBack: 5;
> null
mynamespace::points popFront;
> 3
mynamespace::points get;
> [7,5]
```

The following example shows how to instantiate and manipulate a set of strings:
```
$ telnet 127.0.0.1 8001
Set<String> create: players;
> null
players add: "Babe Ruth";
> null
players add: "David Ortiz";
> null
players add: "Paulo Orlando";
> null
players contains? "Jake Arrieta";
> false
players get;
> {"Babe Ruth","David Ortiz","Paulo Orlando"}
```

The following example shows how to instantiate and manipulate a dictionary that associates floats with strings:
```
$ telnet 127.0.0.1 8001
Dictionary<String, Float> create: ERAs;
> null
ERAs associateValue: 1.82 withKey: "Ed Walsh";
> null
ERAs associateValue: 2.21 withKey: "Mariano Rivera";
> null
ERAs associateValue: 2.39 withKey: "Clayton Kershaw";
> null
ERAs getValueForKey: "Mariano Rivera";
> 2.210
ERAs get;
> (("Clayton Kershaw",2.390),("Ed Walsh",1.820),("Mariano Rivera",2.210))
```

## Reference
Types are divided into:
* **Objects**: [Booleans](#boolean), [Characters](#character), [Integers](#integer), [Floats](#float) and [Strings](#string).
* **Containers**: strongly typed [Vectors](#vector), [Sets](#set) and [Dictionaries](#dictionaryare) of **Objects**.

### Boolean
Booleans are objects that can only represent one of two states: `true` or `false`.

#### Constructors:
* **create: (variable) withValue: (boolean)**
  - Returns: null
```
Boolean create: t withValue: true;
> null
Boolean create: f withValue: false;
> null
t get;
> true
f get;
> false
```

* **createIfNotExists: (variable) withValue: (boolean)**
  - Returns: null
```
Boolean createIfNotExists: t withValue: true;
> null
Boolean createIfNotExists: f withValue: false;
> null
t get;
> true
f get;
> false
```

#### Methods:
* **get**
  - Returns: boolean
```
Boolean create: t withValue: true;
> null
t get;
> true
```

* **set: (boolean)**
  - Returns: null
```
Boolean create: t withValue: true;
> null
t set: false;
> null
t get;
> false
```

* **isTrue?**
  - Returns: boolean
```
Boolean create: t withValue: true;
> null
Boolean create: f withValue: false;
> null
t isTrue?;
> true
f isTrue?;
> false
```

* **isFalse?**
  - Returns: boolean
```
Boolean create: t withValue: true;
> null
Boolean create: f withValue: false;
> null
t isFalse?;
> false
f isFalse?;
> true
```

### Character
Characters are objects that can represent a single character, such as `'a'` or `'0'`.

#### Constructors:
* **create: (variable) withValue: (character)**
  - Returns: null
```
Character create: alphabetic withValue: 'a';
> null
Character create: numeric withValue: '0';
> null
Character create: space withValue: ' ';
> null
alphabetic get;
> 'a'
numeric get;
> '0'
space get;
> ' '
```

* **createIfNotExists: (variable) withValue: (character)**
  - Returns: null
```
Character createIfNotExists: alphabetic withValue: 'a';
> null
Character createIfNotExists: numeric withValue: '0';
> null
Character createIfNotExists: space withValue: ' ';
> null
alphabetic get;
> 'a'
numeric get;
> '0'
space get;
> ' '
```

#### Methods:
* **get**
  - Returns: character
```
Character create: alphabetic withValue: 'a';
> null
alphabetic get;
> 'a'
```

* **set: (character)**
  - Returns: null
```
Character create: alphabetic withValue: 'a';
> null
alphabetic set: 'z';
> null
alphabetic get;
> 'z'
```

* **isAlphabetic?**
  - Returns: boolean
```
Character create: alphabetic withValue: 'a';
> null
Character create: numeric withValue: '0';
> null
Character create: space withValue: ' ';
> null
alphabetic isAlphabetic?;
> true
numeric isAlphabetic?;
> false
space isAlphabetic?;
> false
```

* **isNumeric?**
  - Returns: boolean
```
Character create: alphabetic withValue: 'a';
> null
Character create: numeric withValue: '0';
> null
Character create: space withValue: ' ';
> null
alphabetic isNumeric?;
> false
numeric isNumeric?;
> true
space isNumeric?;
> false
```

* **isAlphanumeric?**
  - Returns: boolean
```
Character create: alphabetic withValue: 'a';
> null
Character create: numeric withValue: '0';
> null
Character create: space withValue: ' ';
> null
alphabetic isAlphanumeric?;
> true
numeric isAlphanumeric?;
> true
space isAlphanumeric?;
> false
```

* **isSpace?**
  - Returns: boolean
```
Character create: alphabetic withValue: 'a';
> null
Character create: numeric withValue: '0';
> null
Character create: space withValue: ' ';
> null
alphabetic isSpace?;
> false
numeric isSpace?;
> false
space isSpace?;
> true
```

* **isEqualTo? (character)**
  - Returns: boolean
```
Character create: alphabetic withValue: 'a';
> null
alphabetic isEqualTo? 'a';
> true
alphabetic isEqualTo? 'z';
> false
```

* **isLessThan? (character)**
  - Returns: boolean
```
Character create: alphabetic withValue: 'a';
> null
alphabetic isLessThan? 'z';
> true
alphabetic isLessThan? 'a';
> false
```

* **isLessThanOrEqualTo? (character)**
  - Returns: boolean
```
Character create: alphabetic withValue: 'z';
> null
alphabetic isLessThanOrEqualTo? 'z';
> true
alphabetic isLessThanOrEqualTo? 'a';
> false
```

* **isGreaterThan? (character)**
  - Returns: boolean
```
Character create: alphabetic withValue: 'z';
> null
alphabetic isGreaterThan? 'a';
> true
alphabetic isGreaterThan? 'z';
> false
```

* **isGreaterThanOrEqualTo? (character)**
  - Returns: boolean
```
Character create: alphabetic withValue: 'a';
> null
alphabetic isGreaterThanOrEqualTo? 'a';
> true
alphabetic isGreaterThanOrEqualTo? 'z';
> false
```

### Integer
Integers are objects that can represent a whole number value, such as `42` or `-42`. Floating-point values, such as `42.5`, can be casted to integers.

#### Constructors:
* **create: (variable) withValue: (integer)**
  - Returns: null
```
Integer create: i withValue: 42;
> null
i get;
> 42
```

* **createIfNotExists: (variable) withValue: (integer)**
  - Returns: null
```
Integer createIfNotExists: i withValue: 42;
> null
i get;
> 42
```

#### Methods:
* **get**
  - Returns: integer
```
Integer create: i withValue: 42;
> null
i get;
> 42
```

* **set: (integer)**
  - Returns: null 
```
Integer create: i withValue: 42;
> null
i set: -42;
> null
i get;
> -42
```

* **isEqualTo? (integer)**
  - Returns: boolean
```
Integer create: i withValue: 42;
> null
i isEqualTo? 42;
> true
i isEqualTo? 0;
> false
```

* **isLessThan? (integer)**
  - Returns: boolean
```
Integer create: i withValue: 0;
> null
i isLessThan? 42;
> true
i isLessThan? 0;
> false
```

* **isLessThanOrEqualTo? (integer)**
  - Returns: boolean
```
Integer create: i withValue: 42;
> null
i isLessThanOrEqualTo? 42;
> true
i isLessThanOrEqualTo? 0;
> false
```

* **isGreaterThan? (integer)**
  - Returns:  boolean
```
Integer create: i withValue: 42;
> null
i isGreaterThan? 0;
> true
i isGreaterThan? 42;
> false
```

* **isGreaterThanOrEqualTo? (integer)**
  - Returns: boolean
```
Integer create: i withValue: 0;
> null
i isGreaterThanOrEqualTo? 0;
> true
i isGreaterThanOrEqualTo? 42;
> false
```

* **add: (integer)**
  - Returns: null
```
Integer create: i withValue: 42;
> null
i add: 10;
> null
i get;
> 52
```

* **subtract: (integer)**
  - Returns: null
```
Integer create: i withValue: 52;
> null
i subtract: 10;
> null
i get;
> 42
```

* **multiplyBy: (float)**
  - Returns: null
```
Integer create: i withValue: 10;
> null
i multiplyBy: 4.2;
> null
i get;
> 42
```

* **divideBy: (float)**
  - Returns: null
```
Integer create: i withValue: 42;
> null
i divideBy: 4.2;
> null
i get;
> 10
```

### Float
Floats are objects that can represent a real value, such as `3.1415` or `1e-3`.

#### Constructors:
* **create: (variable) withValue: (float)**
  - Returns: null
```
Float create: f withValue: 42.0;
> null
Float create: e withValue: 1e-2;
> null
f get;
> 42.000
e get;
> 0.010
```

* **createIfNotExists: (variable) withValue: (float)**
  - Returns: null
```
Float createIfNotExists: f withValue: 42.0;
> null
Float createIfNotExists: e withValue: 1e-2;
> null
f get;
> 42.000
e get;
> 0.010
```

#### Methods:
* **get**
  - Returns: float
```
Float create: f withValue: 42.0;
> null
f get;
> 42.000
```

* **set: (float)**
  - Returns: null 
```
Float create: f withValue: 42.0;
> null
f set: -42.0;
> null
f get;
> -42.000
```

* **isEqualTo? (float)**
  - Returns: boolean
```
Float create: f withValue: 42.0;
> null
f isEqualTo? 42.0;
> true
f isEqualTo? 0.0;
> false
```

* **isLessThan? (float)**
  - Returns: boolean
```
Float create: f withValue: 0.0;
> null
f isLessThan? 42.0;
> true
f isLessThan? 0.0;
> false
```

* **isLessThanOrEqualTo? (float)**
  - Returns: boolean
```
Float create: f withValue: 42.0;
> null
f isLessThanOrEqualTo? 42.0;
> true
f isLessThanOrEqualTo? 0.0;
> false
```

* **isGreaterThan? (float)**
  - Returns:  boolean
```
Float create: f withValue: 42.0;
> null
f isGreaterThan? 0.0;
> true
f isGreaterThan? 42.0;
> false
```

* **isGreaterThanOrEqualTo? (float)**
  - Returns: boolean
```
Float create: f withValue: 0.0;
> null
f isGreaterThanOrEqualTo? 0.0;
> true
f isGreaterThanOrEqualTo? 42.0;
> false
```

* **add: (float)**
  - Returns: null
```
Float create: f withValue: 42.0;
> null
f add: 10.0;
> null
f get;
> 52.000
```

* **subtract: (float)**
  - Returns: null
```
Float create: f withValue: 52.0;
> null
f subtract: 10.0;
> null
f get;
> 42.000
```

* **multiplyBy: (float)**
  - Returns: null
```
Float create: f withValue: 4.2;
> null
f multiplyBy: 10.0;
> null
f get;
42.000;
```

* **divideBy: (float)**
  - Returns: null
```
Float create: f withValue: 42.0;
> null
f divideBy: 4.2;
> null
f get;
> 10.000
```

### String
Strings are objects that can represent a sequence of characters, such as `"knuckleball"`.

#### Constructors:
* **create: (variable)**
  - Returns: null
```
String create: str;
> null
str get;
> ""
```

* **createIfNotExists: (variable)**
  - Returns: null
```
String createIfNotExists: str;
> null
str get;
> ""
```

* **create: (variable) withValue: (string)**
  - Returns: null
```
String create: str withValue: "knuckleball";
> null
str get;
> "knuckleball"
```

* **createIfNotExists: (variable) withValue: (string)**
  - Returns: null
```
String createIfNotExists: str withValue: "knuckleball";
> null
str get;
> "knuckleball"
```

#### Methods:
* **get**
  - Returns: string
```
String create: str withValue: "knuckleball";
> null
str get;
> "knuckleball"
```

* **set: (string)**
  - Returns: null
```
String create: str withValue: "";
> null
str set: "knuckleball";
> null
str get;
> "knuckleball"
```

* **isEmpty?**
  - Returns: boolean 
```
String create: str withValue: "";
> null
str isEmpty?;
> true
str concatenate: "knuckleball";
> null
str isEmpty?;
> false
```

* **startsWith? (string)**
  - Returns: boolean
```
String create: str withValue: "knuckleball";
> null
str startsWith? "knuckle";
> true
str startsWith? "ball";
> false
```

* **endsWith? (string)**
  - Returns: boolean
```
String create: str withValue: "knuckleball";
> null
str endsWith? "ball";
> true
str endsWith? "knuckle";
> false
```

* **isLexicographicallyEqualTo? (string)**
  - Returns: boolean
```
String create: str withValue: "knuckleball";
> null
str isLexicographicallyEqualTo? "knuckleball";
> true
str isLexicographicallyEqualTo? "";
> false
```

* **isLexicographicallyLessThan? (string)**
  - Returns: boolean
```
String create: str withValue: "";
> null
str isLexicographicallyLessThan? "knuckleball";
> true
str isLexicographicallyLessThan? "";
> false
```

* **isLexicographicallyLessThanOrEqualTo? (string)**
  - Returns: boolean
```
String create: str withValue: "knuckleball";
> null
str isLexicographicallyLessThanOrEqualTo? "knuckleball";
> true
str isLexicographicallyLessThanOrEqualTo? "";
> false
```

* **isLexicographicallyGreaterThan? (string)**
  - Returns: boolean
```
String create: str withValue: "knuckleball";
> null
str isLexicographicallyGreaterThan? "";
> true
str isLexicographicallyGreaterThan? "knuckleball";
> false
```

* **isLexicographicallyGreaterThanOrEqualTo? (string)**
  - Returns: boolean
```
String create: str withValue: "";
> null
str isLexicographicallyGreaterThanOrEqualTo? "";
> true
str isLexicographicallyGreaterThanOrEqualTo? "knuckleball";
> false
```

* **length**
  - Returns: integer
```
String create: str withValue: "knuckleball";
> null
str length;
> 11
```

* **atIndex: (integer)**
  - Returns: character
```
String create: str withValue: "knuckleball";
> null
str atIndex: 0;
> 'k'
```

* **first: (integer)**
  - Returns: string
```
String create: str withValue: "knuckleball";
> null
str first: 7;
> "knuckle"
```

* **last: (integer)**
  - Returns: string
```
String create: str withValue: "knuckleball";
> null
str last: 4;
> "ball"
```

* **substringFromIndex: (integer)**
  - Returns: string
```
String create: str withValue: "knuckleball";
> null
str substringFromIndex: 7;
> "ball"
```

* **substringFromIndex: (integer) toIndex: (integer)**
  - Returns: string
```
String create: str withValue: "knuckleball";
> null
str substringFromIndex: 0 toIndex: 7;
> "knuckle"
```

* **pushBack: (character)**
  - Returns: null
```
String create: str withValue: "knucklebal";
> null
str pushBack: 'l';
> null
str get;
> "knuckleball"
```

* **pushFront: (character)**
  - Returns: null
```
String create: str withValue: "nuckleball";
> null
str pushFront: 'k';
> null
str get;
> "knuckleball"
```

* **popBack**
  - Returns: character
```
String create: str withValue: "knuckleball";
> null
str popBack;
> 'l'
str get;
> "knucklebal"
```

* **popFront**
  - Returns: character
```
String create: str withValue: "knuckleball";
> null
str popFront;
> 'k'
str get;
> "nuckleball"
```

* **popAtIndex: (integer)**
  - Returns: character
```
String create: str withValue: "knuckleball";
> null
str popAtIndex: 2;
> 'u'
str get;
> "knckleball"
```

* **concatenate: (string)**
  - Returns: null
```
String create: str withValue: "knuckle";
> null
str concatenate: "ball";
> null
str get;
> "knuckleball"
```

* **insert: (string) beforeIndex: (integer)**
  - Returns: null
```
String create: str withValue: "knkleball";
> null
str insert: "uc" beforeIndex: 2;
> null
str get;
> "knuckleball"
```

* **eraseFromIndex: (integer)**
  - Returns: null
```
String create: str withValue: "knuckleball";
> null
str eraseFromIndex: 7;
> null
str get;
> "knuckle"
```

* **eraseFromIndex: (integer) toIndex: (integer)**
  - Returns: null
```
String create: str withValue: "knuckleball";
> null
str eraseFromIndex: 7 toIndex: 9;
> null
str get;
> "knucklell"
```

* **clear**
  - Returns: null
```
String create: str withValue: "knuckleball";
> null
str clear;
> null
str get;
> ""
```

### Vector
Vectors are containers that store elements using contiguous storage locations.

#### Constructors:
* **create: (variable)**
  - Returns: null
```
Vector<Integer> create: points;
> null
points get;
> []
```

* **createIfNotExists: (variable)**
  - Returns: null
```
Vector<Integer> createIfNotExists: points;
> null
points get;
> []
```

#### Methods:
* **get**
  - Returns: vector
```
Vector<Integer> create: points;
> null
points get;
> []
```

* **isEmpty?**
  - Returns: boolean
```
Vector<Integer> create: points;
> null
points isEmpty?;
> true
points pushBack: 42;
> null
points isEmpty?;
> false
```

* **contains? (object)**
  - Returns: boolean
```
Vector<Integer> create: points;
> null
points contains? 42;
> false
points pushBack: 42;
> null
points contains? 42;
> true
```

* **contains? (object) fromIndex: (integer)**
  - Returns: boolean
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 7;
> null
points pushBack: 5;
> null
points contains? 7 fromIndex: 0;
> true
points contains? 7 fromIndex: 2;
> false
```

* **count: (object)**
  - Returns: integer
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 7;
> null
points pushBack: 7;
> null
points count: 7;
> 2
```

* **atIndex: (integer)**
  - Returns: object
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points atIndex: 0;
> 3
```

* **firstIndexOf: (object)**
  - Returns: integer
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points firstIndexOf: 5;
> 1
```

* **firstIndexOf: (object) fromIndex: (integer)**
  - Returns: integer
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points firstIndexOf: 3 fromIndex: 1;
> 2
```

* **allIndexesOf: (object)**
  - Returns: vector
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 3;
> null
points allIndexesOf: 3;
> [0,2]
```

* **size**
  - Returns: Integer
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points size;
> 3
```

* **first: (integer)**
  - Returns: vector
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points first: 2;
> [3,5]
```

* **last: (integer)**
  - Returns: vector
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points last: 2;
> [5,7]
```

* **sliceFromIndex: (integer)**
  - Returns: vector
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points pushBack: 9;
> null
points sliceFromIndex: 2;
> [7,9]
```

* **sliceFromIndex: (integer) toIndex: (integer)**
  - Returns: vector
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points pushBack: 9;
> null
points sliceFromIndex: 1 toIndex: 3;
> [5,7]
```

* **pushBack: (object)**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points get;
> [3,5,7]
```

* **pushFront: (object)**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushFront: 3;
> null
points pushFront: 5;
> null
points pushFront: 7;
> null
points get;
> [7,5,3]
```

* **popBack**
  - Returns: object
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points popBack;
> 7
points get;
> [3,5]
```

* **popFront**
  - Returns: object
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points popFront;
> 3
points get;
> [5,7]
```

* **popAtIndex: (integer)**
  - Returns: object
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points popAtIndex: 1;
> 5
points get;
> [3,7]
```

* **insert: (object) beforeIndex: (integer)**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 7;
> null
points insert: 5 beforeIndex: 1;
> null
points get;
> [3,5,7]
```

* **eraseFromIndex: (integer)**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points eraseFromIndex: 1;
> null
points get;
> [3]
```

* **eraseFromIndex: (integer) toIndex: (integer)**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points eraseFromIndex: 1 toIndex: 2;
> null
points get;
> [3,7]
```

* **removeFirst: (object)**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points removeFirst: 5;
> null
points get;
> [3,3,5]
```

* **removeFirst: (object) fromIndex: (integer)**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points removeFirst: 5 fromIndex: 2;
> null
points get;
> [3,5,3]
```

* **removeAll: (object)**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points removeAll: 3;
> null
points get;
> [5,5]
```

* **replaceFirst: (object) with: (object)**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points replaceFirst: 5 with: 7; 
> null
points get;
> [3,7,3,5]
```

* **replaceFirst: (object) fromIndex: (integer) with: (object)**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points replaceFirst: 5 fromIndex: 2 with: 7; 
> null
points get;
> [3,5,3,7]
```

* **replaceAll: (object) with: (object)**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points replaceAll: 5 with: 7; 
> null
points get;
> [3,7,3,7]
```

* **sort**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 7;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points sort;
> null
points get;
> [3,5,7]
```

* **reverse**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points reverse;
> null
points get;
> [7,5,3]
```

* **clear**
  - Returns: null
```
Vector<Integer> create: points;
> null
points pushBack: 3;
> null
points pushBack: 5;
> null
points pushBack: 7;
> null
points clear;
> null
points get;
> []
```

### Set
Sets are containers that store unique elements.

#### Constructors:
* **create: (variable)**
  - Returns: null
```
Set<String> create: players;
> null
players get;
> {}
```

* **createIfNotExists: (variable)**
  - Returns: null
```
Set<String> createIfNotExists: players;
> null
players get;
> {}
```

#### Methods:
* **get**
  - Returns: set
```
Set<String> create: players;
> null
players get;
> {}
```

* **isEmpty?**
  - Returns: boolean
```
Set<String> create: players;
> null
players isEmpty?;
> true
players add: "Babe Ruth";
> null
players isEmpty?;
> false
```

* **contains? (object)**
  - Returns: boolean
```
Set<String> create: players;
> null
players contains? "Babe Ruth";
> false
players add: "Babe Ruth";
> null
players contains? "Babe Ruth";
> true
```

* **size**
  - Returns: integer
```
Set<String> create: players;
> null
players add: "Babe Ruth";
> null
players add: "David Ortiz";
> null
players add: "Paulo Orlando";
> null
players size;
> 3
```

* **add: (object)**
  - Returns: null
```
Set<String> create: players;
> null
players add: "Babe Ruth";
> null
players get;
> {"Babe Ruth"}
```

* **remove: (object)**
  - Returns: null
```
Set<String> create: players;
> null
players add: "Babe Ruth";
> null
players remove: "Babe Ruth";
> null
players get;
> {}
```

* **clear**
  - Returns: null
```
Set<String> create: players;
> null
players add: "Babe Ruth";
> null
players add: "David Ortiz";
> null
players add: "Paulo Orlando";
> null
players clear;
> null
players get;
> {}
```

### Dictionary
Dictionaries are associative containers that store keys and their associated values.

#### Constructors:
* **create: (variable)**
  - Returns: null
```
Dictionary<String, Float> create: ERAs;
> null
ERAs get;
> ()
```

* **createIfNotExists: (variable)**
  - Returns: null
```
Dictionary<String, Float> createIfNotExists: ERAs;
> null
ERAs get;
> ()
```

#### Methods:
* **get**
  - Returns: dictionary
```
Dictionary<String, Float> create: ERAs;
> null
ERAs get;
> ()
```

* **isEmpty?**
  - Returns: boolean
```
Dictionary<String, Float> create: ERAs;
> null
ERAs isEmpty?;
> true
ERAs associateValue: 1.82 withKey: "Ed Walsh";
> null
ERAs isEmpty?;
> false
```

* **containsKey? (object)**
  - Returns: boolean
```
Dictionary<String, Float> create: ERAs;
> null
ERAs containsKey? "Ed Walsh";
> false
ERAs associateValue: 1.82 withKey: "Ed Walsh";
> null
ERAs containsKey? "Ed Walsh";
> true
```

* **size**
  - Returns: integer
```
Dictionary<String, Float> create: ERAs;
> null
ERAs size;
> 0
ERAs associateValue: 1.82 withKey: "Ed Walsh";
> null
ERAs size;
> 1
```

* **keys**
  - Returns: vector
```
Dictionary<String, Float> create: ERAs;
> null
ERAs associateValue: 1.82 withKey: "Ed Walsh";
> null
ERAs associateValue: 2.21 withKey: "Mariano Rivera";
> null
ERAs associateValue: 2.39 withKey: "Clayton Kershaw";
> null
ERAs keys;
> ["Clayton Kershaw","Ed Walsh","Mariano Rivera"]
```

* **values**
  - Returns: vector
```
Dictionary<String, Float> create: ERAs;
> null
ERAs associateValue: 1.82 withKey: "Ed Walsh";
> null
ERAs associateValue: 2.21 withKey: "Mariano Rivera";
> null
ERAs associateValue: 2.39 withKey: "Clayton Kershaw";
> null
ERAs values;
> [2.390,1.820,2.210]
```

* **getValueForKey: (object)**
  - Returns: object
```
Dictionary<String, Float> create: ERAs;
> null
ERAs associateValue: 1.82 withKey: "Ed Walsh";
> null
ERAs getValueForKey: "Ed Walsh";
> 1.820
```

* **associateValue: (object) withKey: (object)**
  - Returns: null
```
Dictionary<String, Float> create: ERAs;
> null
ERAs associateValue: 1.82 withKey: "Ed Walsh";
> null
ERAs get;
> (("Ed Walsh",1.82))
```

* **removeKey: (object)**
  - Returns: null
```
Dictionary<String, Float> create: ERAs;
> null
ERAs associateValue: 1.82 withKey: "Ed Walsh";
> null
ERAs removeKey: "Ed Walsh";
> null
ERAs get;
> ()
```

* **clear**
  - Returns: null
```
Dictionary<String, Float> create: ERAs;
> null
ERAs associateValue: 1.82 withKey: "Ed Walsh";
> null
ERAs associateValue: 2.21 withKey: "Mariano Rivera";
> null
ERAs associateValue: 2.39 withKey: "Clayton Kershaw";
> null
ERAs clear;
> null
ERAs get;
> ()
```

### Context
Context is an execution environment.

#### Methods:
* **listNamespaces**
  - Returns: vector
```
Boolean create: std::bool withValue: true;
> null
Integer create: mynamespace::i withValue: 42;
> null
Context listNamespaces;
> [mynamespace,std]
```

* **listVariables**
  - Returns: vector
```
Boolean create: std::bool withValue: true;
> null
Integer create: i withValue: 42;
> null
Context listVariables;
> [i,std::bool]
```

* **listVariablesOfNamespace: (namespace)**
  - Returns: vector
```
Boolean create: std::bool withValue: true;
> null
Character create: std::char withValue: 'a';
> null
Integer create: mynamespace::i withValue: 42;
> null
Context listVariablesOfNamespace: std;
> [std::bool,std::char]
```

* **deleteVariable: (variable)**
  - Returns: null
```
Boolean create: bool withValue: true;
> null
Integer create: i withValue: 42;
> null
Context deleteVariable: bool;
> null
Context listVariables;
> [i]
```

* **deleteVariablesOfNamespace: (namespace)**
  - Returns: null
```
Boolean create: std::bool withValue: true;
> null
Character create: std::char withValue: 'a';
> null
Integer create: i withValue: 42;
> null
Context deleteVariablesOfNamespace: std;
> null
Context listVariables;
> [i]
```

* **getFloatPrecision**
  - Returns: integer
```
Context setFloatPrecision: 5;
> null
Context getFloatPrecision;
> 5
```

* **setFloatPrecision: (integer)**
  - Returns: null
```
Float create: f withValue: 0.1;
> null
Context setFloatPrecision: 5;
> null
f get;
> 0.10000
```

* **getFloatComparisonTolerance**
  - Returns: float
```
Context setFloatComparisonTolerance: 1e-3;
> null
Context getFloatComparisonTolerance;
> 0.001
```

* **setFloatComparisonTolerance: (float)**
  - Returns: null
```
Float create: f withValue: 0.0;
> null
Context setFloatComparisonTolerance: 1.0;
> null
f isEqualTo? 0.9;
> true
Context setFloatComparisonTolerance: 1e-3;
> null
f isEqualTo? 0.9;
> false
```

### Connection
Connection is an object that represents the TCP connection with the server.

#### Methods:
* **authenticateWithPassword: (string)**
  - Returns: null
```
Connection authenticateWithPassword: "password";
> null
```

* **close**
  - Returns: null
```
Connection close;
> null
```

## Future work
* Persistence on disk.
* Transaction manager.
* Containers like lists, stacks, queues, multisets and multidictionaries.
