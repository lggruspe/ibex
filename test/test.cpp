#include <rnd/regex.h>
#include <rnd/nfa.h>
#include <rnd/dfa.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace regex;
using namespace automata;

struct TestCase {
    std::vector<std::pair<std::string, bool>> test_case;

    TestCase(const char* filename)
    {
        std::string line;
        std::ifstream file(filename);

        if (file.is_open()) {
            while (std::getline(file, line)) {
                append(line);
            }
            file.close();
        }
    }

private:
    void append(const std::string& line)
    {
        auto i = line.find(',');
        auto instance = line.substr(0, i);
        bool label = (line[i+1] == '1' ? true : false);
        test_case.push_back({instance, label});

        std::cout << "TEST CASE: " << instance << " " << label << std::endl;
    }
};

Dfa compile(Expr expr)
{
    return Dfa(Nfa(expr));
}

bool matches(const Dfa& dfa, const std::string& word)
{
    int state = dfa.start;
    for (char c: word) {
        try {
            state = dfa.delta.at(state).at(c);
        } catch (std::out_of_range e) {
            return false;
        }
    }
    bool passed = dfa.accept.count(state);
    return passed;
}

bool test_dfa(const Dfa& dfa, const TestCase& test_case)
{
    for (const auto& [instance, label]: test_case.test_case) 
    {
        if (matches(dfa, instance) != label) {
            return false;
        }
    }
    return true;
}

bool test_regex(Expr re, const char* filename)
{
    auto dfa = compile(re);
    auto test_case = TestCase(filename);
    return test_dfa(dfa, test_case);
}

bool test_empty()
{
    auto empty = symbol('\0');
    return test_regex(empty, "tests/empty.csv");
}

bool test_number()
{
    auto integer = symbol('0') | (symbol('1', '9') + closure(symbol('0', '9')));
    // TODO
    return test_regex(integer, "tests/number.csv");
}

bool test_whitespace()
{
    auto whitespace = symbol(' ') | symbol('\t') | symbol('\n');
    return test_regex(whitespace, "tests/whitespace.csv");
}

bool test_identifier()
{
    auto alpha = symbol('_') | symbol('a', 'z') | symbol('A', 'Z');
    auto identifier = alpha + closure(alpha | symbol('0','9'));
    return test_regex(identifier, "tests/identifier.csv");
}

int main()
{
    std::vector<bool (*)(void)> tests = {
        test_empty, test_number, test_whitespace, test_identifier
    };

    int total = 0;
    for (int i = 0; i < tests.size(); ++i) {
        auto test = tests[i];
        bool passed = test();
        std::cout << "Test " << i << " "
            << (passed ? "passed" : "failed")
            << std::endl;
        if (passed) ++total;
    }

    std::cout << "Total: " << total << " tests passed" << std::endl;
}
