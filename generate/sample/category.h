#pragma once
#include <string>

bool contains(const char *s, char c)
{
	auto it = s;
	while (*it && *it != c) {
		++it;
	}
	return *it == c;
}

std::string category(char c)
{
	if (contains(".", c)) {
		return ".";
	}
	if (contains("eE", c)) {
		return "exponent";
	}
	if (contains("123456789", c)) {
		return "nonzero";
	}
	if (contains("+-", c)) {
		return "sign";
	}
	if (contains("0", c)) {
		return "zero";
	}
	return "other";
}

