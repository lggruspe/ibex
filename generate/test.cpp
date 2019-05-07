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
	goto s1;

s0:
	std::cin.get(c);
	lexeme += c;
	checkpoint.push_back(c);
	goto se;

s1:
	std::cin.get(c);
	lexeme += c;
	checkpoint.push_back(c);
	if (c == '0') {
		goto s4;
	}
	if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') {
		goto s6;
	}
	goto se;

s2:
	std::cin.get(c);
	lexeme += c;
	checkpoint.push_back(c);
	goto se;

s3:
	std::cin.get(c);
	lexeme += c;
	checkpoint.push_back(c);
	goto se;

s4:
	std::cin.get(c);
	lexeme += c;
	checkpoint.clear();
	accept = true;
	checkpoint.push_back(c);
	if (c == '.') {
		goto s7;
	}
	if (c == 'E' || c == 'e') {
		goto s8;
	}
	goto se;

s5:
	std::cin.get(c);
	lexeme += c;
	checkpoint.push_back(c);
	goto se;

s6:
	std::cin.get(c);
	lexeme += c;
	checkpoint.clear();
	accept = true;
	checkpoint.push_back(c);
	if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') {
		goto s6;
	}
	if (c == '.') {
		goto s7;
	}
	if (c == 'E' || c == 'e') {
		goto s8;
	}
	goto se;

s7:
	std::cin.get(c);
	lexeme += c;
	checkpoint.push_back(c);
	if (c == '0') {
		goto s11;
	}
	if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') {
		goto s12;
	}
	goto se;

s8:
	std::cin.get(c);
	lexeme += c;
	checkpoint.push_back(c);
	if (c == '+' || c == '-') {
		goto s7;
	}
	if (c == '0') {
		goto s11;
	}
	if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') {
		goto s12;
	}
	goto se;

s9:
	std::cin.get(c);
	lexeme += c;
	checkpoint.push_back(c);
	goto se;

s10:
	std::cin.get(c);
	lexeme += c;
	checkpoint.clear();
	accept = true;
	checkpoint.push_back(c);
	if (c == 'E' || c == 'e') {
		goto s8;
	}
	if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') {
		goto s10;
	}
	goto se;

s11:
	std::cin.get(c);
	lexeme += c;
	checkpoint.clear();
	accept = true;
	checkpoint.push_back(c);
	goto se;

s12:
	std::cin.get(c);
	lexeme += c;
	checkpoint.clear();
	accept = true;
	checkpoint.push_back(c);
	if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9') {
		goto s12;
	}
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

