from py.main import longest_match
from build.example import *

token, lexeme = longest_match(
    EmptyScanner(),
    IdentifierScanner(),
    WhitespaceScanner(),
    IntegerScanner(),
    NumberScanner())
    #CharacterScanner())
    #StringScanner())
print(token, lexeme)
