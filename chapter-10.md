## Chapter 10: Discovering Inheritance Techniques

From the perspective of other code, an object belongs to its defined class as well as to any base classes.

A pointer or reference to an object can refer to an object of the declared class or any of its derived classes. The concept to understand now is that a pointer to a `Base` can actually be pointing to a `Derived` object. The same is true for a reference. The client can still access only the methods and data members that exist in `Base`, but through this mechanism, any code that operates on a `Base` can also operate on a `Derived`. 

For example, the following code compiles and works just fine, even though it initially appears that there is a type mismatch:

```cpp
Base* base = new Derived(); // Create Derived, store it in Base pointer
```

However, you cannot call methods from the `Derived` class through the `Base` pointer. The following does not work:

```cpp
base->someOtherMethod();
```

A derived class can access `public` and `protected` methods and data members declared in its base class as though they were its own, because they are.

Methods should also be `private` by default. Only make those methods `public` that are designed to be public, and make methods `protected` if you want only derived classes to have access to them. 

C++ allows you to mark a class as `final`, which means trying to inherit from it will result in a compilation error. 

```cpp
class Base final
{
    ...
};
```

There is one small twist to overriding methods in C++ and it has to do with the keyword `virtual`. Only methods that are declared as `virtual` in the base class can be overridden properly by derived classes. 

```cpp
class Base
{
    public:
        virtual void someMethod();
}
```

The `virtual` keyword has a few subtleties. A good rule of thumb is to just make all of your methods `virtual`. That way, you won't have to worry about whether or not overriding the method will work. The only drawback is a very tiny performance hit. 

As a rule of thumb, make all your methods `virtual` (including the destructor, but not constructors) to avoid problems associated with omission of the `virtual` keyword. Note that the compiler-generated destructor is not `virtual`!

To override a method, you declare it in the derived class definition exactly as it was declard in the base class, and you add the `override` keyword. In the derived class's implementation file, you provide the new definition. 

For example, the `Base` class contains a method called `someMethod()`. The definition of `someMethod()` is provided in `Base.cpp`:

```cpp
void Base::someMethod()
{
    ...
}
```

Note that you do not repeat the `virtual` keyword in front of the method definition.

If you want to provide a new definition for `someMethod()` in the `Derived` class, you must first add it to the class definition for `Derived`, as follows:

```cpp
class Derived : public Base
{
    public:
        virtual void someMethod() override; // overrides Base's someMethod()
}
```

Adding the `override` keyword is not mandatory, but it is highly recommended. The new definition of `someMethod()` is specified along with the rest of `Derived`'s methods in `Derived.cpp`.

```cpp
void Derived::someMethod()
{
    ...
}
```

Once a method or destructor is marked as `virtual`, it is `virtual` for all derived classes even if the `virtual` keyword is removed from derived classes.

A pointer or reference can refer to an object of a class or any of its derived classes. The object itself "knows" the class of which it is actually a member, so the appropriate method is called as long as it was declared `virtual`. 

```cpp
Derived myDerived;
Base& ref = myDerived;
ref.someMethod();   // calls Derived's version of someMethod()
```

Remember that even though a base class reference or pointer knows that it is actually a derived class, you cannot access derived class methods or members that are not defined in the base class. 

```cpp
Derived myDerived;
Base& = myDerived;
myDerived.someOtherMethod();    // this is fine
ref.someOtherMethod();  // error
```

The derived class knowledge is _not_ true for non-pointers or non-reference objects. You can cast or assign a `Derived` to a `Base` because a `Derived` is a `Base`. However, the object loses any knowledge of the derived class at that point.

```cpp
Derived myDerived;
Base assignedObject = myDerived;    // assigns a Derived to a Base
assignedObject.someMethod();    // calls Base's version of someMethod()
```

Derived classes retain their overridden methods when referred to by base class pointers or references. They lose their uniqueness when cast to a base class object. The loss of the derived class's data members and overridden methods is called slicing.

If a method is not `virtual`, you can still attempt to override it, but it will be wrong in subtle ways.

**Hiding Instead of Overriding**

The following code shows a base class and a derived class, each with a single method. 

```cpp
class Base
{
    public:
        void go() { ... }
};

class Derived : public Base
{
    public:
        void go() { ... }
};
```

Because the method is not `virtual`, it is not actually overridden. Rather, the `Derived` class creates a new method, also called `go()`, that is completely unrelated to the `Base` class's method called `go()`. To prove this:

```cpp
Derived myDerived;
Base& ref = myDerived;
ref.go();
```

You would expect the output to be "go() called on Derived", but in fact, the output is "go() called on Base". This is because the `ref` variable is a `Base` reference and because the `virtual` keyword was omitted. When the `go()` method is called, it simply executes `Base`'s go() method. Because it is not `virtual`, there is no need to consider whether a derived class has overridden it.

**How virtual is Implemented**

When a class is compiled in C++, a binary object is created that contains all methods for the class. In the non-`virtual` case, the code to transfer control to the approriate method is hard-coded directly where the method is called based on the compile-time type. This is called _static binding_, also known as _early binding_.

If the method is declared `virtual`, the correct implementation is called through the use of a special area of memory called the `vtable`, or "virtual table". Each class has one or more virtual methods, has a vtable, and every object of such a class contains a pointer to said vtable. This vtable contains pointers to the implementations of the `virtual` methods. In this way, when a method is called on an object, the pointer is followed into the vtable and the approriate version of the method is executed based on the actual type of the object at run time. This is called _dynamic binding_, also known as late binding. 

```cpp
#include "WeatherPrediction.h"

class MyWeatherPrediction : public WeatherPrediction
{
    public:
        virtual void setCurrentTempCelsius(int temp);
        virtual int getTomorrowTempCelsius() const;
    private:
        static int convertCelsiusToFahrenheit(int celsius);
        static int convertFahrenheitToCelsius(int fahrenheit);
};
```

These methods can be `static` because they are the same for all instances of the class. 

The new methods follow the same naming convention as the parent class. Remember that from the point of view of other code, a `MyWeatherPrediction` object has all of the functionality defined in both `MyWeatherPrediction` and `WeatherPrediction`. 

```cpp
void MyWeatherPrediction::setCurrentTempCelsius(int temp)
{
    int fahrenheitTemp = convertCelsiusToFahrenheit(temp);
    setCurrentTempFahrenheit(fahrenheitTemp);
}
```

**Replacing Functionality in a Derived Class**

The other major technique for inheritance is replacing existing functionality. The `showResult()` method in the `WeatherPrediction` class is in dire need of a facelift. `MyWeatherPrediction` can override this method to replace the behavior with its own implementation.

```cpp
class MyWeatherPrediction : public WeatherPrediction
{
    public:
        virtual void showResult() const override;
}
```

```cpp
void MyWeatherPrediction::showResult() const
{
    ...
}
```

### Respect your Parents

When you write a derived class, you need to be aware of the interaction between parent classes and child classes. Issues such as order of creation, constructor chaining, and casting are all potential sources of bugs.

C++ defines the creation order as follows:

1. If the class has a base class, the default constructor of the base class is executed, unless there is a call to a base class constructor in the ctor-initializer, in which case that constructor is called instead of the default constructor.
2. Non-`static` data members of the class are constructed in the order in which they are declared. 
3. The body of the class's constructor is executed.

**Referring to Parent Names**

When you override a method in a derived class, you are effectively replacing the original as far as the other code is concerned. However, that parent version of the method still exists and you may want to make use of it. For example, an overridden method would like to keep doing what the base class implementation does, plus something else. Take a look at the `getTemperature()` method in the `WeatherPrediction` class that returns a `string` representation of the current temperature:

**Casting Up and Down**

As you have already seen, an object can be cast or assigned to its parrent class. 

```cpp
Base myBase = myDerived;    // slicing!
```

Slicing occurs in situations like this because the end result is a `Base` object, and `Base` objects lack the additional functionality defined in the `Derived` class. However, slicing does _not_ occur if a derived class is assigned to a pointer or reference to its base class:

```cpp
Base& myBase = myDerived;   // no slicing!
```

This is generally the correct way to refer to a derived class in terms of its base class, also called _upcasting_. This is why it's always a good idea to make your methods and functions take reference to classes instead of directly using objects of those classes. By using references, derived classes can be passed in without slicing.

When upcasting, use a pointer or reference to the base class to avoid slicing.

Casting from a base class to one of its derived classes, also called _downcasting_, it is often frowned upon by professional C++ programmers because there is no guarantee that the object really belongs to that derived class, and because downcasting is a sign of bad design.

```cpp
void presumptuous(Base* base)
{
    Derived* myDerived = statc_cast<Derived*>(base);
    // proceed to access Derived methods on myDerived
}
```

Downcasting is sometimes necessary, and you can use it effectively in controlled circumstances. However, if you are going to downcast, you should use a `dynamic_cast()`, which uses the object's built-in knowledge of its type to refuse a cast that doesn't make sense. This built-in knowledge typically resides in the vtable, which means that `dynamic_cast()` works only for objects with a vtable, that is, objects with at least one virtual member. 

```cpp
void lessPresumptuous(Base* base)
{
    Derived* myDerived = dynamic_cast<Derived*>(base);
    if (myDerived != nullptr) {
        // Proceed to access Derived methods on myDerived
    }
}
```

### Inheritance for Polymorphism

Now that you understand the relationship between a derived class and its parent, you can use inheritance in its most powerful scenario—polymorphism. 

The `SpreadsheetCell` base class is responsible for defining the behaviors that all `SpreadsheetCell`-derived classes will support. In this example, all cells need to be able to set their value as a string. All cells also need to be able to return their current value as a `string`. The base class definition declares these methods, as well as an explicitly defaulted `virtual` destructor, but note that it has no data members:

```cpp
class SpreadsheetCell
{
    public:
        virtual ~SpreadsheetCell() = default;
        virtual void set(set::string_view inString);
        virtual std::string getString() const;
}
```

_Pure virtual methods_ are methods that are explicitly undefined in the class definition. By making a method pure virtual, you are telling the compiler that no definition for the method exists in the current class. A class with at least one pure virtual method is said to be an _abstract class_ because no other code will be able to instantiate it. The compiler enforces the fact that if a class contains one or more pure virtual methods, it can never be used to construct an object of that type. 

There is a special syntax for designating a pure virtual method. The method declaration is followed by `=0`. 

```cpp
class SpreadsheetCell
{
    public:
        virtual ~SpreadsheetCell() = default;
        virtual void set(std::string_view inString) = 0;
        virtual std::string getString() const = 0;
};
```

### Leveraging Polymorphism

Now that the `SpreadsheetCell` hierarchy is polymorphic, client code can take advantage of the many benefits that polymorphism has to offer. 

### Future Considerations

The new implementation of the `SpreadsheetCell` hierarchy is certainly an improvement from an object-oriented design point of view. Yet, it would probably not suffice as an actual class hierarchy for a real-world spreadsheet program for several reasons. 

First, despite the improved design, one feature is still missing: the ability to convert from one cell type to another. By dividing them into two classes, the cell objects become more loosely integrated. 

To provide the ability to convert from a `DoubleSpreadsheetCell` to a `StringSpreadsheetCell` you could add a _converting constructor_, also known as a _typed constructor_. It has a similar appearance as a copy constructor, but instead of a reference to an object of the same class, it takes a reference to an object of a sibling class. Note also that you now have to declare a default constructor, which can be explicitly defaulted, because the compiler stops generating one as soon as you declare any constructor yourself:

```cpp
class StringSpreadsheetCell : public SpreadsheetCell
{
    public:
        StringSpreadsheetCell() = default;
        StringSpreadsheetCell(const DoubleSpreadsheetCell& inDoubleCell);
    ...
}
```

This converting constructor can be implemented as follows:

```cpp
StringSpreadsheetCell::StringSpreadsheetCell(
    const DoubleSpreadsheetCell& inDoubleCell)
{
    mValue = inDoubleCell.getString();
}
```

One approach is to implement a version of each operator for every combination of cells. With only two derived classes, this is manageable. There would be an `operator+` function to add two `double` cells, to add two `string` cells, and to add a `double` cell to a `string` cell. Another approach is to device on a common representation. The preceding implementation already standardizes on a `string` as a common representation of sorts. One possible implementation, which assumes that the result of adding two cells is always a `string` cell, is as follows:

```cpp
StringSpreadsheetCell operator+(const StringSpreadsheetCell& lhs,
                                const StringSpreadsheetCell& rhs)
{
    StringSpreadsheetCell newCell;
    newCell.set(lhs.getString() + rhs.getString());
    return newCell;
}
```

### Multiple Inheritance

As you read in Chapter 5, multiple inheritance is often perceived as a complicated and unnecessary part of object-oriented programming. 

**Inheriting from Multiple Classes**

```cpp
class Baz : public Foo, public Bar
{

};
```

By listing multiple parents, the `Baz` object has the following characteristics:

- A `Baz` objects supports the `public` methods, and contains the data members of both `Foo` and `Bar`.
- The methods of the `Baz` class have access to `protected` data and methods in both `Foo` and `Bar`.
- A `Baz` object can be upcast to either a `Foo` or a `Bar`.
- Creating a new `Baz` object automatically calls the `Foo` and `Bar` default constructors, in the order that the classes are listed in the class definition.
- Deleting a `Baz` object automatically calls the destructors for the `Foo` and `Bar` classes, in the reverse order that the classes are listed in the class definition.

**Naming Collisions and Ambigious Base Classes**

The solution to the ambiguity is to either explicitly upcast the object using a `dynamic_cast()`, essentially hiding the undesired version of the method from the compiler, or to use a _disambiguation syntax_. 

```cpp
dynamic_cast<Dog&>(myConfusedAnimal).eat();
myConfusedAnimal.Dog::eat();
```

**Ambiguous Base Classes**

**Uses for Multiple Inheritance**

At this point, you're probably wondering why programmers would want to tackle multiple inheritance in their code. The most straightforward use case for multiple inheritance is to define a class of objects that is-a something and also is-a something else. 

### Special Cases in Overriding Methods

1. The Base Class Method is Static

In C++, you cannot override a `static` method. there are, however, a few corollaries that you need to understand.

First of all, a method cannot be both `static` and `virtual`. This is the first clue that attempting to override a `static` method will not do what you intend to do. If you have a `static` method in your derived class with the same name as a `static` method in your base class, you actually have two seperate methods. 

In C++, you can call a `static` method using an object, but because the method is `static`, it has no `this` pointer and no access to the object itself, so it is equivalent to calling it by `classname::method()`. Referring to the previous example classes, you can write code as follows, but the results may be surprising:

```cpp
DerivedStatic myDerivedStatic;
BaseStatic& ref = myDerivedStatic;
myDerivedStatic.beStatic();
ref.beStatic();
```

2. The Base Class Method is Overloaded

When you override a method by specifying a name and a set of parameters, the compiler implicitly hides all other instances of the name in the base class. The idea is that if you have overridden one method of a given name, you might have intended to override all methods of that name, but simply forgot, and therefore this should be treated as an error. It makes sense if you think about it—why would you want to change some versions of a method and not others?

3. The Base Class Method is privater or protected

There's absolutely nothing wrong with overriding a `private` or `protected` method. Remember that the access specifier for a method determines who is able to _call_ the method. Just because a derived class can't call its parent's `private` methods doesn't mean it can't override them. In fact, overriding a `private` or `protected` method is a common pattern in C++. 

Overriding `private` and `protected` methods is a good way to change certain features of a class without a major overhaul.

4. The Base Class Method has Default Arguments

Derived classes and base classes can each have different default arguments, but the arguments that are used depend on the declared type of the variable, not the underlying object. Following is a simple example of a derived class that provides a different default argument in an overridden method:

```cpp
class Base
{
    public:
        virtual ~Base() = default;
        virtual void go(int i = 2) {
            cout << "Base's go with i=" << i << endl; 
        }
};

class Derived : public Base
{
    public:
        virtual void go(int = 7) override {
            cout << "Derived's go with i=" << i << endl;
        }
}
```

If `go()` is called on a `Derived` object, `Derived`'s version of `go()` is executed with the default argument of 7. If `go()` is called on a `Base` object, `Base`'s version of `go()` is called with the default argument of 2. However (and this is the weird part), if `go()` is called on a `Base` pointer or `Base` reference that really points to a `Derived` object, `Derived`'s version of `go()` is called but with `Base`s default argument of 2. 

```cpp
Base myBase;
Derived myDerived;
Base& myBaseReferenceToDerived = myDerived;
myBase.go();
myDerived.go();
myBaseReferenceToDerived.go();
```

The output of this code is as follows:

```cpp
Base's go with i=2
Derived's go with i=7
Derived's go with i=2
```

The reason for this behavior is that C++ uses the compile-time type of the expression to bind default arguments, not the run-time type. Default arguments are not "inherited" in C++. If the `Derived` class in this example failed to provide a default argument as its parents did, it would be overloading the `go()` method with a new non-zero-argument version.

When overriding a method that has a default argument, you should provide a default argument as well, and it should probably be the same value. It is recommended to use a symbolic constant for default values so that the same symbolic constant can be used in derived classes. 

5. The Base Class Method has a Different Access Level

There are two ways you may want to change the access level of a method: you could try to make it more restrictive or less restrictive. Neither case makes much sense in C++, but there are a few legitimate reasons for attempting to do so.

To enforce tighter restrictions on a method (or a data member for that matter), there are two approaches you can take. One way is to change the access specifier for the entire base class. This approach is described later in this chapter. The other approach is simply to redefine the access in the derived class.

```cpp
class Gregarious
{
    public:
        virtual void talk() {
            cout << "Gregarious says hi!" << endl;
        }
};

class Shy : public Gregarious
{
    protected:
        virtual void talk() {
            cout << "Shy reluctantly says hi" << endl;
        }
};
```

The `protected` version of `talk()` in the `Shy` class properly overrides the `Gregarious::talk()` method. Any client code that attempts to call `talk()` on a `Shy` object get a compilation error.

The previous example redefined the method in the derived class because it wants to display a different message. If you don't want to change the implementation, but instead only want to change the access level of a method, the preferred way is to simply add a `using` statement in the derived class definition with the desired access level.

It is much easier (and makes more sense) to _lessen_ access restrictions in derived classes. The simplest way is to provide a `public` method that calls a `protected` method from the base class.

### Copy Constructors and Assignment Operators in Derived Classes

Chapter 9 says that providing a copy constructor and assignment operator is considered a good programming practice when you have dynamically allocated memory in a class. When defining a derived class, you need to be careful about copy constructors and `operator=`.

If your derived class does not have any special data (pointers, usually) that require a nondefault copy constructor or `operator=`, you don't need to have one, regardless of whether or not the base class has one. If your derived class omits the copy constructor or `operator=`, a default copy constructor or `operator=` will be provided for the data members specified in the derived class, and the base class copy constructor or `operator=` will be used for the data members specified in the base class. 

On the other hand, if you _do_ specify a copy constructor in the derived class, you need to explicitly chain to the parent copy constructor, as shown in the following code. If you do not do this, the default constructor (not the copy constructor!) will be used for the parent portion of the object.

### Casts

The old C-style casts with `()` still work in C++, and are still used extensively in existing code bases. C-style casts cover all four C++ casts, so they are more error-prone because it's not always obvious what you are trying to achieve, and you might end up with unexpected results. It is strongly recommended to only use the C++ style casts in new code because they are safer and stand out better syntactically in your code.

**const_cast()**

`const_cast()` is the most straightforward of the different casts available. You can use it to add `const`-ness to a variable, or cast away `const`-ness of a variable. It is the only cast of the four that is allowed to cast away `const`-ness. Theoretically, of course, there should be no need for a `const` cast. If a variable is `const`, it should stay `const`. In practice, however, you sometimes find yourself in a situation where a function is specified to take a `const` variable, which it must then pass to a function that takes a non-`const` variable. The "correct" solution would be to make `const` consistent in the program, but that is not always an option, especially if you are using third-party libraries. Thus, you sometimes need to cast away the `const`-ness of a variable, but you should only do this when you are sure the function you are calling will not modify the object; otherwise, there no other option than to restructure your program. 

```cpp
extern void ThirdPartyLibraryMethod(char * str);

void f(const char* str)
{
    ThirdPartyLibraryMethod(const_cast<char*>(str));
}
```

Starting with C++17, there is a helper method called `std::as_const()`, defined in `<utility>`, that returns a `const` reference version of its reference parameter. Basicallym `as_const<obj>` is equivalent to `const_cast<const T&>(obj)`, where T is the type of `obj`. As you can see, using `as_const()` is shorter than using `const_cast()`. Here is an example:

```cpp
std::string str = "C++";
const std::string& constStr = std::as_const(str);
```

Watch out when using `as_const()` in combination with `auto`. Remember from Chapter 1 that `auto` strips away reference and `const` qualifiers! So, the following `result` variable has type `std::string`, not `const std::string&`:

```cpp
auto result = std::as_const(str);
```

**static_cast()**

You can use `static_cast()` to perform explicit conversions that are supported directly by the language. For example, if you write an arithmetic expression in which you need to convert an `int` to a `double` in order to avoid integer division, use a `static_cast()`. In this example, it's enough to only use `static_cast()` with `i`, because that makes one of the two operands a `double`, making sure C++ performs floating point division.

```cpp
int i = 3;
int j = 4;
double result = static_cast<double>(i)/j;
```

You can also use `static_cast()` to perform explicit conversions that are allowed because of user-defined constructors or conversion routines. For example, if class `A` has a constructor that takes an object of class `B`, you can convert a `B` object to an `A` object with `static_cast()`. In most situations where you want this behavior, however, the compiler performs the conversion automatically.

Another use of `static_cast()` is to perform downcasts in an inheritance hierarchy, as in this example:

```cpp
int main()
{
    Base* b;
    Derived* d = new Derived();
    b = d;  // don't need a cast to go up the inheritance hierarchy
    d = static_cast<Derived*>(b);   // need a cast to go down the hierarchy

    Base base;
    Derived derived;
    Base& br = derived;
    Derived& dr = static_cast<Derived&>(br);
    return 0;
}
```

These casts work with both pointers and references. They do not work with objects themselves. 

Note that these casts using `static_cast()` do not perform run-time type checking. They allow you to convert any `Base` pointer to a `Derived` pointer, or `Base` reference to a `Derived` reference, even if the `Base` really isn't a `Derived` at run time. For example, the following code compiles and executes, but using the pointer `d` can result in potentially catastrophic failure, including memory overwrites outside the bounds of the object. 

```cpp
Base* b = new Base();
Derived* d = static_cast<Derived*>(b);
```

To perform the cast safely with run-time type checking, use `dynamic_cast()`, which is explained a little later.

**dynamic_cast()**

`dynamic_cast()` provides a run-time check on casts within an inheritance hierarchy. You can use it to cast pointers or references. `dynamic_cast()` checks the run-time type information of the underlying object at run time. If the cast doesn't make sense, `dynamic_cast()` returns a null pointer (for the pointer version), or throws an `std::bad_cast` exception (for the reference version).

The following example shows a correct use of `dynamic_cast()`:

```cpp
Base* b;
Derived* d = new Derived();
b = d;
d = dynamic_cast<Derived*>(b);
```

The following `dynamic_cast()` on a reference will cause an exception to be thrown:

```cpp
Base base;
Derived derived;
Base& br = base;
try {
    Derived& dr = dynamic_cast<Derived&>(br);
} catch (const bad_cast&) {
    cout << "Bad cast!" << endl;
}
```

Note that you can perform the same casts down the inheritance hierarchy with a `static_cast()` or `reinterpret_cast()`. The difference with `dynamic_cast()` is that it performs run-time (dynamic) type checking, while `static_cast()` and `reinterpret_cast()` perform the casting even if they are erroneous.

As Chapter 10 discusses, the run-time type information is stored in the vtable of an object. Therefore, in order to use `dynamic_cast()`, your classes must have at least one `virtual` method. If your classes don't have a vtable, trying to use `dynamic_cast()` will result in a compilation error. 