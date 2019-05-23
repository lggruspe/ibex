#pragma once
#include <iostream>
#include <string>
#include <utility>

struct Scanner {
    std::string token;
    std::istream in;
    Scanner(const std::string& token, const std::istream& in)
    {
        this->token = token;
        this->in = in;
    }

    // category and other functions here
    std::pair<std::string, std::string> operator()
    {
        // put scanner code here
    }
};
