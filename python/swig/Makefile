CXX = g++

rnd_wrap.cxx:	rnd.i
	@swig -python -c++ rnd.i;\
	$(CXX) -c -fpic rnd_wrap.cxx -I/usr/include/python3.7m;\
	$(CXX) -shared rnd_wrap.o ../build/*.o -o _rnd.so

clean:
	@rm -r -f rnd_wrap.cxx _rnd.so rnd_wrap.o __pycache__ rnd.py
