### A Crash Course in C++ and the Standard Library

- A brief overview of the most important parts and syntax of the C++ language and the Standard Library
- The basics of smart pointers

The `main()` function either takes no parameters, or takes two parameters as follows:

```cpp
int main(int argc, char* argv[])
```

`argc` gives the number of arguments passed to the program, and `argv` contains those arguments. Note that `argv[0]` can be the program name, but it might as well be an empty string, so do not rely on it; instead, use platform-specific functionality to retrieve the program name. The important thing to remember is that the actual parameters start at index 1.

_Namespaces_ address the problem of naming conflicts between different pieces of code. To place code in a namespace, enclose it within a namespace block. For example, the following could be the contents of a file called `namespaces.h`:


```cpp
namespace mycode {
    void foo();
}
```

The implementation of a method or function can also be handled in a namespace. The `foo()` function, for instance, could be implemented in `namespaces.cpp` as follows:

```cpp
#include <iostream>
#include "namespaces.h"

void mycode::foo()
{
    std::cout << "foo() called in the mycode namespace" << std::endl;
}
```

To call the namespace-enabled version of `foo()`, prepend the namespace onto the function name by using `::`, also called the _scope resolution operator_, as follows:

```cpp
mycode::foo();
```

C++17 makes it easier to work with _nested namespaces_. A nested namespace is a namespace inside another. 

```cpp
namespace MyLibraries::Networking::FTP {
    /* ... */
}
```

A _namespace alias_ can be used to give a new and possibly shorter name to another namespace. For example:

```cpp
namespace MyFTP = MyLibraries::Networking::FTP;
```

Variables can be converted to other types by _casting_ them. For example, a `float` can be cast to an `int`. 

```cpp
float myFloat = 3.14f;
int i1 = (int)myFloat;  // holdover from C, not recommended
int i2 = int(myFloat);  // rarely used
int i3 = static_cast<int>(myFloat); // cleanest one,, recommended
```

In some contexts, variables can be automatically cast, or _coerced_. For example, a `short` can be automatically converted into a `long` because a `long` represents the same type of data with at least the same percision. 

In C++, you can use the basic types (`int`, `bool`, and so on) to build more complex types of your own design. Classes are more powerful.

```cpp
enum PieceType { PieceTypeKing, PieceTypeQueen, PieceTypeRook, PieceTypePawn };
```

_Structs_ let you encapsulate one or more existing types into a new type. 

```cpp
struct Employee {
    char firstInitial;
    char lastInitial;
    int employeeNumber;
    int salary;
};
```

A variable with type `Employee` will have all of these _fields_ built in. The individual fields of a struct can be accessed by using the "." operator. 

The `switch` statement is an alternate syntax for performing actions based on the value of an expression. In C++, the expression of a `switch` statement must be of an integral type, a type convertible to an integer type, an enumerated type, or a strongly typed enumeration, and must be compared to constants. Each constant value represents a "case". If the expression matches the case, the subsequent lines of code are executed until a `break` statement is reached. You can also provide a `default` case, which is matched if none of the other cases match. 

```cpp
switch (menuItem) {
    case OpenMenuItem:
        // Code to open a file
        break;
    case SaveMenuItem:
        // Code to save a file
        break;
    default:
        // Code to give an error message
        break;
}
```

Once a `case` expression matching the `switch` condition is found, all statements that follow it are executed until a `break` statement is reached. This execution is called _fallthrough_. 

```cpp
switch (backgroundColor) {
    case Color::DarkBlue:
    case Color::Black:
        // Code to execute for both a dark blue or black background color
        break;
    case Color::Red:
        // Code to execute for a red background color
        break;
}
```

Starting with C++17, you can tell the compiler that a fallthrough is intentional using the `[[fallthrough]]` attribute as follows:

```cpp
switch (backgroundColor) {
    case Color::DarkBlue:
        doSomethingForDarkBlue();
        [[fallthrough]];
    case Color::Black:
        // Code is executed for both a dark blue or black background color
        doSomethingForBlackOrDarkBlue();
        break;
    case Color::Red:
    case Color::Green:
        // Code to execute for a red or green background color
        break;
}
```

Just as for `if` statements, C++17 adds support for intiliazers to `switch` statements.

```cpp
switch (<initializer> ; <expression>) { <body> }
```

C++ has one operator that takes three arguments, known as _ternary operator_. It is used as a short-hand conditional expression of the form "if [something] then [perform action], otherwise [perform some other action]." 

```cpp
std::cout << ((i > 2) ? "yes" : "no");
```

C++ uses _short-circuit logic_ when evaluating logical expressions. That means once the final result is certain, the rest of the operation won't be evaluated. 

In this way, the language saves your code from doing unnecesary work. It can, however, be a source of hard-to-find bugs if the later expressions in some way influence the state of the program (for example, by calling a separate function). 

Short-circuiting can be beneficial for performance. You can put cheaper tests first so that more expensive tests are not even executed when the logic short-circuits. It is also useful in the context of pointers to avoid parts of the expression to be executed when a pointer is not valid.

In C++, you first declare a function to make it available for other code to use. If the function is used inside only a particular file, you generally declare and define the function in the source file. If the function is for use by other modules or files, you generally put the declaration in a header file and the definition in a source file. 

Every function has a local predefined variable `__func__` containing the name of the current function. One use of this variable would be for logging purposes: 

```cpp
int addNumbers(int number1, int number2)
{
    std::cout << "Entering function " << __func__ << std::endl;
    return number1 + number2;
}
```

If you do specify the size of the array, and the initializer list has less elements than the given size, the remaining elements are set to 0. The following code only sets the first element in the array to the value 2, and sets all other elements to 0:

```cpp
int myArray[3] = {2};
```

To get the size of a stack-based C-style array, you can use the C++17 `std::size()` function (requires `<array>`).

```cpp
unsigned int arraySize = std::size(myArray);
```

The old trick to get the size of a stack-based C-style array is to use the `sizeof` operator. The `sizeof` operator returns the size of its arguments in bytes. To get the number of elements in a stack-based array, you divide the size in bytes of the array by the size in bytes of the first element. 

```cpp
unsigned int arraySize = sizeof(myArray) / sizeof(myArray[0]);
```

C++ allow multi-dimensional arrays. 

```cpp
char ticTacToeBoard[3][3];
ticTacToeBoard[1][1] = 'o';
```

In C++, it's best to avoid C-style arrays as discussed in this section, and instead use Standard Library functionality, such  as `std::array`, and `std::vector`, as discussed in the next two sections. 

C++ has a special type of fixed-size container called `std::array`, defined in the `<array>` header file. It's basically a thin wrapper around C-style arrays. 

There are a number of advantages to using `std::arrays` instead of C-style arrays. They always know their own size, are not automatically cast to a pointer to avoid certain types of bugs, and have iterators to easily loop over the elements. 

```cpp
array<int, 3> arr = {9, 8, 7};
cout << "Array size = " << arr.size() << endl;
cout << "2nd element = " << arr[1] << endl;
```

If you want an array with a dynamic size, it is recommended to use `std::vector`, as explained in the next section. A `vector` automatically increases in size when you add new elements to it. 

The C++ Standard Library provides a number of different non-fixed-size containers that can be used to store information. `std::vector`, declared in `<vector>`, is an example of such a container. The `vector` replaces the concept of C-style arrays with a much more flexible and safer mechanism. 
As a user, you need not worry about memory management, as the `vector` automatically allocates enough memory to hold its elements. A `vector` is dynamic, meaning that element can be added and removed at run time. 

```cpp
// Create a vector of integers
vector<int> myVector = { 11, 22 };
// Add some more integers to the vector using push_back()
myVector.push_back(33);
myVector.push_back(44);
// Access elements
cout << "1st element: " << myVector[0] << endl;
```

C++17 introduces the concept of _structured bindings_. Structured bindings allow you to declare multiple variables that are initialized with elements from an array, struct, pair or tuple. 

You can declare three variables, x, y, and z, initialized with the three values from the array as follows. Note that you have to use the `auto` keyword for structured bindings. You cannot, for example, specify `int` instead of `auto`. 

```cpp
std::array<int, 3> values = { 11, 22, 33 };
auto [x, y, z] = values;
```

The number of variables declared with the structured binding has to match the number of values in the expression on the right. 

Structured bindings also work with structures if all non-static members are public.

```cpp
struct Point { double mX, mY, mZ; };
Point point;
point.mX = 1.0; point.mY = 2.0; point.mZ = 3.0;
auto [x, y, z] = point;
```

The while loop. The keyword `break` can be used within a loop to immediately get out of the loop and continue execution of the program. The keyword `continue` can be used to return to the top of the loop and reevaluate the `while` expression. However, using `continue` in loops is often considered poor style because it causes the execution of a program to jump arround somewhat haphazardly, so use it sparingly. 

The following example first defines an array of four integers. The range-based `for` loop then iterates over a _copy_ of every element in this array and prints each value. To iterate over the elements themselves _without making copies_, use a reference variable, discussed later.

```cpp
std::array<int, 4> arr = {1, 2, 3, 4};
for (int i : arr) {
std::cout << i << std::endl;
}
```

Initializer lists are defined in the `<intializer_list>` header file and make it easy to write functions that can accept a variable number of arguments. The `initializer_list` class is a template and so it requires you to specify the type of elements in the list between angle brackets. The following example shows how to use an initializer list:

```cpp
#include <initializer_list>
using namespace std;
int makeSum(initializer_list<int> lst)
{
    int total = 0;
    
    for (int value : lst) {
        total += value;
    }

    return total;
}
```

The function `makeSum()` accepts an initializer list of integers as arguments. The body of the function uses a range-based `for` loop to accumulate the total sum. This function can be used as follows:

```cpp
int a = makeSum({1,2,3});
int b = makeSum({10,20,30,40,50,60});
```

Initializer lists are type safe and define which type is allowed to be in the list. For the `makeSum()` function shown here, all elements of the initializer must be integers. Trying to call it with a `double` will result in a compiler error. 

The C++ `string` type is defined in the `<string>` header file. 

Dynamic memory allows you to build programs with data that is not fixed size at compile time. 

Stack frames are nice because they provide an isolated memory workspace for each function. If a variable is declared inside the `foo()` stack frame, calling the `bar()` function won't change it unless you specifically tell it to. Variables that are stack-allocated do not need to be deallocated (deleted) by the programmer; it happens automatically. 

The _heap_ is an area of memory that is completely independent of the current function or stack frame. You can put variables on the heap if you want them to exist even when the function in which they were created has completed. The heap is less structured than the stack. You have to make sure that you deallocate (delete) any memory that you allocated on the heap. This does not happen automatically, unless you use smart pointers, which are discussed in the section "Smart Pointers".

You can put anything on the heap by explicitly allocating memory for it. 

```cpp
int* myIntegerPointer;
```

You should always declare and initialize your pointers at the same time. You can initialize them to a null pointer (`nullptr`) if you don't want to allocate memory right away.

```cpp
int* myIntegerPointer = nullptr;
```

A null pointer is a special default value that no valid pointer will ever have, and converts to `false` when used in a Boolean expression. 

```cpp
if (!myIntegerPointer) { /* myIntegerPointer is a null pointer */ }
```

You use the `new` operator to allocate the memory:

```cpp
myIntegerPointer = new int;
```

After you are finished with your dynamically allocated memory, you need to deallocate the memory using the `delete` operator. To prevent the pointer from being used after having deallocated the memory it points to, it's recommended to set your pointer to `nullptr`:

```cpp
delete myIntegerPointer;
myIntegerPointer = nullptr;
```

Pointers don't always point to heap memory. You can declare a pointer that points to a variable on the stack, even another pointer. 

C++ has a special syntax for dealing with pointers to structures. Technically, if you have a pointer to a structure, you can access its fields by first dereferencing it with*, then using the normal syntax. 

```cpp
Employee* anEmployee = getEmployee();
cout << (*anEmployee).salary << endl;
```

The -> operator lets you perform both the dereference and the field access in one step. 

```cpp
Employee* anEmployee = getEmployee();
cout << anEmployee->salary << endl;
```

The concept of short-circuiting logic. This can be useful in combination with pointers to avoid using an invalid pointer, as in the following example:

```cpp
bool isValidSalary = (anEmployee && anEmployee->salary > 0);
```

or, a bit more verbose:

```cpp
bool isValidSalary = (anEmployee != nullptr && anEmployee->salary > 0);
```

The heap can also be used to dynamically allocate arrays. You use the `new[]` operator to allocate memory for an array. 

```cpp
int arraySize = 8;
int* myVariableSizedArray = new int[arraySize];
```

When your code is done with the array, it should remove the array from the heap so that other variables can use the memory. In C++, you use the `delete[]` operator to do this.

```cpp
delete[] myVariableSizedArray;
myVariableSizedArray = nullptr;
```

Avoid using `malloc()` and `free()` from C. Instead, use `new` and `delete`, or `new[]` and `delete[]`.

To prevent memory leaks, every call to `new` should be paired with a call to `delete`.

To avoid common memory problems, you should use _smart pointers_ instead of "raw", also called "naked", C-style pointers. Smart pointers automatically deallocate memory when the smart pointer object goes out of scope, for example, when the function has finished executing. 

The two most important smart pointer types in C++, both defined in the `<memory>` and in the `std` namespace:

```cpp
std::unique_ptr
std::shared_ptr
```

`unique_ptr` is analogous to an ordinary pointer, except that it automatically frees the memory or resource when the `unique_ptr` goes of scope or is deleted. As such, `unique_ptr` has sole ownership of the object pointed to. 

To create a `unique_ptr`:

```cpp
auto anEmployee = make_unique<Employee>();
```

or:

```cpp
unique_ptr<Employee> anEmployee(new Employee);
```

An `unique_ptr` can also be used to store a C-style array.

```cpp
auto employees = make_unique<Employee[]>(10);
cout << "Salary: " << employees[0].salary << endl;
```

`shared_ptr` allows for distributed ownership of the data. Each time a `shared_ptr` is assigned, a reference count is incremented indicating there is one more owner of the data. When the reference counter goes to zero, it means there is no longer any owner of the data, and the object referenced by the pointer is freed.

To create a `shared_ptr`:

```cpp
auto anEmployee = make_shared<Employee>();
if (anEmployee) {
    cout << "Salary: " << anEmployee->salary << endl;
}
```

Starting with C++17, you can also store an array in a `shared_ptr`.

```cpp
shared_ptr<Employee[]> employees(new Employee[10]);
cout << "Salary: " << employees[0].salary << endl;
```

The keyword `const` can be used in several different ways in C++. 

**const Constants**

In C++, programmers are encouraged to avoid `#define` in favor of using `const` to define constants. Defining a constant with `const` is just like defining a variable, except that the compiler guarantees that code cannot change the value. 

```cpp
const int versionNumberMajor = 2;
const int versionNumberMinor = 1;
const std::string productName = "Super Hyper Net Modulator";
```

**const to Protect Parameters**

In C++, you can cast a non-`const` variable to a `const` variable. It offers some degree of protection from other code changing the variable. 

```cpp
void mysteryFunction(const std::string* someString)
{
    *someString = "Test"; // Will not compile.
}
int main()
{
    std::string myString = "The string";
    mysteryFunction(&myString);
    return 0;
}
```

A reference in C++ allows you to give another name to an existing variable. 

```cpp
int x = 42;
int& xReference = x;
```

Attaching `&` to a type indicates that the variable is a reference. It is still used as though it was a normal variable, behind the scenes, it is really a pointer to the original variable. Both the variable `x` and the reference variable `xReference` points to exactly the same value. If you change the value through either one of them, the change is visible through the other as well. 

pass by reference p81

### Working with Strings and String Views

### Coding with Style