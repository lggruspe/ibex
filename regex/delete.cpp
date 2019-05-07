#include <any>
#include <iostream>
#include <string>
#include <tuple>

int main()
{
  std::tuple<std::any, std::any, std::any> p = std::make_tuple("3.8", 'A', "Lisa Simpson");
  std::cout << std::get<0>(p) << ' ' << std::get<1>(p) << ' ' << std::get<2>(p) << std::endl;
}
