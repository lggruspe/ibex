#define pardir_output_scanner_utils_hpp() \
std::string lexeme;\
if (scanner_name == "identifier")\
    lexeme = match::single<Identifier>();\
else if (scanner_name == "whitespace")\
    lexeme = match::single<Whitespace>();\
else if (scanner_name == "integer")\
    lexeme = match::single<Integer>();\
else if (scanner_name == "number")\
    lexeme = match::single<Number>();\
else if (scanner_name == "character")\
    lexeme = match::single<Character>();\
else if (scanner_name == "string")\
    lexeme = match::single<String>();\
else if (scanner_name == "dot")\
    lexeme = match::single<Dot>();\
else if (scanner_name == "lparen")\
    lexeme = match::single<Lparen>();\
else if (scanner_name == "rparen")\
    lexeme = match::single<Rparen>();\
else if (scanner_name == "comma")\
    lexeme = match::single<Comma>();\
else if (scanner_name == "star")\
    lexeme = match::single<Star>();\
else if (scanner_name == "equal")\
    lexeme = match::single<Equal>();\
else if (scanner_name == "lbrace")\
    lexeme = match::single<Lbrace>();\
else if (scanner_name == "rbrace")\
    lexeme = match::single<Rbrace>();\
else if (scanner_name == "colon")\
    lexeme = match::single<Colon>();\
else if (scanner_name == "lbracket")\
    lexeme = match::single<Lbracket>();\
else if (scanner_name == "rbracket")\
    lexeme = match::single<Rbracket>();\
else if (scanner_name == "plus")\
    lexeme = match::single<Plus>();\
else if (scanner_name == "minus")\
    lexeme = match::single<Minus>();\
else if (scanner_name == "slash")\
    lexeme = match::single<Slash>();\
else if (scanner_name == "lessthan")\
    lexeme = match::single<Lessthan>();\
else if (scanner_name == "greaterthan")\
    lexeme = match::single<Greaterthan>();\
else\
    return;\
std::cout << lexeme << std::endl
