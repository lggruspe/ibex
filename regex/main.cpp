#include "regex.h"
#include "utilities.h"
#include "test_regex.h"
#include <iostream>

int main()
{
  re_load_predefined();
  print_regex(real);
  std::cout << std::endl;
}
