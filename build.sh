#!/bin/sh
cpp="$1"
out=$cpp
out=${out/\.cpp/}
out=${out/\.cc/}
out=${out/src/bin}
if [ $out == $cpp ]; then
	cpp=${out/bin/src}.cpp
fi
g++ -Iv8/include $cpp -o $out v8/libv8.a -lpthread

