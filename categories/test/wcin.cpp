#include <iostream>
#include <string>

int main()
{
    std::string s;
    /*
    wchar_t c = std::wcin.get();
    s += c;
    */

    std::getline(std::cin, s);
    std::cout << s << std::endl;

    for (auto c: s) {
        std::cout << c << std::endl;
    }
}
