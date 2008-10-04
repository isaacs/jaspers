#include <v8.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "../parser/parser.h"
#include "quit.cc"
#include "print.cc"
#include "load.cc"
#include "version.cc"

void loadCommands (v8::Handle<v8::ObjectTemplate> global) {
	
	// Bind the global 'print' function to the C++ Print callback.
	global->Set(v8::String::New("print"), v8::FunctionTemplate::New(Print));
	// Bind the global 'load' function to the C++ Load callback.
	global->Set(v8::String::New("load"), v8::FunctionTemplate::New(Load));
	// Bind the 'quit' function
	global->Set(v8::String::New("quit"), v8::FunctionTemplate::New(Quit));
	// Bind the 'version' function
	global->Set(v8::String::New("version"), v8::FunctionTemplate::New(Version));
	
}