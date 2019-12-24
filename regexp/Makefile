all:	scanner.py scanner.hpp

scanner.py:	tokens.py
	python $< template.py -d python > $@

scanner.hpp:	tokens.py
	python $< template.cpp -d cpp > $@
