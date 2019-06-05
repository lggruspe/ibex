#include "asdf.h"

int main()
{
    ScannerCollection scanners;
    NumberScanner numscanner(Number);
    IdentifierScanner idscanner(Identifier);
    IgnoreScanner wsscanner(Ignore);
    scanners.add_scanner(numscanner);
    scanners.add_scanner(idscanner);
    scanners.add_scanner(wsscanner);
    scanners.scan();
}
