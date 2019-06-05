#include "todo.h"

int main()
{
    ScannerCollection scanners;
    integerScanner inscanner(Integer);
    floatScanner flscanner(Number);
    identifierScanner idscanner(Identifier);
    whitespaceScanner wsscanner(Ignore);
    scanners.add_scanner(flscanner);
    scanners.add_scanner(inscanner);
    scanners.add_scanner(idscanner);
    scanners.add_scanner(wsscanner);
    scanners.scan();
}
