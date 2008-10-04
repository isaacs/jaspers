#!/bin/sh
cc="$1"
out=$cc
out=${out/\.cc/}
out=${out/src/bin}
if [ $out == $cc ]; then
	cc=${out/bin/src}.cc
fi
g++ -Iv8/include $cc -o $out v8/libv8.a -lpthread
