#include "regex.h"
#include "test_regex.h"

int main()
{
  Regex re;
  Re a = re.symbol('a');
  Re b = re.symbol('b');
  Re c = re.symbol('c');
  Re d = re.concatenate(re.alternate(a, b), re.closure(c));

  std::cout << "empty: ";
  print_regex(re.empty);
  std::cout << std::endl;
  print_regex(d);
  std::cout << std::endl;
}
