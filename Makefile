JASPERS_PROGRAMS=bin/hello_world bin/jaspers
ALL=$(JASPERS_PROGRAMS) bin/shell v8/libv8.a

all: $(ALL)

clean:
	@for i in `ls bin/* 2>/dev/null`; do rm $$i; done

v8/libv8.a: v8
	cd v8 && scons

v8:
	if [ -d v8 ]; then cd v8; svn up; else svn checkout http://v8.googlecode.com/svn/trunk/ v8; fi

bin/shell: v8/libv8.a
	g++ -Iv8/include v8/samples/shell.cc -o bin/shell v8/libv8.a -lpthread

$(JASPERS_PROGRAMS): v8/libv8.a
	./build.sh $@
