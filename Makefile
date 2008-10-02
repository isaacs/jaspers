
all: bin/hello_world

bin/hello_world: v8/libv8.a
	g++ -Iv8/include src/hello_world.cpp -o bin/hello_world v8/libv8.a -lpthread

v8/libv8.a: v8
	cd v8 && scons

v8:
	if [ -d v8 ]; then cd v8; svn up; else svn checkout http://v8.googlecode.com/svn/trunk/ v8; fi
