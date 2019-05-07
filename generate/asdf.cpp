#include <iostream>
#include <string>
#include <utility>
#include <vector>

std::pair<bool, std::string> scanner()
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
	if (c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F' || c == 'G' || c == 'H' || c == 'I' || c == 'J' || c == 'K' || c == 'L' || c == 'M' || c == 'N' || c == 'O' || c == 'P' || c == 'Q' || c == 'R' || c == 'S' || c == 'T' || c == 'U' || c == 'V' || c == 'W' || c == 'X' || c == 'Y' || c == 'Z' || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g' || c == 'h' || c == 'i' || c == 'j' || c == 'k' || c == 'l' || c == 'm' || c == 'n' || c == 'o' || c == 'p' || c == 'q' || c == 'r' || c == 's' || c == 't' || c == 'u' || c == 'v' || c == 'w' || c == 'x' || c == 'y' || c == 'z') {
		goto s1;
	}
	goto se;

s1:
	std::cin.get(c);
	lexeme += c;
	checkpoint.clear();
	accept = true;
	checkpoint.push_back(c);
	goto se;

se:
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
	std::pair<bool, std::string> res = scanner();
	std::cout << res.first << ' ' << res.second << std::endl;
}

