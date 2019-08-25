from main import longest_match
from output.scanners import *

token, lexeme = longest_match(
    EmptyScanner(),
    IdentifierScanner(),
    WhitespaceScanner(),
    IntegerScanner(),
    NumberScanner())
    #CharacterScanner())
    #StringScanner())
print(token, f"\"{lexeme}\"")
