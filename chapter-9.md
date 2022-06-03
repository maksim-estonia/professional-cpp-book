## Chapter 9: Mastering Classes and Objects

### Dynamic Memory Allocation in Objects

Dynamically allocated memory in objects provides several challenges:

- freeing the memory
- handling object copying
- handling object assignment

### Handling Moving with Move Semantics

Move semantics for objects requires a move constructor and a move assignment operator. These can be used by the compiler when the source object is a temporary object that will be destroyed after the operation is finished. Both the move constructor and the move assignment operator move the data members from the source object to the new object, leaving the source object in some valid but otherwise indeterminate state. Often, data members of the source object are reset to null values. This process actually moves ownership of the memory and other resources from one object to another object. They basically do a _shallow_ copy of the member variables, and switch ownership of allocated memory and other resources to prevent dangling pointers or resources, and to prevent memory leaks. 

An _rvalue reference_ is a reference to an rvalue. In particular, it is a concept that is applied when the rvalue is a temporary object. The purpose of an rvalue reference is to make it possible for a particular function to be chosen when a temporary object is involved. The consequence of this is that certain operations that normally involve copying large values can be implemented by copying pointer to those values, knowing the temporary object will be destroyed. 

A function can specify an rvalue reference parameter by using `&&` as part of the parameter specification; for example, `type&& name`. Normally, a temporary object will be seen as a `const type&`, but when there is a function overload that uses an rvalue reference, a temporary object can be resolved to that overload. The following example demonstrates this. 

```cpp
// lvalue reference parameter
void handleMessage(std::string& message)
{
    ...
}

// rvalue reference parameter
void handleMessage(std::string&& message)
{
    ...
}
```

You can call `handleMessage()` with a named variable as argument:

```cpp
std::string a = "Hello";
std::string b = " world";
handleMessage(a); // calls handleMessage(string& value)
```

Because `a` is a named variable, the `handleMessage()` function accepting an lvalue reference is called. Any changed `handleMessage()` does through its reference parameter will change that value of `a`.

You can also call `handleMessage()` with an expression as argument:

```cpp
handleMessage(a + b); // calls handleMessage(string&& value)
```

The `handleMessage()` function accepting an lvalue reference cannot be used, because the expression `a + b` results in a temporary, which is not an lvalue. In this case the rvalue reference version is called. Because the argument is temporary, any changes `handleMessage()` does through its reference parameter will be lost after the call returns. 

A literal can also be used as argument to `handleMessage()`. This also triggers a call to the rvalue reference version because a literal cannot be an lvalue (though a literal can be passed as argument to a `const` reference parameter).

```cpp
handleMessage("Hello world");   // calls handleMessage(string&& value)
```

If you remove the `handleMessage()` function accepting an lvalue reference, calling `handleMessage()` with a named variable like `handleMessage(b)` will result in a compilation error because an rvalue reference parameter (`string&& message`) will never be bound to an lvalue (`b`). You can force the compiler to call the rvalue reference version of `handleMessage()` by using `std::move()`, which casts an lvalue into an rvalue as follows:

```cpp
handleMessage(std::move(b)); // calls handleMessage(string&& value)
```

As said before, a _named variable is an lvalue_. So, inside the `handleMessage()` function, the rvalue reference `message` parameter itself is an lvalue because it has a name! If you want to forward this rvalue reference parameter to another function as an rvalue, then you need to use `std::move()` to cast the lvalue to an rvalue. 

**Implemeting Move Semantics**

Move constructors and move assignment operators should be marked with the `noexcept` qualifier to tell the compiler that they don't throw any exceptions. 

```cpp
class Spreadsheet
{
    public:
        Spreadsheet(Spreadsheet&& src) noexcept;    // Move constructor
        Spreadsheet& operator=(Spreadsheet&& rhs) noexcept; // Move assign
    private:
        ...
}
```

### More about Methods

**static methods**

Methods, like data members, sometimes apply to the class as a whole, not to each object. You can write `static` methods as well as data members. As an example, consider the SpreadsheetCell class. It has two helper functions: `stringToDouble()` and `doubleToString()`. These methods don't access information about specific objects, so they could be `static`. 

```cpp
class SpreadsheetCell
{
    private:
        static std::string doubleToString(double inValue);
        static double stringToDouble(std::string_view inString);
}
```

Note that `static` methods are not called on a specific object, so they have no `this` pointer, and are not executing for a specific object with access to its non-`static` members. 

**const Methods**

A `const` object is an object whose value cannot be changed. If you have a `const`, reference to `const`, or pointer to a `const` object, the compiler does not let you call any methods on that object unless those method guarantee that they won't change any data members. 

You can call `const` and non-`const` methods on a non-`const` object. However, you can only call `const` methods on a `const` object.

```cpp
SpreadsheetCell myCell(5);
cout << myCell.getValue() << endl;  //ok
myCell.setString("6");  //ok

const SpreadsheetCell& myCellConstRef = myCell;
cout << myCellConstRef.getValue() << endl; //ok
myCellConstRef.setString("6");  //compilation error!
```

**mutable Data Members**

```cpp
class SpreadsheetCell
{
    private:
        double mValue = 0;
        mutable size_t mNumAccesses = 0;
};

double SpreadsheetCell::getValue() const
{
    mNumAccesses++;
    return mValue;
}

std::string SpreadsheetCell::getString() const
{
    mNumAccesses++;
    return doubleToString(mValue);
}
```

### Method Overloading

**Overloading Based on const**

Often, the implementation of the `const` version and the non-`const` version is identical. To prevent code duplication, you can use the `const_cast()` pattern. For example, the `Spreadsheet` class has a method called `getCellAt()` returning a non-`const` reference to a `SpreadsheetCell`. You can add a `const` overload that returns a `const` reference to a `SpreadsheetCell` as follows:

```cpp
class Spreadsheet
{
    public:
        SpreadsheetCell& getCellAt(size_t x, size_t y);
        const SpreadsheetCell& getCellAt(size_t x, size_t y) const;
};
```

You should implement the `const` version as you normally would, and implement the non-const version by forwarding the call to the `const` version with the appropriate casts. Basically, you cast `*this` to a `const Spreadsheet&` using `std::as_const()` (defined in `<utility>`), call the const version of `getCellAt()`, and then remove the `const` from the result by using a `const_cast()`:

```cpp
const SpreadsheetCell& Spreadsheet::getCell(size_t x, size_t y) const
{
    verifyCoordinate(x, y);
    return mCells[x][y];
}

SpreadsheetCell& Spreadsheet::getCellAt(size_t x, size_t y)
{
    return const_cast<SpreadsheetCell&>(std::as_const(*this).getCellAt(x, y));
}
```

The `std::as_const()` function is available since C++17. If your compiler doesn't support it yet, you can use the following `static_cast()` instead:

```cpp
return const_cast<SpreadsheetCell&>(static_cast<const Spreadsheet&>(*this).getCellAt(x, y));
```

**Explicitly Deleting Overloads**

Overloaded methods can be explicitly deleted, which enables you to disallow calling a method with particular arguments. 

```cpp
class MyClass
{
    public:
        void foo(int i);
};
```

The `foo()` method can be called as follows:

```cpp
MyClass c;
c.foo(123);
c.foo(1.23);
```

The compiler converts the `double` value (1.23) to an integer value (1) and then calls `foo(int i)`. The compiler might give you a warning, but it will perform this implicit conversion. You can prevent the compiler from peforming this conversion by explicitly deleting a `double` instance of `foo()`:

```cpp
class MyClass
{
    public:
        void foo(int i);
        void foo(double d) = delete;
}
```

With this change, an attempt to call `foo()` with a `double` will be flagged as an error by the compiler, instead of performing a conversion to an integer.

### Inline Methods

Inlining is safer than using `#define` macros.

You can specify an `inline` method by placing the `inline` keyword in front of its name in the method definition. 

```cpp
inline double SpreadsheetCell::getValue() const
{
    mNumAccesses++;
    return mValue;
}

inline std::string SpreadsheetCell::getString() const
{
    mNumAccesses++;
    return doubleToString(mValue);
}
```

This gives a hint to the compiler to replace calls to `getValue()` and `getString()` with the actual method body instead of generating code to make a function call. Note that the `inline` keyword is just a hint for the compiler. The compiler can ignore if it thinks it would hurt performamce.

### Default Arguments

A feature similar to method overloading in C++ is default arguments. There is a limitation, though: you can only provide defaults for a continuous list of parameters starting from the rightmost parameter. Otherwise, the compiler will not be able to match missing arguments to default arguments. 

### Different Kinds of Data Members

In addition to declaring simple data members in your classes, you can create:

- `static` data members that all objects of the class share
- `const` members
- reference members
- `const` reference members
- ...

**static Date Members**

Starting with C++17, you can declare your static data members as _inline_. The benefit of this is that you do not have to allocate space for them in a source file. 

```cpp
class Spreadsheet
{
    private:
        static inline size_t sCounter = 0;
};
```

Note the `inline` keyword. With this class definition, the following line can be removed from the source file:

```cpp
size_t Spreadsheet::sCounter;
```

**Accessing static Data Members within Class Methods**

You can use `static` data members as if they were regular data members from within class methods.

```cpp
class Spreadsheet
{
    public:
        size_t getId() const;
    private:
        static size_t sCounter;
        size_t mId = 0;
};
```

Here is an implementation of the Spreadsheet constructor that assigns the initial ID:

```cpp
Spreadsheet::Spreadsheet(size_t width, size_t height)
    : mId(mCounter++), mWidth(width), mHeight(height)
{
    ...
}
```

You should not copy the ID in the copy assignment operator. Once an ID is assigned to an object, it should never change. Thus, it's recommended to make `mId` a `const` data member. 

**Accessing static Data Members Outside Methods**

Access control specifiers apply to `static` data members: `sCounter` is `private`, so it cannot be accessed from outside class methods. If `sCounter` was `public`, you could access it from outside the class methods by specifying that the variable is part of the `Spreadsheet` class with the `::` scope resolution operator.

However, it is not recommended to have `public` data members (`const static` members discussed in next section are an exeption). You should grant access through `public` get/set methods. If you want to grant access to a `static` data member, you need to implement `static` get/set methods.

**const static Data members**

Data members in your class can be declared `const`, meaning they can't be changed after they are created and initialized. You should use `static const` data members in place of global constants when the constants apply only to your class, also called _class constants_. `static const` data members of integral and enumeration types can be defined and initialized inside the class definition without making them inline variables. 

```cpp
class Spreadsheet
{
    public:
        static const size_t kMaxHeight = 100;
        static const size_t kMaxWidth = 100;
};
```

You can use these new constants in your constructor as follows:

```cpp
Spreadsheet::Spreadsheet(size_t width, size_t height)
    : mId(sCounter++)
    , mWidth(std::min(width, kMaxWidth))    //std::min() requires <algorithm>
    , mHeight(std::min(height, kMaxHeight))
{
    ...
}
```

### Reference Data Members

The following is a new `Spreadsheet` class definition that uses a _forward declaration_ in one of the header files. 

```cpp
class SpreadsheetApplication;   // forward declaration

class Spreadsheet
{
    public:
        Spreadsheet(size_t width, size_t height,
            SpreadsheetApplication& theApp);
    private:
        SpreadsheetApplication& myApp;
};
```

This definition adds a `SpreadsheetApplication` as a data member. It's recommended to use a reference in this case instead of a pointer because a `Spreadsheet` should always refer to a `SpreadsheetApplication`. This would not be guaranteed with a pointer.

### const Reference Data Members

### Nested Classes

Class definitions can contain more than just member functions and data members. You can also write nested classes and `structs`, declare type alliases, or create enumerated types. Anything declared inside a class is in the scope of that class. If it is public you can access it outside the class by scoping it with the `Classname::` scope resolution syntax. 

### Enumerated Types inside Classes

If you want to define a number of classes inside a class, you should use an enumerated type instead of a collection of `#defines`. For example, you can add support for cell coloring to the `SpreadsheetCell` class as follows:

```cpp
class SpreadsheetCell
{
    public:
        enum class Color { Red = 1, Green, Blue, Yellos };
        void setColor(Color color);
        Color getColor() const;
    private:
        Color mColor = Color::Red;
}
```

The implementation of the `setColor()` and `getColor()` methods is straightforward:

```cpp
void SpreadsheetCell::setColor(Color color) { mColor = color; }

SpreadsheetCell::Color SpreadsheetCell::getColor() const { return mColor; }
```

The new methods can be used as follows:

```cpp
SpreadsheetCell myCell(5);
myCell.setColor(SpreadsheetCell::Color::Blue);
auto color = myCell.getColor();
```

### Building Stable Interfaces

Classes are the main unit of abstraction in C++. You should apply the principles of abstraction to your classes to seperate the interface from the implementation as much as possible. 

**Using Interface and Implementation Classes**

The basic principle is to define two classes for every class you want to write: the _interface class_ and the _implementation class_. The implementation class is identical to the class you would have written if you were not taking this approach. The interface class presents `public` methods identical to those of the implementation class, but it only has one data member: a pointer to an implementation class object. The interface class method implementation simply calls the equivalent methods on the implementation class object. The result of this is that no matter how the implementation changes, it has no impact on the `public` interface class. This recudes the need for recompilation. None of the clients that use the interface class need to be recompiled if the implementation (and only the implementation) changes. Note that this idiom only works if the single data member is a pointer to the implementation class. If it were a by-value data member, the client would have to be recompiled when the definition of the implementation class changes. 

To use this approach with the `Spreadsheet` class, define a public interface class, `Spreadsheet`, that looks like this:

```cpp
#include "SpreadsheetCell.h"
#include <memory>

// Forward declarations
class SpreadsheetApplication

class Spreadsheet
{
    public:
        Spreadsheet(const SpreadsheetApplication& theApp,
            size_t width = kMaxWidth, size_t height = kMaxHeight);
        Spreadsheet(const Spreadsheet& src);
        ~Spreadsheet();

        Spreadsheet& operator=(const Spreadsheet& rhs);

        void setCellAt(size_t x, size_t y, const SpreadsheetCell& cell);
        SpreadsheetCell& getCellAt(size_t x, size_t y);

        size_t getId() const;

        static const size_t kMaxHeight = 100;
        static const size_t kMaxWidth = 100;

        friend void swap(Spreadsheet& first, Spreadsheet& second) noexcept;
    
    private:
        class Impl;
        std::unique_ptr<Impl> mImpl;
}
```

The implementation class, `Impl`, is a private nested class, because no one else beside the `Spreadsheet` class needs to know about the implementation class. The `Spreadsheet` class now contains only one data member: a pointer to an `Impl` instance. The `public` methods are identical to the old `Spreadsheet`.

The nested `Spreadsheet::Impl` class has almost the same interface as the original `Spreadsheet` class. However, because the `Impl` class is a private nested class of `Spreadsheet`, you cannot have the following `friend swap()` function that swaps two `Spreadsheet::Impl` objects:

```cpp
friend void swap(Spreadsheet::Impl& first, Spreadsheet::Impl& second) noexcept;
```

Instead, a private `swap()` method is defined for the `Spreadsheet::Impl` class as follows:

```cpp
void swap(Impl& other) noexcept;
```

The implementation is straightforward, but you need to remember that this is a nested class, so you need to specify `Spreadsheet::Impl::swap()` instead of just `Impl::swap()`. The same holds true for the other members. Here is the `swap()` method:

```cpp
void Spreadsheet::Impl::swap(Impl& other) noexcept
{
    using std::swap;

    swap(mWidth, other,mWidth);
    swap(mHeight, other.mHeight);
    swap(mCells, other.mCells);
}
```

Now that the `Spreadsheet` class has a `unique_ptr` to the implementation class, the `Spreadsheet` class needs to have a user-declared destructor. Since we don't need to do anything in this destructor, it can be defaulted in the implementation file as follows:

```cpp
Spreadsheet::~Spreadsheet() = default;
```

The implementations of the `Spreadsheet` methods, such as `setCellAt()` and `getCellAt()`, just pass the request to the undelying `Impl` object:

```cpp
void Spreadsheet::setCellAt(size_t x, size_t y, const SpreadsheetCell& cell)
{
    mImpl->setCellAt(x, y, cell);
}

SpreadsheetCell& Spreadsheet::getCellAt(size_t x, size_t y)
{
    return mImpl->getCellAt(x, y);
}
```

The constructors for the `Spreadsheet` must construct a new `Impl` to do its work:

```cpp
Spreadsheet::Spreadsheet(const SpreadsheetApplication& theApp,
    size_t width, size_t height)
{
    mImpl = std::make_unique<Impl>(theApp, width, height);
}

Spreadsheet::Spreadsheet(const Spreadsheet& src)
{
    mImpl = std::make_unique<Impl>(*src.mImpl);
}
```

The copy constructor looks a bit strange because it needs to copy the underlying `Impl` from the source spreadsheet. The copy constructor takes a reference to an `Impl`, not a pointer, so you must dereference the `mImpl` pointer to get the object itself so the constructor call can take its reference, not a pointer, so you must dereference the `mImpl` pointer to get the object itself so the constructor call can take its reference. 

The `Spreadsheet` assignment operator must similarly pass on the assignment to the underlying `Impl`:

```cpp
Spreadsheet& Spreadsheet::operator=(const Spreadsheet& rhs)
{
    *mImpl = *rhs.mImpl;
    return *this;
}
```

The first line in the assignment operator looks a little odd. The `Spreadsheet` assignment operator needs to forward the call to the `Impl` assignment operator, which only runs when you copy direct objects. By dereferencing the `mImpl` pointers, you force direct object assignment, which causes the assignment operator of `Impl` to be called. 

