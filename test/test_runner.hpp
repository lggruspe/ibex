// MIT License
// 
// Copyright (c) 2019 Levi Gruspe
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#include <cstddef>
#include <string>
#include <iostream>

static bool global_passed = true;

#define run_test(runner, test) (runner).run(#test, (test))

#define define_test_runner(TestRunner, declarations_, ...) \
class TestRunner;\
\
class TestRunner {\
    size_t total_asserts_;\
    size_t failed_asserts_;\
\
public:\
    void (*setup_)(TestRunner*);\
    void (*teardown_)(TestRunner*);\
    declarations_;\
\
private:\
    char dummy_;\
public:\
    TestRunner(\
            void (*setup_)(TestRunner*)=nullptr,\
            void (*teardown_)(TestRunner*)=nullptr)\
        : total_asserts_(0)\
        , failed_asserts_(0)\
        , setup_(setup_)\
        , teardown_(teardown_)\
        , __VA_ARGS__\
        dummy_(0)\
    {}\
\
    bool run(\
            std::string test_name,\
            void (*test)(TestRunner*),\
            void (*setup_)(TestRunner*)=nullptr,\
            void (*teardown_)(TestRunner*)=nullptr)\
    {\
        if (!setup_) {\
            setup_ = this->setup_;\
        }\
        if (!teardown_) {\
            teardown_ = this->teardown_;\
        }\
\
        if (setup_) {\
            setup_(this);\
        }\
        total_asserts_ = 0;\
        failed_asserts_ = 0;\
        test(this);\
        if (teardown_) {\
            teardown_(this);\
        }\
        std::cerr << #TestRunner << "." << test_name << " " << (passed() ? "passed" : "failed") << std::endl;\
        global_passed = global_passed && passed();\
        return passed();\
    }\
    bool passed() const\
    {\
        return total_asserts_ > 0 && failed_asserts_ == 0;\
    }\
\
    void assert(bool condition)\
    {\
        total_asserts_++;\
        if (!condition) {\
            failed_asserts_++;\
        }\
    }\
};

int exit_test()
{
    return global_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
