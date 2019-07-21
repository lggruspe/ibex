#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define check_assertion(condition) do {\
    if (!(condition)) {\
        passed = false;\
    }\
} while (0)

bool global_passed = true;

void fail_tests()
{
    global_passed = false;\
}

#define run_test(test) do {\
    bool passed = test();\
    printf(#test " %s\n", passed ? "passed" : "failed");\
    if (!passed) {\
        fail_tests();\
    }\
} while (0)

int exit_test()
{
    printf("passed: %s\n\n", global_passed ? "yes" : "no");
    return global_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
