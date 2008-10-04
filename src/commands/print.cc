// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.	Prints its arguments on stdout separated by
// spaces and ending with a newline.
v8::Handle<v8::Value> Print(const v8::Arguments& args) {
	bool first = true;
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope handle_scope;
		if (first) {
			first = false;
		} else {
			printf(" ");
		}
		v8::String::Utf8Value str(args[i]);
		printf("%s", *str);
	}
	printf("\n");
	return v8::Undefined();
}

