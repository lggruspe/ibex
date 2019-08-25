#include "../output/scanners.h"
#include "vector.h"
#include "swlist/swlist.h"

vector_register(int)

typedef struct scanner scanner;
sw_register(scanner)

int main()
{
    struct vector_int *lexeme = NULL;
    struct sw_list_scanner *scanners = NULL;
    scanners = sw_push(scanners, sw_create(scanner_create(token_STRING, transition_string)));
    scanners = sw_push(scanners, sw_create(scanner_create(token_CHARACTER, transition_character)));
    scanners = sw_push(scanners, sw_create(scanner_create(token_NUMBER, transition_number)));
    scanners = sw_push(scanners, sw_create(scanner_create(token_INTEGER, transition_integer)));
    scanners = sw_push(scanners, sw_create(scanner_create(token_WHITESPACE, transition_whitespace)));
    scanners = sw_push(scanners, sw_create(scanner_create(token_IDENTIFIER, transition_identifier)));
    scanners = sw_push(scanners, sw_create(scanner_create(token_EMPTY, transition_empty)));
    sw_destroy(scanners);
}
