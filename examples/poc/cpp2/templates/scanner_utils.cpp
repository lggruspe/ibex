#define pardir_output_scanner_utils_hpp() \
std::string lexeme;\
## for scanner in scanners
## if loop.index == 1
if (scanner_name == "{{ scanner.token }}")\
## else
else if (scanner_name == "{{ scanner.token }}")\
## endif
    lexeme = match::single<{{ scanner.token|title }}>();\
##endfor
else\
    return;\
std::cout << lexeme << std::endl
