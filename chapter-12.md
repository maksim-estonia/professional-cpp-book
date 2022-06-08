## Chapter 12: Writing Generic Code with Templates

C++ provides language support not only for object-oriented programming, but also for _generic programming_. The fundamental tools for generic programming in C++ are templates. 

### Overview of Templates

The main programming unit in the procedural paradigm is the _procedure_ or _function_. Functions are useful primarily because they allow you to write algorithms that are independent of specific values and can thus be reused for many different values. 

Templates take the concept of parametrization a step further to allow you to parametrize on _types_ as well as _values_. 

### Class Templates

_Class templates_ define a class where the types of some of the variables, return types of methods, and/or parameters to the methods are specified as parameters. Class parameters are useful primarily for containers, or data structures, that store objects. 

```cpp
template <typename T>
class Grid
{
    public:
        explicit Grid(size_t width = kDefaultWidth,
            size_t height = kDefaultHeigth);
        virtual ~Grid() = default;

        // Explicitly default a copy constructor and assignment operator
        Grid(const Grid& src) = default;
        Grid<T>& operator=(const Grid& rhs) = default;

        // Explicitly default a move constructor and assignment operator
        Grid(Grid&& src) = default;
        Grid<T>& operator=(Grid&& rhs) = default;

        std::optional<T>& at(size_t x, size_t y);
        const std::optional<T>& at(size_t x, size_t y) const;

        size_t getHeight() const { return mHeight; }
        size_t getWidth() const { return mWidth; }

        static const size_t kDefaultWidth = 10;
        static const size_t kDefaultHeight = 10;

    private:
        void verifyCoordinate(size_t x, size_t y) const;

        std::vector<std::vector<std::optional<T>>> mCells;
        size_t mWidth, mHeight;
}
```

```cpp
template <typename T>
```

The first line says that the following class definition is a template on one type. Both `template` and `typename` are keywords. The template specifier holds for the entire statement, which in this case is the class definition.

The Grid Class Method Definitions

The `template <typename T>` specifier must precede each method definition for the `Grid` template. The constructor looks like this:

```cpp
template <typename T>
Grid<T>::Grid(size_t width, size_t height)
    : mWidth(width), mHeigth(height)
{
    mCells.resize(mWidth);
    for (auto& column : mCells) {
        column.resize(mHeight);
    }
}
```

Templates require you to put the implementation of the methods in the header file itself, because the compiler needs to know the complete definition, including the definition of methods, before it can create an instance of the template. Some way around this restriction are discussed later. 

Note that the class name before the `::` is `Grid<T>`, not `Grid`. You must specify `Grid<T>` as the class name in all your methods and `static` data member definitions. 

The rest of the method definitions are also similar to their equivalents in the `GameBoard` class with the exception of the appropriate template and `Grid<T>` syntax changes:

```cpp
template <typename T>
void Grid<T>::verifyCoordinate(size_t x, size_t y) const {
    ...
}

template <typename T>
const std::optional<T>& Grid<T>::at(size_t x, size_t y) const
{
    ...
}

template <typename T>
std::optional<T>& Grid<T>::at(size_t x, size_t y)
{
    ...
}
```

If an implementation of a class template method needs a default value for a certain template type parameter, for example T, then you can use the `T()` syntax. `T()` calls the default constructor for the object if `T` is a class type, or generates zero if `T` is a simple type. This syntax is called the zero-initialization syntax. It's a good way to provide a reasonable default value for a variable whose type you don't know yet. 

**Using the Grid Template**

Note also the use of `value_or()`. The `at()` method returns an `std::optional` returns an `std::optional` reference. This `optional` can contain a value or not. The `value_or()` method returns the value inside the `optional` if there is a value; otherwise it returns the argument given to `value_or()`.

If you want to declare a function or method that takes a `Grid` object, you must specify the type stored in that grid as part of the `Grid` type. 

Instead of writing the full `Grid` type every time—for example, `Grid<int>`—you can use a type alias to give it an easier name:

```cpp
using IntGrid = Grid<Int>;
```

### How the compiler processes templates

Selective Instantiation

The compiler always generates code for all virtual methods of a generic class. However, for non-virtual methods, the compiler generates code only for those non-virtual methods that you actually call for a particular type. 

### Distributing Template Code between Files

Normally you put class definitions in a header file and method definitions in a source file. Code that creates or uses objects of the class `#includes` the header file and obtains access to the method code via the linker. Templates don't work taht way. Because they are "templates" for the compiler to generate the actual methods for the instantiated types, both class template definitions and method definitions must be available to the compiler in any source file that uses them. There are several mechanisms to obtain this inclusion.

Template definitions in header files

Alternatively, you can place the template method definitions in a seperate header file that you `#include` in the header file with the class definitions. Make sure the `#include` for the method definitions follows the class definition; otherwise, the code won't compile.

```cpp
template <typename T>
class Grid
{
    ...
};

#include "GridDefinitions.h"
```

Any client that wants to use the `Grid` template needs only to include the `Grid.h` header file. This division helps to keep the distinction between class definitions and method definitions. 

Template Definitions in Source files

The header file looks like this:

```cpp
template <typename T>
class Grid
{
    ...
};

#include "Grid.cpp"
```

When using this technique, make sure you don't add the `Grid.cpp` file to your project, because it is not supposed to be, and cannot be, compiled separately; it should be `#included` only in a header file!

Limit Class Template Instantiations

If you want your class templates to be used only with certain known types, you can use the following technique.

Suppose you want the `Grid` class to be instantiated only for `int`, `double` and `vector<int>`. The header file should look like this:

```cpp
template <typename T>
class Grid
{
    // class definitions
};
```

Note that there are no method definitions in this header file and that there is no `#include` at the end!

In this case, you need a real `.cpp` file added to your project, which contains the method definitions and looks like this:

```cpp
#include "Grid.h"
#include <utility>

template <typename T>
Grid<T>::Grid(size_t width, size_t height)
    : mWidth(width), mHeight(height)
{
    mCells.resize(mWidth);
    for (auto& column: mCells) {
        column.resize(mHeight);
    }
}
```

For this method to work, you need to explicitly instantiate the template for those types that you want to allow clients to use. At the end of the `.cpp` file you can do this as follows:

```cpp
// explicit instantiations for the types you want to allow
template class Grid<int>;
template class Grid<double>;
template class Grid<std::vector<int>>;
```

With these explicit instantiations, you disallow client code from using the `Grid` class template with other types, such as `SpreadsheetCell`.

### Method Templates

C++ allows you te templatize individual methods of a class. Method templates come in useful for assignment operators and copy constructors in class templates.

Consider the original `Grid` template with only one template parameter: the element type. You can instantiate grids of many different types, such as `int`s and `double`s:

However, `Grid<int>` and `Grid<double>` are two different types. If you write a function that takes an object of type `Grid<double>`, you cannot pass a `Grid<int>`. Neither of the following two lines compiles:

```cpp
myDoubleGrid = myIntGrid;   // does not compile
Grid<double> newDoubleGrid(myIntGrid);  // does not compile
```

The problem is that the copy constructor and assignment operator for the `Grid` template are as follows,

```cpp
Grid(const Grid& src);
Grid<T>& operator=(const Grid& rhs);
```

which are equivalent to

```cpp
Grid(const Grid<T>& src);
Grid<T>& operator=(const Grid<T>& rhs);
```

The `Grid` copy constructor and `operator=` both take a reference to a `const Grid<T>`. When you instantiate a `Grid<double>` and try to call the copy constructor and `operator=`, the compiler generates methods with these prototypes:

```cpp
Grid(const Grid<double>& src);
Grid<double>& operator=(const Grid<double>& rhs);
```

Note that there are no constructors or `operator=` that take a `Grid<int>` within the generated `Grid<double>` class.

