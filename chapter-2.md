## Chapter 2: Working with Strings and String Views

The `sizeof()` operator in C and C++ can be used to get the size of a certain data type or variable. For example, `sizeof(char)` returns 1 because a `char` has a size of 1 byte. However in the context of C-style strings, `sizeof()` is not the same as `strlen()`. You should never use `sizeof()` to try to get the size of the string. It returns different sizes depending on how the C-style string is stored. If it is stored as a `char[]`, the `sizeof()` returns the actual memory used by the string, including the `\0` character. 

```cpp
char text1[] = "abcdef";
size_t s1 = sizeof(text1); // is 7
size_t s2 = strlen(text1); // is 6
```

However if the C-style string is stored as a `char*`, then `sizeof()` returns the size of a pointer!

```cpp
const char* text2 = "abcdef";
size_t s3 = sizeof(text2); // is platform-dependent
size_t s4 = strlen(text2); // is 6
```

Here `s3` will be 4 when compiled in 32-bit mode, and 8 when compiled in 64-bit mode because it is returning the size of a `const char*`, which is a pointer.

```cpp
cout << "hello" << endl;
```

In the preceding line, `"hello"` is a _string literal_ because it is written as a value, not a variable. String literals are actually stored in a read-only part of memory. This allows the compiler to optimize memory usage by reusing references to equivalent string literals. That is, even if your program uses the string literal `"hello"` 500 times, the compiler is allowed to create just one instance of `hello` in memory. This is called `literal pooling`.

String literals can be _assigned_ to variables, but because string literals are in a read-only part of memory and because of the possibility of literal pooling, assigning them to variables can be risky. The C++ standard officialy says that string literals are of type "array of n `const char`"; however, for backward compatibility with older non-`const`-aware code, most compilers do not enforce your program to assign a string literal to a variable type `const char*`. They let you assign a string literal to a `char*` without `const`, and the program will work fine unless you attempt to change the string. The following code exhibits undefined behaviour:

```cpp
char* ptr = "hello"; // Assign the string literal to a variable.
ptr[1] = 'a'; // Undefined behavior!
```

A much safer way to code is to use a pointer to `const` characters when referring to string literals. The following code contains the same bug, but because it assigned the literal to a `const char*`, the compiler catches the attempt to write to read-only memory:

```cpp
const char* ptr = "hello"; // Assign the string literal to a variable.
ptr[1] = 'a'; // Error! Attempts to write to read-only memory
```

You can also use a string literal as an initial value for a character array (`char[]`). In this case, the compiler creates an array that is big enough to hold the string and copies the string to this array. The compiler does not put the literal in read-only memory and does not do any literal pooling.

```cpp
char arr[] = "hello"; // Compiler takes care of creating appropriate sized
                      // character array arr.
arr[1] = 'a'; // The contents can be modified.
```

_Raw string literals_ are string literals that can span across multiple lines of code, that don't require escaping of embedded double quotes, and where escape sequences like `\t` and `\n` are processed as normal text and not as escape sequences. The following examples gets a compilation error because the string contains non-escaped double quotes:

```cpp
const char* str = "Hello "World"!"; // Error!
```

Normally you have to escape the double quotes as follows:

```cpp
const char* str = "Hello \"World\"!";
```

With a raw string literal, you can avoid the need to escape the quotes. The raw string literal starts with `R"`.

```cpp
const char* str = R"(Hello "World"!)";
```

If you need a string consisting of multiple lines, without raw string literals you need to embed `\n` escape sequences in your string where you want to start a new line. For example:

```cpp
const char* str = "Line 1\nLine 2";
```

With a raw string literal:

```cpp
const char* str = R"(Line 1
Line 2)";
```

Escape sequences are ignored in raw string literals. For example, in the following raw string literal, the `\t` escape sequence is not replaced with a tab character, but is kept as the sequence of a back-slash followed by the letter t:

```cpp
const char* str = R"(Is the following a tab character? \t)";
```

If you output this string to the console:

```
Is the following a tab character? \t
```

Because a raw string literal ends with ")" you cannot embed a `)"` in your string using this syntax. The following string is not valid because it contains the `)"` sequence in the middle of the string:

```cpp
const char* str = R"(Embedded )" characters)"; // Error!
```

Raw string literals make it easier to work with database querying strings, regular expressions, file paths, and so on.

```cpp
string myString = "hello";
myString += ", there";
string myOtherString = myString;
if (myString == myOtherString) {
    myOtherString[0] = 'H';
}
cout << myString << endl;
cout << myOtherString << endl;
```

The output of this code is:

```
hello, there
Hello, there
```

There are several things to note in this example. One point is that there are no memory leaks even though strings are allocated and resized on a few places. All of these `string` objects are created as stack variables. While the `string` class certainly has a bunch of allocating and resizing to do, the `string` destructors cleans up this memory when `string` objects go out of scope.

For compatibility, you can use the `c_str()` method on a `string` to get a `const` character pointer, representing a C-style string. However, the returned `const` pointer becomes invalid whenever the `string` has to perform any memory reallocation, or when the `string` object is destroyed. You should call the method just before using the result so that it accurately reflects the current contents of the `string`, and you must never return the result of `c_str()` called on a stack-based `string` object from a function. 

There is also a `data()` method which, up until C++14, always returned a `const char*` just as `c_str()`. Starting with C++17, however, `data()` returns a `char*` when called on a non-`const` string. 

A string literal in source code is usually interpreted as a `const char*`. You can use the standard user-defined literal `s` to interpret a string literal as an std::string instead.

```cpp
auto string1 = "Hello World"; // string1 is a const char*
auto string2 = "Hello World"s; // string2 is an std::string
```

The standard user-defined literal `s` requirer a `using namespace std::string_literals` or `using namespace std;`.

The `std` namespace includes a number of helper functions that make it easy to convert numerical values into `string` or `string` into numerical values.

```cpp
string to_string(int val); // also supports: unsigned, long, ...
```

Converting in the other direction:

```cpp
int stoi(const string& std, size_t *idx=0, int base=10);
long stol(const string& str, size_t *idx=0, int base=10);
unsigned long stoul(const string& str, size_t *idx=0, int base=10);
long long stoll(const string& str, size_t *idx=0, int base=10);
unsigned long long stoull(const string& str, size_t *idx=0, int base=10);
float stof(const string& str, size_t *idx=0);
double stod(const string& str, size_t *idx=0);
long double stold(const string& str, size_t *idx=0);
```

The C++17 standard also provides a number of lower-level numerical conversion functions, all defined in the `<charconv>` header. These functions do not perform any memory allocations, but instead use buffers allocated by the caller. Additionaly, they are tuned for high performance and are locale-independent. The end result is that these functions can be orders of magnitude faster than other higher-level numerical conversion functions. You should use these functions if you want high performant, locale-independent conversions, for example to serialize/deserialize numerical data to/from human readable formats such as JSON, XML, and so on. 

For converting integers to characters, the following set of functions is available:

```cpp
to_chars_result to_chars(char* first, char* last, IntegerT value, int base = 10);
```

Here, `IntegerT` can be any signed or unsigned integer type or `char`. The result is of type `to_chars_result`, a type defined as follows:

```cpp
struct to_chars_result {
    char* ptr;
    errc ec;
};
```

Here is an example of its use:

```cpp
std::string out(10, ' ');
auto result = std::to_chars(out.data(), out.data() + out.size(), 12345);
if (result.ec == std::errc()) { /* Conversion successful. */ }
```

Using C++17 structured bindings, you can write as follows:

```cpp
std::string out(10, ' ');
auto [ptr, ec] = std::to_chars(out.data(), out.data() + out.size(), 12345);
if (ec == std::errc()) { /* Conversion successful. */ }
```

For the opposite conversion—that is, converting character sequences into numerical values—the following set of functions is available:

...

Before C++17, there was always a dilemma of choosing the parameter type for a function that accepted a read-only string. Should it be a `const char*`? In that case, if a client had an `std::string` available, they had to call `c_str()` or `data()` on it to get a `const char*`. Even worse, the function would lose the nice object-oriented aspects of the `std::string` and all its nice helper methods. Maybe the parameter could instead be a `const std::string&`? In that case, you always needed an std::string. If you passed a string literal, for example, the compiler siletly created a temporary `string` object that contained a copy of your string literal and passed that object to your function, so there was a bit of overhead. 

With C++17, all those problems are solved with the introduction of the `std::string_view` class, which is an intantiation of the `std::basic_string_view` class template, and is defined in the `<string_view>` header. A `string_view` is basically a drop-in replacement for `const string&`, but without the overhead. It never copies strings! A `string_view` supports an interface similar to `std::string`. One exception is the absence of `c_str()`, but `data()` is available. On the other hand, `string_view` does add the methods `remove_prefix(size_t)` and `remove_suffix(size_t)`, which shrink the string by advancing the starting point by a given offset, or by moving the end pointer backward by a given offset. 

Note that you cannot concatenate a `string` and a `string_view`. 

```cpp
string str = "Hello";
string_view sv = " world";
auto result = str + sv;
```

To make it compile:

```cpp
auto result = str + sv.data();
```

The `extractExtension()` function extracts and returns the extension of a given filename. Note that `string_view`s are usually passed by value because they are extremely cheap to copy. They just contain a pointer to, and the length of, a stirng.

```cpp
string_view extractExtension(string_view fileName)
{
return fileName.substr(fileName.rfind('.'));
}
```

```cpp
string fileName = R"(c:\temp\my file.ext)";
cout << "C++ string: " << extractExtension(fileName) << endl;
const char* cString = R"(c:\temp\my file.ext)";
cout << "C string: " << extractExtension(cString) << endl;
cout << "Literal: " << extractExtension(R"(c:\temp\my file.ext)") << endl;
```

There is not a single copy being made in all these calls to `extractExtension()`. The `fileName` parameter of the `extractExtension()` function is just a pointer and a length, and a length, and so is the return type of the function. This is all very efficient. 

You cannot implicitly construct a `string` from a `string_view`. Either you use an explicit `string` constructor, or you use the `string_view::data()` member. For example, suppose you have the following function that accepts a `const string&`:

```cpp
void handleExtension(const string& extension) { /* ... */ }
```

Calling this function as follows does not work:

```cpp
handleExtension(extractExtension("my file.ext"));
```

The following are two possible options you can use:

```cpp
handleExtension(extractExtension("my file.ext").data()); // data() method
handleExtension(string(extractExtension("my file.ext"))); // explicit ctor
```

Use an `std::string_view` instead of `const std::string&` or `const char*` whenever a function or method requires a read-only string as one of its parameters. 

You can use the standard user-defined `sv` to interpret a string literal as an `std::string_view`.

```cpp
auto sv = "My string_view"sv;
```

The standard user-defined literal `sv` requires a `using namespace std::string_view_literals` or `using namespace std;`.

