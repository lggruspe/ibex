.PHONY:	all
all:	output/scanner.dot scanner.pdf

output/scanner.dot:	templates/template.dot
	python ../example.py template.dot -d templates > $@

scanner.pdf:	output/scanner.dot
	dot -Tpdf $< -o $@	

.PHONY:	clean
clean:
	-rm -rf scanner.pdf
