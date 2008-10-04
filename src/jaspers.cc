// Copyright 2008 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//		 * Redistributions of source code must retain the above copyright
//			 notice, this list of conditions and the following disclaimer.
//		 * Redistributions in binary form must reproduce the above
//			 copyright notice, this list of conditions and the following
//			 disclaimer in the documentation and/or other materials provided
//			 with the distribution.
//		 * Neither the name of Google Inc. nor the names of its
//			 contributors may be used to endorse or promote products derived
//			 from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <v8.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "commands/commands.h"
#include "parser/parser.h"

void RunShell(v8::Handle<v8::Context> context);
// bool ExecuteString(v8::Handle<v8::String> source,
// 									 v8::Handle<v8::Value> name,
// 									 bool print_result,
// 									 bool report_exceptions);
// v8::Handle<v8::Value> Print(const v8::Arguments& args);
// v8::Handle<v8::Value> Load(const v8::Arguments& args);
// 
// 
// v8::Handle<v8::Value> Version(const v8::Arguments& args);
// v8::Handle<v8::String> ReadFile(const char* name);
void ReportException(v8::TryCatch* handler);


int main(int argc, char* argv[]) {
	v8::V8::SetFlagsFromCommandLine(&argc, argv, true);
	v8::HandleScope handle_scope;
	// Create a template for the global object.
	v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
	
	loadCommands(global);
	
	// global->Set(v8::String::New("server"), global);
	
	global->Set(v8::String::New("foo"), v8::String::New("bar"));

	
	// Create a new execution environment containing the built-in
	// functions
	v8::Handle<v8::Context> context = v8::Context::New(NULL, global);
	// Enter the newly created execution environment.
	v8::Context::Scope context_scope(context);
	
	ExecuteString(v8::String::New("this.server = this"),
								v8::String::New("(shell)"),
								false,
								true);
	
	bool run_shell = (argc == 1);
	for (int i = 1; i < argc; i++) {
		const char* str = argv[i];
		if (strcmp(str, "--shell") == 0) {
			run_shell = true;
		} else if (strcmp(str, "-f") == 0) {
			// Ignore any -f flags for compatibility with the other stand-
			// alone JavaScript engines.
			continue;
		} else if (strncmp(str, "--", 2) == 0) {
			printf("Warning: unknown flag %s.\n", str);
		} else {
			// Use all other arguments as names of files to load and run.
			v8::HandleScope handle_scope;
			v8::Handle<v8::String> file_name = v8::String::New(str);
			v8::Handle<v8::String> source = ReadFile(str);
			if (source.IsEmpty()) {
				printf("Error reading '%s'\n", str);
				return 1;
			}
			if (!ExecuteString(source, file_name, false, true))
				return 1;
		}
	}
	if (run_shell) RunShell(context);
	return 0;
}





// Executes a string within the current v8 context.
bool ExecuteString(v8::Handle<v8::String> source,
									 v8::Handle<v8::Value> name,
									 bool print_result,
									 bool report_exceptions) {
	v8::HandleScope handle_scope;
	v8::TryCatch try_catch;
	v8::Handle<v8::Script> script = v8::Script::Compile(source, name);
	if (script.IsEmpty()) {
		// Print errors that happened during compilation.
		if (report_exceptions)
			ReportException(&try_catch);
		return false;
	} else {
		v8::Handle<v8::Value> result = script->Run();
		if (result.IsEmpty()) {
			// Print errors that happened during execution.
			if (report_exceptions)
				ReportException(&try_catch);
			return false;
		} else {
			if (print_result && !result->IsUndefined()) {
				// If all went well and the result wasn't undefined then print
				// the returned value.
				v8::String::Utf8Value str(result);
				printf("%s\n", *str);
			}
			return true;
		}
	}
}



// The read-eval-execute loop of the shell.
void RunShell(v8::Handle<v8::Context> context) {
	printf("V8 version %s\n", v8::V8::GetVersion());
	static const int kBufferSize = 256;
	while (true) {
		char buffer[kBufferSize];
		printf("> ");
		char* str = fgets(buffer, kBufferSize, stdin);
		if (str == NULL) break;
		v8::HandleScope handle_scope;
		ExecuteString(v8::String::New(str),
									v8::String::New("(shell)"),
									true,
									true);
	}
	printf("\n");
}



void ReportException(v8::TryCatch* try_catch) {
	v8::HandleScope handle_scope;
	v8::String::Utf8Value exception(try_catch->Exception());
	v8::Handle<v8::Message> message = try_catch->Message();
	if (message.IsEmpty()) {
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		printf("%s\n", *exception);
	} else {
		// Print (filename):(line number): (message).
		v8::String::Utf8Value filename(message->GetScriptResourceName());
		int linenum = message->GetLineNumber();
		printf("%s:%i: %s\n", *filename, linenum, *exception);
		// Print line of source code.
		v8::String::Utf8Value sourceline(message->GetSourceLine());
		printf("%s\n", *sourceline);
		// Print wavy underline (GetUnderline is deprecated).
		int start = message->GetStartColumn();
		for (int i = 0; i < start; i++) {
			printf(" ");
		}
		int end = message->GetEndColumn();
		for (int i = start; i < end; i++) {
			printf("^");
		}
		printf("\n");
	}
}

v8::Handle<v8::String> ReadFile(const char* name) {
	FILE* file = fopen(name, "rb");
	if (file == NULL) return v8::Handle<v8::String>();

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);

	char* chars = new char[size + 1];
	chars[size] = '\0';
	for (int i = 0; i < size;) {
		int read = fread(&chars[i], 1, size - i, file);
		i += read;
	}
	fclose(file);
	v8::Handle<v8::String> result = v8::String::New(chars, size);
	delete[] chars;
	return result;
}
