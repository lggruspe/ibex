.PHONY:	package
package:
	-rm -rf build dist scangen.egg-info
	python setup.py sdist bdist_wheel
	twine upload dist/*
