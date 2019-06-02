#include <iostream>
#include <unicode/unistr.h>
#include <string>
#include <unicode/schriter.h>
#include <ios>

int main()
{
    icu::UnicodeString str(1120);
    std::string utf8;
    str.toUTF8String(utf8);
    std::cout << utf8 << std::endl;


    std::string bar("ѠѠѠѠѠ");
    //icu::UnicodeString foo("ѠѠѠѠѠ");
    icu::UnicodeString foo(bar.c_str());

    icu::StringCharacterIterator it(foo);
    for (char16_t c = it.first(); c != it.DONE;
            c = it.next()) {
        std::cout << c << std::endl;
    }


    std::string test;
    auto pos = std::cin.tellg();
    std::getline(std::cin, test);
    std::cout << test << std::endl;
    std::cin.seekg(pos, std::ios_base::beg);

    std::cin >> test;
    std::cout << test << std::endl;
}
