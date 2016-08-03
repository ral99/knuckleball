# Copyright (c) 2016, Rodrigo Alves Lima
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
# following conditions are met:
#
#     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the
#        following disclaimer.
#
#     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#        following disclaimer in the documentation and/or other materials provided with the distribution.
#
#     3. Neither the name of Knuckleball nor the names of its contributors may be used to endorse or promote products
#        derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 2.8)

project(knuckleball)

# Dependencies
find_package(GTest QUIET)
find_package(Boost 1.58 COMPONENTS system program_options REQUIRED)

# Compiler options
set(EXECUTABLE_OUTPUT_PATH "build")
set(CMAKE_CXX_FLAGS "-O2 -Wall -pedantic -std=c++11")
include_directories(src)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# List files
set(HEADERS src/context.h src/exceptions.h src/grammar.h src/object.h src/parser.h src/server.h src/str_utils.h)
set(SOURCES src/context.cpp src/grammar.cpp src/object.cpp src/parser.cpp src/server.cpp src/str_utils.cpp)
set(UNIT_TESTS tests/context_test.cpp tests/grammar_test.cpp tests/object_test.cpp tests/parser_test.cpp
               tests/str_utils_test.cpp)

# Unit tests
if (GTEST_FOUND)
    enable_testing()
    include_directories(${GTEST_INCLUDE_DIRS})
    add_executable(unit_tests ${UNIT_TESTS} ${HEADERS} ${SOURCES})
    target_link_libraries(unit_tests ${GTEST_BOTH_LIBRARIES} ${Boost_LIBRARIES})
    GTEST_ADD_TESTS(unit_tests "" ${UNIT_TESTS})
endif (GTEST_FOUND)

# Build
add_executable(knuckleball src/knuckleball.cpp ${HEADERS} ${SOURCES})
target_link_libraries(knuckleball ${Boost_LIBRARIES})
install(TARGETS knuckleball DESTINATION build)
