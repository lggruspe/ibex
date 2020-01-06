#include "dfa.hpp"
#include <sstream>
#include <string>

std::string state_code(const Fsm& fsm) 
{
    std::string pad(4, ' ');
    std::stringstream code;
    for (const auto& [q, dq]: fsm.transitions) {
        code << pad;
        code << "q" << q << " [shape=";
        if (fsm.accepts.find(q) != fsm.accepts.end()) {
            code << "double";
        }
        code << "circle,label=" << q << "];\n";
    }
    return code.str();
}

std::string transition_code(const Fsm& fsm)
{
    std::string pad(4, ' ');
    std::stringstream code;
    for (const auto& [q, dq]: fsm.transitions) {
        for (const auto& [a, r]: dq) {
            code << pad;
            code << "q" << q << " -> q" << r << " [label=\"";
            if (a.start != a.end) {
                code << "[" << (char)(a.start) << ", " << (char)(a.end) << "]";
            } else {
                code << (char)(a.start);
            }
            code << "\"];\n";
        }
    }
    return code.str();
}

std::string render_template(
    const std::string& template_, 
    const std::string& states,
    const std::string& transitions)
{
    std::string code = template_;
    int i = code.find("{states}");
    code.replace(i, 8, states);
    i = code.find("{transitions}", i+8);
    code.replace(i, 13, transitions);
    return code;
}

std::string dotify(const Fsm& fsm)
{
    std::string template_ = R"VOGON(digraph {
    rankdir=LR
    start [shape=plain,label=start];
{states}    start -> q0;
{transitions}})VOGON";
    return render_template(
        template_,
        state_code(fsm),
        transition_code(fsm));
}
