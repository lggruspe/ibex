#include "test.h"

int main()
{
    ScannerCollection scanners;
    integerScanner inscanner("integer");
    floatScanner flscanner("float");
    identifierScanner idscanner("identifier");
    scanners.add_scanner(flscanner);
    scanners.add_scanner(inscanner);
    scanners.add_scanner(idscanner);
    scanners.scan();
}
