CXX = g++

tree_wrap.cxx:	tree.i
	@swig -python -c++ tree.i;\
	$(CXX) -c -fpic tree_wrap.cxx -I/usr/include/python3.7m;\
	$(CXX) -shared tree_wrap.o -o _tree.so

clean:
	@rm -r -f tree_wrap.cxx _tree.so tree_wrap.o __pycache__
