.PHONY:	package
package:
	-rm -rf build dist codegen.egg-info
	python setup.py sdist bdist_wheel
	twine upload dist/*

.PHONY:	clean
clean:
	-rm -rf build dist codegen.egg-info
