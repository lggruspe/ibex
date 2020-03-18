#include "regexp.h"
#include "regexp/eval.hpp"
#include "regexp/parser/parser.hpp"
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>

using namespace scanner;

char const* serialize(const rnd::Automaton& fsm)
{
    std::ostringstream json;
    json << "{" << std::endl;
    json << R"VOGON(    "start": 0,
    "error": )VOGON";
    json << fsm.error << "," << std::endl;
    json << R"VOGON(    "accepts": [)VOGON";

    bool first = true;
    for (const auto& f: fsm.accepts) {
        if (first) {
            first = false;
        } else {
            json << ", ";
        }
        json << f;
    }
    json << "]," << std::endl;

    json << R"VOGON(    "transitions": [)VOGON" << std::endl;
    for (int q = 0; q < (int)(fsm.states.size()); ++q) {
        json << R"VOGON(        [)VOGON" << std::endl;
        bool first = true;
        for (auto it = fsm.states.at(q).begin(); it != fsm.states.at(q).end(); ++it) {
            const auto& [a, r] = *it;
            if (first) {
                first = false;
            } else {
                json << "," << std::endl;
            }
            json << R"VOGON(            [{"start": )VOGON";
            json << a.start << ", \"end\": " << a.end << "}, " << r << "]";
        }
        json << "]";
        if (q != (int)(fsm.states.size()) - 1) {
            json << ",";
        }
        json << std::endl;
    }
    json << R"VOGON(    ])VOGON" << std::endl;
    json << "}";
    return strdup(json.str().c_str());
}

char const* regexp_open(char const* re)
{
    eval::Callback cb;
    bool ok = parser::parse(re, &cb);
    auto m = cb.accept(ok);
    return serialize(m);
}

void regexp_close(void* re)
{
    free(re);
}
