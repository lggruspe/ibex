#include "category.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>

std::pair<bool, std::string> scanner()
{
	char c;
	std::string cat;
	std::vector<char> checkpoint;
	std::string lexeme;
	bool accept = false;
	goto s1;

s0:
	std::cin.get(c);
	lexeme += c;
	checkpoint.clear();
	accept = true;
	checkpoint.push_back(c);
	cat = category(c);
	if (cat == "nonzero" || cat == "zero") {
		goto s0;
	}
	if (cat == "exponent") {
		goto s7;
	}
	goto se;

s1:
	std::cin.get(c);
	lexeme += c;
	checkpoint.push_back(c);
	cat = category(c);
	if (cat == "nonzero") {
		goto s2;
	}
	if (cat == "zero") {
		goto s3;
	}
	goto se;

s2:
	std::cin.get(c);
	lexeme += c;
	checkpoint.clear();
	accept = true;
	checkpoint.push_back(c);
	cat = category(c);
	if (cat == "nonzero" || cat == "zero") {
		goto s2;
	}
	if (cat == ".") {
		goto s6;
	}
	if (cat == "exponent") {
		goto s7;
	}
	goto se;

s3:
	std::cin.get(c);
	lexeme += c;
	checkpoint.clear();
	accept = true;
	checkpoint.push_back(c);
	cat = category(c);
	if (cat == ".") {
		goto s6;
	}
	if (cat == "exponent") {
		goto s7;
	}
	goto se;

s4:
	std::cin.get(c);
	lexeme += c;
	checkpoint.clear();
	accept = true;
	checkpoint.push_back(c);
	cat = category(c);
	if (cat == "nonzero" || cat == "zero") {
		goto s4;
	}
	goto se;

s5:
	std::cin.get(c);
	lexeme += c;
	checkpoint.clear();
	accept = true;
	checkpoint.push_back(c);
	cat = category(c);
	goto se;

s6:
	std::cin.get(c);
	lexeme += c;
	checkpoint.push_back(c);
	cat = category(c);
	if (cat == "nonzero" || cat == "zero") {
		goto s0;
	}
	goto se;

s7:
	std::cin.get(c);
	lexeme += c;
	checkpoint.push_back(c);
	cat = category(c);
	if (cat == "nonzero") {
		goto s4;
	}
	if (cat == "zero") {
		goto s5;
	}
	if (cat == "sign") {
		goto s8;
	}
	goto se;

s8:
	std::cin.get(c);
	lexeme += c;
	checkpoint.push_back(c);
	cat = category(c);
	if (cat == "nonzero") {
		goto s4;
	}
	if (cat == "zero") {
		goto s5;
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

