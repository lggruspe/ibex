#include "parser.h"

/* list -> list pair
 * list -> pair
 * pair -> ( pair )
 * pair -> ()
 */

int main()
{
    Parser parser("grammar.txt");
    parser.parse("sample.txt");
}
