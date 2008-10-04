v8::Handle<v8::Value> Version(const v8::Arguments& args) {
	return v8::String::New(v8::V8::GetVersion());
}
