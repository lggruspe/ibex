grammar = """
start -> expr
expr -> expr pipe term
expr -> term
term -> term factor
term -> factor
factor -> value star
factor -> value plus
factor -> value question
factor -> value
value -> lparen expr rparen
value -> dot
value -> symbol
value -> interval
"""
