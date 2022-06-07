## Chapter 11: C++ Quirks, Oddities, and Incidentals

### References

A _reference_ in C++ is an _alias_ for another variable. All modifications to the reference change the value of the variable to which it refers. You can think of references as implicit pointers that save you the trouble of taking the address of variables and dereferencing the pointer. Alternatively, you can think of references as just another name for the original variable. You can create stand-alone reference variables, use reference data members in classes, accept references as parameters to functions and methods, and return references from functions and methods. 

Reference variables must be initialized as soon as they are created, like this:

```cpp
int x = 3;
int & xRef = x;
```

Subsequent to this assignment, `xRef` is another name for `x`. Any use of xRef uses the current value of `x`. Any assignment to `xRef` changes the value of x. For example, the following code sets `x` to 10 through `xRef`:

```cpp
xRef = 10;
```

You cannot declare a reference variable outside of a class without initializing it:

```cpp
int& xRef // does not compile!
```

You must always initialize a reference when it is created. Usually references are created when they are declared, but reference data members need to be initialized in the constructor initializer for the containing class. 

You cannot create a reference to an unnamed value, such as an integer literal, unless the reference is to a `const` value. 

```cpp
int& unnamedRed1 = 5;   // Does not compile
const int& unnamedRef2 = 5; // works as expected
```

The same holds for temporary objects. You cannot have a non-`const` reference to a temporary object, but a `const` reference is fine. 

```cpp
std::string getString() { return "Hello world"; }
```

```cpp
std::string& string1 = getString();     // does not compile
const std::string& string2 = getString();   // works as expected
```

**Modifying References**

A reference always refers to the same variable to which it is initialized; references cannot be changed once they are created. If you "assign" a variable to a reference when the reference is declared, the reference refers to that variable. However, if you assign a variable to a reference after that, the variable to which the reference refers is changed to the value of the variable being assigned. The reference is not updated to refer to that variable. 

```cpp
int x = 3, y = 4
int& xRef = x;
xRef = y;   // Changes value of x to 4. Doesn't make xRef refer to y
```

You might try to circumvent this restriction by taking the address of `y` when you assign it:

```cpp
xRef = &y;  //does not compile
```

You cannot change the variable to which a reference refers after it is initialized; you can change only the value of that variable.

**References to Pointers and Pointers to References**

You can create references to any type, including pointer types. Here is an example of a reference to a pointer to `int`.

```cpp
int* intP;
int*& ptrRef = intP;
ptrRef = new int;
*ptrRef = 5
```

### Reference Data Members

As Chapter 9 explains, data members of classes can be references. A reference cannot exist without referring to some other variable. Thus, you must initialize reference data members in the constructor initializer, not in the body of the constructor. 

```cpp
class myClass
{
    public:
        MyClass(int& ref) : mRef(ref) {}
    private:
        int& mRef;
};
```

C++ programmers do not often use stand-alone reference variables of reference data members. The most common use of references is for parameters to functions and methods. Recall that the default parameter-passing semantics are pass-by-value: functions receive copies of their arguments. When those parameters are modified, the original arguments remain unchanged. References allow you to specify pass-by-reference semantics for arguments passed to the function. When you use reference parameters, the function receives references to the function arguments. If those references are modified, the changes are reflected in the original argument variables. 

```cpp
void swap(int& first, int& second)
{
    int temp = first;
    first = second;
    second = temp;
}
```

**References from Pointers**

A common quandary arises when you have a pointer to something that you need to pass to a function or method that takes a reference. You can "convert" a pointer to a reference in this case by dereferencing the pointer.

```cpp
int x = 5, y = 6
int *xp = &x, *yp = &y;
swap(*xp, *yp);
```

**Pass-by-Reference versus Pass-by-Value**

Pass-by-reference is required when you want to modify the parameter and see those changes reflected in the variable passed to the function or method. However, you should not limit your use of pass-by-reference to only those cases. Pass-by-reference avoids copying the arguments to the function, providing two additional benefits in some cases:

1. Efficiency. Large objects and `structs` could take a long time to copy. 
2. Correctness. Not all objects allow pass-by-value. Even those that do allow it might not support deep copying correctly. Objects with dynamically allocated memory must provide a custom copy constructor and copy assignment operator in order to support deep copying. 

If you want to leverage these benefits, but do not want to allow the original objects to be modified, you should mark the parameters `const`, giving you pass-by-const-reference. 

These benefits to pass-by-reference imply that you should use pass-by-value only for simple built in types like int and double for which you don't need to modify the arguments. Use pass-by-`const`-reference or pass-by-reference in all other cases. 

**Reference Return Values**

You can also return a reference from a function or method. The main reason to do so is efficiency. Instead of returning a whole object, return a reference to the object to avoid copying it unnecessarily. Of course, you can only use this technique if the object in question continues to exist following the function termination.

From a function or method, never return a reference to a variable that is locally scoped to that function or method, such as an automatically allocated variable on the stack that will be destroyed when the function ends.

Note that if the type you want to return from your function supports move semantics, then returning it by value is almost as efficient as returning a reference. 

A second reason to return a reference is if you want to be able to assign to the return value directly as an _lvalue_. Several overloaded operators commonly return references. 

**Rvalue Reference**

An _rvalue_ is anything that is not an lvalue, such as a constant value, or a temporary object or value. Typically, an rvalue is on the right-hand side of an assignment operator. 

```cpp
void handleMessage(std::string& message)
{
    ...
}
```

With only this version of `handleMessage()`, you cannot call it as follows:

```cpp
handleMessage("Hello World");   // a literal is not an lvalue

std::string a = "Hello";
std::string b = "World";
handleMessage(a+b); // a temporary is not an lvalue
```

To allow these kinds of calls, you need a version that accepts an rvalue reference:

```cpp
void handleMessage(std::string&& message)
{
    ...
}
```

**Deciding between References and Pointers**

Most of the time, you can use references instead of pointers. References to objects even support polymorphism in the same way as pointers to objects. However, there are some use-cases in which you need to use a pointer. One example is when you need to change the location to which it points. Recall that you cannot change the variable to which references refer. For example, when you dynamically allocate memory, you need to store a pointer to the result in a pointer rather than a reference. A second use-case in which you need to use a pointer is when the pointer is optional, that is, when it can be `nullptr`. Yet another use-case is if you want to store polymorphic types in a container. 

A way to distinguish between approriate use of pointers and references in parameters and return types is to consider who _owns_ the memory. If the code receiving the variable becomes the owner and thus becomes responsible for releasing the memory associated with an object, it must receive a pointer to the object. Better yet, it should receive a smart pointer, which is the recommended way to transfer ownership. If the code receiving the variable should not free memory, it should receive a reference. 

Consider a function that splits an array of `int`s into two arrays: one of even numbers and one of odd numbers. The function doesn't know how many numbers in the source array will be even or odd, so it should dynamically allocate the memory for the destination arrays after examining the source array. It should also return the sizes of the two arrays. Obviously you must use pass-by-reference. 

```cpp
void seperateOddsAndEvens(const int arr[], size_t size, int*& odds, size_t& numOdds, int*& evens, size_t& numEvens)
{
    numOdds = numEvems = 0;
    for (size_t i = 0; i < size; ++i) {
        if (arr[i] % 2 == 1) {
            ++numOdds;
        } else {
            ++numEvens;
        }
    }

    odds = new int[numOdds];
    evens = new int[numEvens];

    size_t oddsPos = 0, evensPos = 0;
    for (size_t i = 0; i < size; i++) {
        if (arr[i] % 2 == 1) {
            odds[oddsPos++] = arr[i];
        } else {
            evens[evensPos++] = arr[i];
        }
    }
}
```

In this case, the `odds` and `evens` parameters are references to `int*`s. `seperateOddsAndEvens()` can modify the `int*`s that are used as arguments to the function (through the reference).

```cpp
void seperateOddsAndEvens(const vector<int>& arr, vector<int>& odds, vector<int>& evens)
{
    for (int i : arr) {
        if (i % 2 == 1) {
            odds.push_back(i);
        } else {
            evens.push_back(i);
        }
    }
}
```

The version using `vector`s is already much better than the versions using pointers or references, but it's usually recommended to avoid output parameters as much as possible. If a function needs to return something, you should just return it, instead of using output parameters. Especially since C++11 introduced move semantics, returning something by value from a function is efficient. And now that C++17 has introduced structured bindings, it is really convenient to return multiple values from a function.

So, for the `seperateOddsAndEvens()` function, instead of accepting two output `vector`s, it should simply return a `pair` of `vector`s. The `std::pair` utility class, defined in `<utility>`, is discussed in Chapter 17, but its use is rather straightforward. Basically, a `pair` can store two value of two different or equal types. It's a class template, and it requires two types between the angle brackets to specify the type of both values. 

```cpp
pair<vector<int>, vector<int>> seperateOddsAndEvens(const vector<int>& arr)
{
    vector<int> odds, evens;
    for (int i : arr) {
        if (i % 2 == 1) {
            odds.push_back(i);
        } else {
            evens.push_back(i);
        }
    }

    return make_pair(odds, evens);
}
```

By using structured binding, the code call `separateOddsAndEvens()` becomes very compact, yet very easy to read and understand:

```cpp
vector<int> vecUnSplit = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
auto[odds, evens] = separateOddsAndEvens(vecUnSplit);
```

### Keyword Confusion

Two keywords in C++ appear to cause more confusion than any others: `const` and `static`. Both of these keywords have several different meanings, and each of their uses presents subtleties that are important to understand. 

**The const keyword**

const variables and parameters

You can also use `const` to specify that parameters to functions or methods should remain unchanged. 

const pointers

When a variable contains one or more levels of indirection via pointer, applying a `const` becomes trickier. 

```cpp
int* ip;
ip = new int[10];
ip[4] = 5;
```

Suppose that you decide to apply `const` to `ip`. Do you want to prevent the `ip` variable itself from being changed, or do you want to prevent the values to which it points from being changed? That is, do you want to prevent the second or third line?

In order to prevent the pointed-to values from being modified:

```cpp
const int* ip;
ip = new int[10];
ip[4] = 5; // does not compile
```

Now you cannot change the values to which `ip` points.

An alternative but semantically equivalent way to write this is as follows:

```cpp
int cosnt* ip;
```

Putting the `const` before or after the `int` makes no difference in its functionality.

If you instead want to mark `ip` itself `const` (not the values to which it points), you need to write:

```cpp
int* const ip = nullptr;
ip = new int[10];   // does not compile
ip[4] = 5;  // error: dereferencing a null pointer
```

Now that `ip` itself cannot be changed, the compiler requires you to initialize it when you declare it, either with `nullptr` as in the preceding code or with newly allocated memory as follows:

```cpp
int* const ip = new int[10];
ip[4] = 5;
```

You can also mark both the pointer and the values it points `const` like this:

```cpp
int const* const ip = nullptr;
```

or:

```cpp
const int* const ip = nulltpr;
```

Although this syntax might seem confusing, there is actually a very simple rule: the `const` keyword applies to whatever is directly to its left. 

```cpp
int const* const ip = nullptr;
```

From left to right, the first `const` is directly to the right of the word `int`. Thus, it applies to the `int` to which `ip` points. Therefore, it specifies that you cannot change the values to which `ip` points. The second `const` is directly to the right of the `*`. Thus, it applies to the pointer to the `int`, which is the `ip` variable. Therefore, it specifies that you cannot change `ip` (the pointer) itself.

const References

When C++ programmers refer to a `const` reference, they mean something like this:

```cpp
int z;
const int& zRef = z;
zRef = 4;   //does not compiler
```

By applying `const` to the `int&`, you prevent assignment to `zRef`, as shown. 

`const` references are used most commonly as parameters, where they are quite useful. If you want to pass something by reference for efficiency, but don't want it to be modifiable, make it a `const` reference.

```cpp
void doSomething(const BigClass& arg)
{

}
```

The constexpr Keyword

C++ always had the notion of constant expressions, and in some circumstances constant expressions are required. For example, when defining an array, the size of the array needs to be a constant expression. Because of this restriction, the following piece of code is not valid in C++.

```cpp
const int getArraySize() { return 32; }

int main()
{
    int myArray[getArraySize()];    // Invalid in C++
    return 0;
}
```

Using the `constexpr` keyword, the `getArraySize()` function can be redefined to make it a _constant expression_. Constant expressions are evaluated at compile time!

```cpp
constexpr int getArraySize() { return 32; }

int main()
{
    int myArray[getArraySize()];    // ok
}
```



Declaring a function as `constexpr` imposes quite a lot of restrictions on what the function can do because the compiler has to be able to evaluate the function at compile time, and the function is not allowed to have any side effects. Here are a couple of restrictions, although this is not an exhaustive list:

- The function body shall not contain any `goto` statements, try catch blocks, uninitialized variables, or variable definitions that are not literal types, and shall not throw any exceptions. It is allowed to call other `constexpr` functions. 
- The return type of the funtion shall be a literal type.
- If the `constexpr` function is a member of a class, the function cannot be `virtual`.
- All the function parameters shall be literal types. 
- A `constexpr` function cannot be called until it's defined in the translation unit because the compiler needs to know the complete definition.
- `dynamic_cast()` and `reinterpret_cast()` are not allowed.
- `new` and `delete` expressions are not allowed.

By defining a `constexpr` constructor, you can create constant expression variables of user-defined types. A `constexpr` constructor also has a lot of restrictions. Here are some of them:

- The class cannot have any virtual base classes.
- All the constructor parameters shall be literal types.
- The constructor body cannot be a function-try-block
- The constructor body either shall be explicitly defaulted, or shall satisfy the same requirements as the body of a `constexpr` function.
- All data members shall be initialized with constant expressions.

Example:

```cpp
class Rect
{
    public:
        constexpr Rect(size_t width, size_t height)
            : mWidth(width), mHeight(height) {}
        
        constexpr size_t getArea() const { return mWidth * mHeight; }
    
    private:
        size_t mWidth, mHeight;
};
```

Using this class to declare a `constexpr` object is straightforward:

```cpp
constexpr Rect r(8, 2);
int myArray[r.getArea()];   // ok
```

### The static Keyword

**static Data Members and Methods**

You can declare `static` data members and methods of classes, `static` data members, unlike non-`static` data members, are not part of each object. Instead, there is only one copy of the data member, which exists outside any objects of that class. 

`static` methods are similarly at the class level instead of the object level. A `static` method does not execute in the context of a specific object. 

**static Linkage**

Before covering the `static` keyword for linkage, you need to understand the concept of _linkage_ in C++. C++ source files are each compiled independently, and the resulting object files are linked together. Each name in a C++ source file, including functions and global variables, has a linkage that is either external or internal. External linkage means that the name is available from other source files. Internal linkage (also called static linkage) means that it is not. By default, functions and global variables have external linkage. However, you can specify internal (or static) linkage by prefixing the declaration with the keyword `static`. For example, suppose you have two source files: `FirstFile.cpp` and `AnotherFile.cpp`. Here is `FirstFile.cpp`:

```cpp
void f();

int main()
{
    f();
    return 0;
}
```

Note that this file provides a prototype for `f()`, but doesn't show the definition. Here is `AnotherFile.cpp`.

```cpp
#include <iostream>

void f();

void f()
{
    ...
}
```

This file provides both a prototype and a definition for `f()`. Note that it is legal to write prototypes for the same function in two different files. That's precisely what the precisely what the preprocessor does for you if you put the prototype in a header file that you `#include` in each of the source files. 

However, suppose you apply `static` to the `f()` prototype in `AnotherFile.cpp`. Note that you don't need to repeat the `static` keyword in front of the definition of `f()`. As long as it precedes the first instance of the function name, there is no need to repeat it.

```cpp
#include <iostream>

static void f();

void f()
{
    ...
}
```

Now each of the source files compiles without error, but the linker step fails because `f()` has internal (`static`) linkage, making it unavailable from `FirstFile.cpp`. 

An alternative to using `static` for internal linkage is to employ _anonymous namespaces_. Instead of marking a variable or function `static`, wrap it in an unnamed namespace:

```cpp
#include <iostream>

namespace {
    void f();

    void f()
    {
        ...
    }
}
```

Entities in an anonymoys namespace can be accessed anywhere following their declaration in the same source file, but cannot be accessed from other source files. 

**The extern keyword**

A related keyword, `extern`, seems like it should be the opposite of `static`, specifying external linkage for the names it precedes. It can be used that way in certain cases. For example, `const`s and `typedef`s have internal linkage by default. You can use `extern` to give them external linkage. However, `extern` has some complications. When you specify a name as `extern`, the compiler treats it as a declaration, not a definition. For variables, this means the compiler doesn't allocate space for the variable. You must provide a separate definition line for the variable without the `extern` keyword. For example, here is the content of `AnotherFile.cpp`:

```cpp
extern int x;
int x = 3;
```

Alternatively, you can initialize x in the `extern` line, which then serves as the declaration and definition:

```cpp
extern int x = 3;
```

The `extern` in this case is not very useful, because `x` has external linkage by default anyway. The real use of `extern` is when you want to use `x` from another source file, `FirstFile.cpp`:

```cpp
#include <iostream>

extern int x;

int main()
{
    ...
}
```

It is not recommended to use global variables at all. They are confusing and error-prone.

The final use of the `static` keyword in C++ is to create local variables that retain their values between exits and entrances to their scope. A `static` variable inside a function is like a global variable that is only accessible from that function. One common use of `static` variables is to "remember" whether a particular initialization has been performed for a certain function. 

```cpp
void performTask()
{
    static bool initialized = false;
    if (!initialized) {
        ...
    }
    ...
}
```

Usually better ways to structure your code. In this case, you might want to write a class in which the constructor performs the required initialization.

### Types and Casts

This section explores some of the trickier aspects of types: type aliases, type aliases for function pointers, type aliases for pointers to methods and data members, `typedefs`, and casts.

Type Aliases

A type alias provides a new name for an existing type declaration. You can think of a type alias as syntax for introducing a synonym for an existing type declaration without creating a new type. 

The most common use for type aliases is to provide manageable names when the real type declarations become too unwieldy. This situation commonly arises with templates. To declare a `vector` of `string`s, you need to declare it as `std::vector<std::string>`. It's a templated class, and thus requires you to specify the template parameters any time you want to refer to the type of this `vector`. With a type alias, you can create a shorter, more meaningful name:

```cpp
using StringVector = std::vector<std::string>;

void processVector(const StringVector& vec) {...}

int main()
{
    StringVector myVector;
    processVector(myVector);
    return 0;
}
```

**Type Aliases for Function Pointers**

In C++, you can use _functions as data_. In other words, you can take the address of a function and use it like you use a variable. 

Function pointers are types according to the parameters types and return type of compatible functions. One way to work with function pointers is to use a type alias. A type alias allows you to assign a type name to the family of functions that have the given characteristics. 

```cpp
using MatchFunction = bool(*) (int, int);
```

Now, you can write a function that takes a MatchFunction as a parameter. For example, the following function accepts two `int` arrays and their size, as well as a `MatchFunction`. It iterates through the arrays in parallel and calls the `MatchFunction` on corresponding elements in both arrays, printing a message if the call returns `true`. Notice that even though the `MatchFunction` is passed in as a variable, it can be called just like a regular function:

```cpp
void findMatches(int values1[], int values2[], size_t numValues, MatchFunction matcher)
{
    for (size_t i = 0; i < numValues; i++) {
        if (matcher(values1[i], values2[i])) {
            ....
        }
    }
}
```

To call the `findMatches()` function, all you need is any function that adheres to the defined `MatchFunction` type—that is, any type that takes in two `int`s and returns a `bool`. 

While function pointers in C++ are uncommon, you may need to obtain function pointers in certain cases. Perhaps the most common example of this is when obtaining a pointer to a function in a dynamic link library. 

You can create and use pointers to both variables and functions. Now, consider pointers to class data members and methods. 

```cpp
Employee employee;
int (Employee::*methodPtr) () const = &Employee::getSalary;
cout << (employee.*methodPtr)() << endl;
```

The second line declares a variable called `methodPtr` of type pointer to a non-`static` `const` method of `Employee` that takes no arguments and returns an `int`. At the same time, it initialized this variable to point to the `getSalary()` method of the `Employee` class. This syntax is quite similar to declaring a simple function pointer, except for the addition of `Employee::` before the `*methodPtr`. Note also that the `&` is required in this case.

The third line calls the `getSalary()` method (via the `methodPtr` pointer) on the `employee` object. Note the use of parentheses surrounding `employee.*methodPtr`. They are needed because `()` has higher precedence than `*`.

The second line can be made easier to read with a type alias:

```cpp
Employee employee;
using PtrToGet = int (Employee::*) () const;
PtrToGet methodPtr = &Employee::getSalary;
cout << (employee.*methodPtr) () << endl;
```

Using `auto`, it can be simplified even further:

```cpp
Employee employee;
auto methodPtr = &Employee::getSalary;
cout << (employee.*methodPtr)() << endl;
```

You can get rid of the (.*) syntax by using `std::mem_fn()`. 

Pointers to methods and data members usually won't come up in your programs. However, it's important to keep in mind that you can't dereference a pointer to  a non-`static` method or data member without an object. Every so often, you may want to try something like passing a pointer to a non-`static` method to a function, which simply won't work.

**typedefs**

Type aliases were introduced in C++11. Before C++11, you had to use `typedef`s to accomplish something similar but in a more convoluted way.

Just as a type alias, a `typedef` provides a new name for an existing type declaration. 

Without type aliases, you had to use `typedef` which looked as follows:

```cpp
typedef int* IntPtr;
```

Much less readable.

Always prefer type aliases over typedefs.

### Casts

`const_cast()`

You can use it to add `const`-ness to a variable, or cast away `const`-ness of a variable. It is the only cast of the four that is allowed to cast away `const`-ness. 

Starting with C++17, there is a helper method called `std::as_const()`, defined in `<utility>`, that returns a `const` reference version of its reference parameter. Basically, `as_const(obj)` is equivalent to `const_cast<const T&>(obj)`. 

Watch out when using `as_const()` in combination with `auto`. Remember that `auto` strips away reference and `const` qualifiers!

`static_cast()`

You can use `static_cast()` to perform explicit conversions that are supported directly by the language. For example, if you write an arithmetic expression in which you need to convert an `int` to a `double` in order to avoid integer division, use a `static_cast()`. 

You can also use `static_cast()` to perform explicit conversion that are allowed because of user-defined constructors or conversion routines. For example, if class `A` has a constructor that takes an object of class B, you can convert a `B` object to an `A` object with `static_cast()`. 

These casts work with both pointers and references. They do not work with objects themselves. 

To perform the cast safely with run-time type checking, use `dynamic_cast()`.

**reinterpret_cast()**

`reinterpret_cast()` is a bit more powerful and less safe. For example, you can cast a reference from one type to a reference of another type, even if the references are unrelated. This is commonly used to cast a pointer to a `void*`. 

**dynamic_cast()**

`dynamic_cast()` provides run-time check on casts within an inheritance hierarchy. You can use it to cast pointers or references. If the cast doesn't make sense, `dynamic_cast()` returns a null pointer, or throws an `std::bad_cast` exception (for the reference version).

The run-time type information is stored in the vtable of an object. Therefore, in order to use `dynamic_cast()`, your classes must have at least one `virtual` method. If your classes don't have a vtable, trying to use `dynamic_cast()` will result in a compilation error. 

### Scope Resolution

### Attributes

Attributes are a mechanism to add optional and/or vendor-specific information into source code. 

The C++ standard defines only six standard attributes.

**[[noreturn]]**

`[[noreturn]]` means that a function never returns control to the call site. Typically, the function either causes some kind of termination (process termination or thread termination), or throws an exception. With this attribute, the compiler can avoid giving certain warnings or errors because it now knows more about the intent of the function. 

**[[deprecated]]**

can be used to mark something as deprecated, which means you can still use it, but its use is discouraged. 

```cpp
[[deprecated("Unsafe method, please use xyz")]] void funct();
```

If you use this deprecated function, you'll get a compilation error or warning. 

**[[fallthrough]]**

Starting with C++17, you can tell the compiler that a fallthrough in a `switch` statement is intentional using the `[[fallthrough]]` attribute. 

**[[nodiscard]]**

attribute can be used on a function returning a value to let the compiler issue a warning when that function is used without doing something with the returned value. 

### User-defined Literals

C++ also allows you to define your own literals. User-defined literals should start with an underscore. The first character following the underscore must be a lowercase letter. 

C++ defines the following standard user-defined literals. Note that these standard user-defined literals do not start with an underscore.

- s for creating `std::string` (requires `using namespace std::string_literal`)

```cpp
auto myString = "Hello world"s;
```

- sv for creating `std::string_view` (requires `using namespace std::string_view_literals`)

```cpp
auto myStringView = "Hello world"sv;
```

- h, min, s, ms, us, ns, for creating `std::chrono::duration` time intervals.
(requires `using namespace std::chrono_literals`)

```cpp
auto myDuration = 42min;
```

A `using namespace std` also makes these standard user-defined literals available.

### Header files

The problem of duplicate definitions can be avoided with a mechanism known as `include guards`. The following code snippet shows the `Logger.h` with include guards. At the beginning of each header file, the #`ifndef` directive checks to see if a certain key has not been defined. If the key has been defined, the compiler skips to the mathcing `#endif`, which is usually placed at the end of the file. 

Nearly all compilers these days support the `#pragma once` directive which replaces include guards.

Another tool for avoiding problems with header files is _forward declarations_. If you need to refer to a class but you cannot include its header file, you can tell the compiler that such a class exists without providing a formal definition through the `#include` mechanism. 

For example, assume that the `Logger` class uses another class called `Preferences`, that keeps track of user settings. 

It's recommended to use forward declarations as much as possible in your header files instead of including other headers. This can reduce your compilation and recompilation times, because it breaks dependencies of your header file on other headers. Of course, your implementation file needs to include the correct headers for types that you've forward-declared; otherwise, it won't compile.

To query whether a certain header file exists, C++17 adds the `__has_include("filename")` and `__has_include(<filename>)` preprocessor constants. 

```cpp
#if __has_include(<optional>)
    #include <optional>
#elif __has_include(<experimental/optional>)
    #include <experimental/optional>
#endif
```

### Variable-Length Argument Lists

For example, suppose you want to write a quick-and-dirty debug function that prints strings to `stderr` if a debug flag is set, but does nothing if the debug flag is not set. Just like `printf()`, this function should be able to print strings with an arbitrary number of arguments and arbitrary types of arguments. 

```cpp
#include <cstdio>
#include <cstdarg>

bool debug = false;

void debugOut(const char* std,...)
{
    va_list ap;
    if (debug) {
        va_start(ap, str);
        vfprintf(stderr, str, ap);
        va_end(ap);
    }
}
```

First, note that the prototype of `debugOut()` contains one typed and named parameter `str`, followed by ... They stand for any number and type of arguments. 

Avoid using C-style variable-length argument lists. It is preferable to pass in an `std::array` or `vector` of values.

### Macros

You should avoid macros entirely in favor of inline functions. 

Most compilers can output the preprocessed source to a file or to standard output. You can use that to see how the preprocessor is preprocessing your file. For example, with gcc you can use the -E switch.

