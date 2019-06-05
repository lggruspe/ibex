#include "asdf.h"

int main()
{
    ScannerCollection scanners;
    numberScanner numscanner(Number);
    identifierScanner idscanner(Identifier);
    whitespaceScanner wsscanner(Ignore);
    scanners.add_scanner(numscanner);
    scanners.add_scanner(idscanner);
    scanners.add_scanner(wsscanner);
    scanners.scan();
}
