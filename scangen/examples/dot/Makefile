.PHONY:	all
all:	output/scanners.dot scanner.pdf

output/scanners.dot:	templates/template.dot
	python ../example.py template.dot -d templates > $@

scanner.pdf:	output/scanners.dot
	dot -Tpdf $< -o $@	

.PHONY:	clean
clean:
	-rm -rf scanner.pdf
