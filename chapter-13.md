## Chapter 13: Demystifying C++ IO

The rest of this section deals with console streams (`cin` and `cout`). Examples of file and string streams are provided later in this chapter. Other types of streams, such as printer output or network IO, are platform dependent, so they are not covered.

The `<iostream>` header also declares all predefined stream instances: `cout`, `cin`, `cerr`, `clog`, and the wide versions.

Handling Output Errors

```cpp
cout.flush();
if (cout.fail()) {
    cerr << "Unable to flush to standard out" << endl;
}
```

You can also tell the streams to throw exceptions when a failure occurs. 

```cpp
cout.exceptions(ios::failbit | ios::badbit | ios::eofbit);
try {
    cout << "Hello World." << endl;
} catch (const ios_base::failure& ex) {
    cerr << "Caught exception: " << ex.what()
         << ", error code = " << ex.code() << endl;
}
```

To reset the error state of a stream, use the `clear()` method:

```cpp
cout.clear();
```

Output Manipulators

The `seek()` methods let you move to an arbitrary position within an input or output stream. The methods of `seek()` within an input stream are actually called `seekg()` (the g is for get), and the versions of `seek()` in an output stream are called `seekp()` (the p is for put). 