#define pardir_output_scanner_utils_hpp() \
bool ok; \
std::string lexeme; \
if (scanner_name == "identifier") \
    std::tie(ok, lexeme) = Identifier().match(); \
else if (scanner_name == "whitespace") \
    std::tie(ok, lexeme) = Whitespace().match(); \
else if (scanner_name == "number") \
    std::tie(ok, lexeme) = Number().match(); \
else if (scanner_name == "character") \
    std::tie(ok, lexeme) = Character().match(); \
else if (scanner_name == "string") \
    std::tie(ok, lexeme) = String().match(); \
else if (scanner_name == "dot") \
    std::tie(ok, lexeme) = Dot().match(); \
else if (scanner_name == "lparen") \
    std::tie(ok, lexeme) = Lparen().match(); \
else if (scanner_name == "rparen") \
    std::tie(ok, lexeme) = Rparen().match(); \
else if (scanner_name == "comma") \
    std::tie(ok, lexeme) = Comma().match(); \
else if (scanner_name == "star") \
    std::tie(ok, lexeme) = Star().match(); \
else if (scanner_name == "equal") \
    std::tie(ok, lexeme) = Equal().match(); \
else if (scanner_name == "lbrace") \
    std::tie(ok, lexeme) = Lbrace().match(); \
else if (scanner_name == "rbrace") \
    std::tie(ok, lexeme) = Rbrace().match(); \
else if (scanner_name == "colon") \
    std::tie(ok, lexeme) = Colon().match(); \
else if (scanner_name == "lbracket") \
    std::tie(ok, lexeme) = Lbracket().match(); \
else if (scanner_name == "rbracket") \
    std::tie(ok, lexeme) = Rbracket().match(); \
else if (scanner_name == "plus") \
    std::tie(ok, lexeme) = Plus().match(); \
else if (scanner_name == "minus") \
    std::tie(ok, lexeme) = Minus().match(); \
else if (scanner_name == "slash") \
    std::tie(ok, lexeme) = Slash().match(); \
else if (scanner_name == "lessthan") \
    std::tie(ok, lexeme) = Lessthan().match(); \
else if (scanner_name == "greaterthan") \
    std::tie(ok, lexeme) = Greaterthan().match(); \
else \
    return; \
std::cout << lexeme << std::endl
