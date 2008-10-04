#include <v8.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

bool ExecuteString(v8::Handle<v8::String> source,
									 v8::Handle<v8::Value> name,
									 bool print_result,
									 bool report_exceptions);
								
v8::Handle<v8::String> ReadFile(const char* name);
								
// Reads a file into a v8 string.



