#include "grammar.h"
#include "lr1.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> split_string(const std::string& s, char delim=' ')
{
    struct pred_not_delim {
        pred_not_delim(char delim) : delim(delim) {}
        bool operator()(char c) const { return c != delim; }
    private:
        char delim;
    };
    
    std::vector<std::string> res;
    auto it = s.begin();
    auto jt = s.begin();
    while (it != s.end()) {
        it = std::find_if(jt, s.end(), pred_not_delim(delim));
        jt = std::find(it, s.end(), delim);
        if (it != jt) {
            res.push_back(std::string(it, jt));
        }
    }
    return res;
}

class Parser {
    lr1::Parser parser;
public:
    Parser(const char* filename) {
        cfg::Grammar grammar;
        grammar.add_terminal("");

        std::string line;

        std::ifstream file(filename);
        if (file.is_open()) {
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    std::vector<std::string> rule = split_string(line);
                    cfg::Symbol lhs = rule[0];
                    cfg::Sentence rhs(rule.begin() + 1, rule.end());
                    grammar.add_variable(lhs);
                    grammar.add_rule(lhs, rhs);
                }
            }
            file.close();
        }

        // add special start state
        // add terminal states
        
        parser.construct_parser(grammar);
    }

    void parse(const char* filename) {}
};

int main()
{
    Parser parser("sample.grammar");
}
