#include "nfa.h"
#include "thompson.h"
#include "../regex/regex.h"
#include "../regex/utilities.h"
#include "test_nfa.h"

int main()
{
    re::Re integer = re::integer();
    re::Re real = re::real();
    re::Re identifier = re::identifier();

    nfa::Nfa N = nfa::thompson(integer);
    nfa::print(N);

    N = nfa::thompson(real);
    nfa::print(N);

    N = nfa::thompson(identifier);
    nfa::print(N);
}
