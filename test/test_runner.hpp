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
    void assert_true(bool condition)\
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
