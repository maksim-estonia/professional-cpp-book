## Chapter 3: Coding with Style

- The importance of documenting your code, and what kind of documenting styles you can use
- What decomposition means and how to use it
- What naming conventions are
- What formatting rules are

Doxygen parses comments to automatically build HTML documentation, class diagrams, UNIX man pages, and other useful documents. 

```cpp
/**
* Implements the basic functionality of a watermelon
* TODO: Implement updated algorithms!
*/
class Watermelon
{
    public:
        /**
        * @param initialSeeds The starting number of seeds, must be > 0.
        */
        Watermelon(int initialSeeds);
    
        /**
        * Computes the seed ratio, using the Marigold algorithm.
        * @param slowCalc Whether or not to use long (slow) calculations
        * @return The marigold ratio
        */
        double calcSeedRatio(bool slowCalc);
};
```

Decomposition is the practice of breaking up code into smaller pieces. 

_Refactoring_ is the act of restructuring your code.

- Techniques that allow for more abstraction:
  - **Encapsulate Field.** Make a field private and give access to it with getter and setter methods.
  - **Generalize Type.** Create more general types to allow for more code sharing. 
- Techniques for breaking code apart into more logical pieces:
  - **Extract Method.** Turn part of a larger method into a new method to make it easier to understand.
  - **Extract class.** Move part of the code from an existing class into a new class.
- Techniques for improving names and the location of code:
  - **Move Method or Move Field.** Move to a more appropriate class or source file. 
  - **Rename Method or Rename Field.** Change the name to better reveal its purpose.
  - **Pull Up.** In object-oriented programming, move to a base class.
  - **Push Down.** In object-oriented programming, move to a derived class. 

When refractoring code, it is very important to be able to rely on a testing framework that catches any defects that you might introduce. Unit tests, in Chapter 26, are particularly well suited for helping you catch mistakes.

The C++ compiler has a few naming rules:

- Names cannot start with a number
- Names that contain a double underscore (such as `my__name`) are reserver and shall not be used.
- Names that begin with an underscore are reserver

Good variable names:

- `sourceName`, `destinationName`: Distinguishes two objects
- `gSettings`: Conveys global status
- `mNameCounter`: Conveys data member status
- `calculateMarigoldOffset()`: Simple, accurate
- `mTypeString`
- `errorMessage`
- `sourceFile`, `destinationFile`

Naming conventions

Counters

It is customary to use `i` and `j` as counters and inner-loop counters. When working with 2-D data, it's probably easier to use `row` and `column` as indices. 

Prefixes

Many programmers begin their variable names with a letter that provides some information about the variable's type or usage. 

Some potentional prefixes:


Prefix | Example Name | Literal Prefix Meaning | Usage
---------|----------|---------|---------
 m, m_    | mData, m_data               | "member"  | Data member within a class
 s, ms, ms_ | sLookupTable              | "static"  | Static variable or data member
 k          | kMaximumLength            | "konstant"| A constant value
 b, is      | bCompleted, isCompleted   | "Boolean" | Designates a Boolean value
 n, mNum    | nLines, mNumLines         | "number"  | A data member that is also a counter

