## Chapter 14: Handling Errors

The C++ language provides a feature called _exceptions_ to handle these _exceptional_ but not _unexpected_ situations.

### Errors and Exceptions

Exceptions are a mechanism for a piece of code to notify another piece of code of an "exceptional" situation or error condition without progressing through the normal code paths. Exceptions do not follow the fundamental rule of step-by-step execution to which you are accustomed. When a piece of code throws an exception, the program control immediately stops executing code step by step and transitions to the exception handler, which could be anywhere from the next line in the same function to several function calls up the stack. 

In C++, a function can throw any exceptions it wants, unless it specifies that it will not throw any exceptions (using the `noexcept` keyword).

### Exceptions Mechanics

If the `catch` block does not do a control transfer—for example, by returning a value, throwing a new exceptions, or rethrowing the exceptions—then the "remaining code" is executes after the last statement of that `catch` block.

If the functions fails to open the file and executes the `throw exception()` linem the rest of the function is skipped, and control transitions to the nearest exception handler. 

Exception Types

You can throw an exception of any type. The preceding example throws an object of type `std::exception`, but exceptions do not need to be objects. You could throw a simple `int`. 

You should generally throw objects as exceptions for two reasons:

- Objects convey information by their class name
- Objects can store information, including strings that describe the exceptions

Always catch exception objects by `const` reference! This avoids object slicing, which could happen when you catch exception objects by value.

Matching Any Exception

You can write a `catch` line that matches any possible exception with the special syntax:

```cpp
try {
    ...
} catch(...) {
    return 1;
}
```

Uncaught Exceptions

If your program throws an exceptions that is not caught anywhere, the program terminates. 

**noexcept**

A function is allowed to throw any exception it likes. However, it is possible to mark a function with the `noexcept` keyword to state that it will not throw any exceptions. When a function markes as `noexcept` throws an exception anyway, C++ calls `terminate()`.

### Exceptions and Polymorphism

Exception classes are usually written in a hierarchy, so that you can employ polymorphism when you catch the exceptions.

A feature of exception hierarchies is that you can catch exceptions polymorphically. For example, if you look at the two `catch` statements in `main()` following the call to `readIntegerFile()`, you can see that they are identical except for the exception class that they handle. 

When you catch exceptions polymorphically, make sure to catch them by reference! If you catch exceptions by value, you can encouter slicing, in which case you lose information from the object. 