// The callback that is invoked by v8 whenever the JavaScript 'load'
// function is called.	Loads, compiles and executes its argument
// JavaScript file.
v8::Handle<v8::String> ReadFile(const char* name);

v8::Handle<v8::Value> Load(const v8::Arguments& args) {
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope handle_scope;
		v8::String::Utf8Value file(args[i]);
		v8::Handle<v8::String> source = ReadFile(*file);
		if (source.IsEmpty()) {
			return v8::ThrowException(v8::String::New("Error loading file"));
		}
		if (!ExecuteString(source, v8::String::New(*file), false, false)) {
			return v8::ThrowException(v8::String::New("Error executing	file"));
		}
	}
	return v8::Undefined();
}
