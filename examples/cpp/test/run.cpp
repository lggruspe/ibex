#include "../output/scanners.hpp"
#include "../output/scanner_utils.hpp"
#include <iostream>
#include <string>
#include <tuple>

void run_longest_match()
{
    auto [token, lexeme] = match_longest<ALL_RECOGNIZERS>();
    std::cout << token << "\n" << lexeme << std::endl;
}

void run_single_match(const std::string& scanner_name)
{
    pardir_output_scanner_utils_hpp();
}

void run_tokenizer()
{
    InputStack in;
    for (;;) {
        auto [token, lexeme] = match_longest<ALL_RECOGNIZERS>(in);
        if (token == Token::EMPTY || token == Token::ERROR) {
            break;
        }
        std::cout << token << " " << lexeme << std::endl;
    }
}

int main(int argc, char **argv)
{
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    if (args.size() < 1) {
        std::cerr << "usage: run {single,longest,tokenizer} ..." << std::endl;
        return EXIT_FAILURE;
    }
    if (args[0] == "single") {
        if (args.size() < 3) {
            std::cerr << "usage: run single -s SCANNER" << std::endl;
            return EXIT_FAILURE;
        }
        run_single_match(args[2]);
    } else if (args[0] == "longest") {
        run_longest_match();
    } else if (args[0] == "tokenizer") {
        run_tokenizer();
    }
}
