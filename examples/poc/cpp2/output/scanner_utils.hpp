#define pardir_output_scanner_utils_hpp() \
std::string lexeme;\
if (scanner_name == "identifier")\
    lexeme = Identifier().match();\
else if (scanner_name == "whitespace")\
    lexeme = Whitespace().match();\
else if (scanner_name == "integer")\
    lexeme = Integer().match();\
else if (scanner_name == "number")\
    lexeme = Number().match();\
else if (scanner_name == "character")\
    lexeme = Character().match();\
else if (scanner_name == "string")\
    lexeme = String().match();\
else if (scanner_name == "dot")\
    lexeme = Dot().match();\
else if (scanner_name == "lparen")\
    lexeme = Lparen().match();\
else if (scanner_name == "rparen")\
    lexeme = Rparen().match();\
else if (scanner_name == "comma")\
    lexeme = Comma().match();\
else if (scanner_name == "star")\
    lexeme = Star().match();\
else if (scanner_name == "equal")\
    lexeme = Equal().match();\
else if (scanner_name == "lbrace")\
    lexeme = Lbrace().match();\
else if (scanner_name == "rbrace")\
    lexeme = Rbrace().match();\
else if (scanner_name == "colon")\
    lexeme = Colon().match();\
else if (scanner_name == "lbracket")\
    lexeme = Lbracket().match();\
else if (scanner_name == "rbracket")\
    lexeme = Rbracket().match();\
else if (scanner_name == "plus")\
    lexeme = Plus().match();\
else if (scanner_name == "minus")\
    lexeme = Minus().match();\
else if (scanner_name == "slash")\
    lexeme = Slash().match();\
else if (scanner_name == "lessthan")\
    lexeme = Lessthan().match();\
else if (scanner_name == "greaterthan")\
    lexeme = Greaterthan().match();\
else\
    return;\
std::cout << lexeme << std::endl
