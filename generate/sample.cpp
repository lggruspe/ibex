#include <cctype>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

std::pair<bool, std::string> integer()
{
  char c;
  std::vector<char> checkpoint;
  std::string lexeme;
  bool accept = false;
  goto s0;

s0:
  std::cin.get(c);
  lexeme += c;
  checkpoint.push_back(c);
  if (c == '_' || isalpha(c)) {
    goto s1;
  }
  goto s2;

s1:
  std::cin.get(c);
  lexeme += c;
  checkpoint.clear();
  checkpoint.push_back(c);
  accept = true;
  if (c == '_' || isalpha(c) || isdigit(c)) {
    goto s1;
  }
  goto s2;

s2:
  while (!checkpoint.empty()) {
    c = checkpoint.back();
    checkpoint.pop_back();
    std::cin.putback(c);
    lexeme.pop_back();
  }

  return std::pair<bool, std::string>(accept, lexeme);
}

int main()
{
  std::pair<bool, std::string> res = integer();
  std::cout << res.first << " " << res.second << std::endl;
}
