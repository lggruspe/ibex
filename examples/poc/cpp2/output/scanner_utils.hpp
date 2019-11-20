#define pardir_output_scanner_utils_hpp() \
std::string lexeme;\
if (scanner_name == "identifier")\
    lexeme = match::single<IdentifierScanner>();\
else if (scanner_name == "whitespace")\
    lexeme = match::single<WhitespaceScanner>();\
else if (scanner_name == "integer")\
    lexeme = match::single<IntegerScanner>();\
else if (scanner_name == "number")\
    lexeme = match::single<NumberScanner>();\
else if (scanner_name == "character")\
    lexeme = match::single<CharacterScanner>();\
else if (scanner_name == "string")\
    lexeme = match::single<StringScanner>();\
else if (scanner_name == "dot")\
    lexeme = match::single<DotScanner>();\
else if (scanner_name == "lparen")\
    lexeme = match::single<LparenScanner>();\
else if (scanner_name == "rparen")\
    lexeme = match::single<RparenScanner>();\
else if (scanner_name == "comma")\
    lexeme = match::single<CommaScanner>();\
else if (scanner_name == "star")\
    lexeme = match::single<StarScanner>();\
else if (scanner_name == "equal")\
    lexeme = match::single<EqualScanner>();\
else if (scanner_name == "lbrace")\
    lexeme = match::single<LbraceScanner>();\
else if (scanner_name == "rbrace")\
    lexeme = match::single<RbraceScanner>();\
else if (scanner_name == "colon")\
    lexeme = match::single<ColonScanner>();\
else if (scanner_name == "lbracket")\
    lexeme = match::single<LbracketScanner>();\
else if (scanner_name == "rbracket")\
    lexeme = match::single<RbracketScanner>();\
else if (scanner_name == "plus")\
    lexeme = match::single<PlusScanner>();\
else if (scanner_name == "minus")\
    lexeme = match::single<MinusScanner>();\
else if (scanner_name == "slash")\
    lexeme = match::single<SlashScanner>();\
else if (scanner_name == "lessthan")\
    lexeme = match::single<LessthanScanner>();\
else if (scanner_name == "greaterthan")\
    lexeme = match::single<GreaterthanScanner>();\
else\
    return;\
std::cout << lexeme << std::endl
