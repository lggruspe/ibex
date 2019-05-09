#include "nfa.h"
#include "thompson.h"
#include "../regex/regex.h"
#include "../regex/utilities.h"
#include "test_nfa.h"

int main()
{
    regex::Re integer = regex::integer();
    regex::Re real = regex::real();
    regex::Re identifier = regex::identifier();

    automata::Nfa N = automata::thompson(integer);
    automata::print(N);

    N = automata::thompson(real);
    automata::print(N);

    N = automata::thompson(identifier);
    automata::print(N);
}
