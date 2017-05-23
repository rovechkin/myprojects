// support the following JS functions
// dir(path) - enumerate files in dir

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "libplatform/libplatform.h"
#include "v8.h"

using namespace v8;
using namespace std;




Local<Context> CreateContext(Isolate* isolate);

static void enumDir(const char* path){
		DIR *dir;
		struct dirent *ent;
		vector<string> ret;
		if ((dir = opendir (path)) != NULL) {
		  /* print all the files and directories within directory */
		  while ((ent = readdir (dir)) != NULL) {
			ret.push_back(ent->d_name);
		  }
		  closedir (dir);
		} else {
		  /* could not open directory */
		  perror ("error opening dir");
		  return;
		}

		for (const auto& d:ret){
		cout<<d<<endl;
		}



}


static void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
	if (args.Length()<1) return;
	HandleScope scope(args.GetIsolate());
	Local<Value> arg0=args[0];
	String::Utf8Value v(arg0);
	cout<<*v<<endl;
}
static void DirCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
	if (args.Length()<1) return;
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(args.GetIsolate());
	Local<Value> arg0=args[0];
	String::Utf8Value pathname(arg0);
	struct stat info;
	if( stat( *pathname, &info ) != 0 ){
		stringstream ss;
		ss<<*pathname<<" does not exists";
		isolate->ThrowException(String::NewFromUtf8(isolate, ss.str().c_str()));
		return;
	}
	else if( !(info.st_mode & S_IFDIR) )  {
		stringstream ss;
		ss<<*pathname<<" is not directory";
		isolate->ThrowException(String::NewFromUtf8(isolate, ss.str().c_str()));
		return;
	} 
	cout<<*pathname<<endl;
}
int main(int argc, char* argv[]) {

	int i = 1;
	string cmd;
	string arg1;
	if (argc>1){
		if(string(argv[i++]) == "-f" && i < argc){
			cmd = "f";
			arg1 = argv[i++];
		} else {
			cerr<<"invalid args"<<endl;
			return 1;
		}
	}


  // Initialize V8.
  V8::InitializeICUDefaultLocation(argv[0]);
  V8::InitializeExternalStartupData(argv[0]);
  Platform* platform = platform::CreateDefaultPlatform();
  V8::InitializePlatform(platform);
  V8::Initialize();
  // Create a new Isolate and make it the current one.
  Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  Isolate* isolate = Isolate::New(create_params);
  {
    Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);
    // Create a new context.
    Local<Context> context = CreateContext(isolate);
    // Enter the context for compiling and running the hello world script.
    Context::Scope context_scope(context);
    // Create a string containing the JavaScript source code.
	
	stringstream ss;
	if (cmd == "f"){
		fstream file(arg1);
		string c;
		while(file >> c) {
			ss<<c<<' ';
		}
  		cout<<ss.str()<<endl;
	} else {
    	// read stdi
    	for (string line; getline(cin, line);) {
        	ss << line;
    	}
		
  		cout<<ss.str()<<endl;
	}

    Local<String> source = 
        String::NewFromUtf8(isolate, ss.str().c_str(),
                            NewStringType::kNormal).ToLocalChecked();

    // Compile the source code.
    Local<Script> script = Script::Compile(context, source).ToLocalChecked();
    // Run the script to get the result.
	TryCatch trycatch(isolate);
    MaybeLocal<Value> result = script->Run(context);
	if (result.IsEmpty()) {
		  Local<Value> exception = trycatch.Exception();
		  String::Utf8Value exc(exception);
		  cout<<"Uncaught exception: "<<*exc<<endl;
	} else {
    	Local<Value> rr = result.ToLocalChecked(); 

    	// Convert the result to an UTF8 string and print it.
    	String::Utf8Value utf8(rr);
    	printf("%s\n", *utf8);
	}
  }
  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  V8::Dispose();
  V8::ShutdownPlatform();
  delete platform;
  delete create_params.array_buffer_allocator;
  return 0;
}



Local<Context> CreateContext(Isolate* isolate){
	// context for global funcitons
	Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
	// log function
	global->Set(String::NewFromUtf8(isolate, "log", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate, LogCallback));

	// dir function
	global->Set(String::NewFromUtf8(isolate, "dir", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate, DirCallback));

	Local<Context> ctxt = Context::New(isolate,NULL,global);
	return ctxt;
}
