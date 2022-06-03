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

