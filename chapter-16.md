## Chapter 16: Overview of the C++ Standard Library

### Overview of the C++ Standard Library

Regular Expressions

Regular expressions are available through the `<regex>` header file. They make it easy to perform so-called pattern-matching, often used in text processing. Pattern-matching allows you to search special patterns in strings and optionally to replace them with a new pattern. 

Filesystem

C++17 introduces a filesystem support library. Everything is defined in the `<filesystem>` header, and lives in the `std::filesystem` namespace. It allows you to write portable code to work with a filesystem. You can use it for querying whether something is a directory or a file, iterating over the contents of a directory, manipulating paths, and retrieving information about files such as their size, ecention, creation time and so on.

Multithreading

Individual threads can be created using the `thread` class from the `<thread>` header. 

In multithreaded code you need to take care that several threads are not reading and writing the same piece of data at the same time. To prevent this, you can use atomics, defined in `<atomic>`, which give you thread-safe atomic access to a piece of data. Other thread synchronization mechanisms are provided by `<condition_variable>` and `<mutex>`.

If you need to calculate something, possibly on a different thread, and get the result back with proper exception handling, you can use `async` and `future`.

Containers

Each container instance stores objects of only one type; that is, they are homogeneous collections. If you need non-fixed-size heterogeneous collections, you can wrap each element in an `std::any` instance and store those `any` instances in a container. Alternatively, you can store `std::variant` instances in a container. A `variant` can be used if the number of different required types is limited and known at compile time. 

### vector

The `<vector>` header file defines `vector`, which stores a sequence of elements and provides random access to these elements. You can think of a `vector` as an array of elements that grows dynamically as you insert elements, and additionally provides some bounds checking. Like an array, the elements of a `vector` are stored in contiguous memory.

Even though inserting and removing elements in the middle of a `vector` requires moving other elements up or down, a `vector` should be you default container! Often, a `vector` will be faster than, for example, a linked list, even for inserting and removing elements in the middle. The reason is that a `vector` is stored contiguously in memory, while a linked list is scattered around in memory. You should only use something like a linked list if a performance profiles tells you that it performs better than a `vector`.

### list

A Standard Library `list` is a _doubly linked list_ structure and is defined in `<list>`. Like an array or `vector`, it stores a sequence of elements. However, unlike an array or `vector`, the elements of a `list` are not necessarily contigious in memory. 

### forward_list

is a singly linked list

### deque

The name `deque` is an abbreviation for _double-ended queue_. A `deque`, provides quick (constant time) element access. It also provides fast insertion and deletion at both ends of the sequence, but it provides slow insertion and deletion in the middle of the sequence. 

### array

Sometimes you know the exact number of elements in your container up front and you don't need the flexibility of a `vector` or a `list`, which are able to grow dynamically to accomodate new elements. There are a number of advantages in using `array`s instead of standard C-style arrays: they always know their own size, and do not automatically get cast to a pointer to avoid certain types of bugs. 

### queue

provides standard first in, first out semantics. 

### priority_queue

A `priority_queue`, provides `queue` functionality in which each element has a priority. Elements are removed from the queue in priority order. In case of priority ties, the order in which elements are removed is undefined. 

### stack

provides standard first-in, last-out semantics. 

### set and multiset

The `set` class template is a set of elements, each element is unique, and there is at most one instance of the element in the set.

You could use a `set` when you need the elements to be in order, to have equal amounts of insertion/deletion and lookups, and you want to optimize peformance for both as much as possible. 

### map and multimap

is an associative array. You can use it as an array in which the index can be any type; for example, a `string`. A `map` stores key/value pairs, and keeps its elements in sorted order, based on the keys. 

A `multimap`, has the same relation to a `map` as a `multiset` does to a `set`. Specifically, a `multimap` is a `map` that allows duplicate keys.

### hash tables

see Chapter 17

### bitset

when you want a collection of flags

### Algorithms

Note that the generic algorithms do not work directly on the containers. They use an intermediary called an _iterator_. 

1. **Non-modifying Sequence Algorithms**: cannot change the values of elements or the order of elements within the sequence.
   1. Search Algorithms
   2. Comparison Algorithms
   3. Counting Algorithms
2. Modifying Sequence Algorithms
3. Operational Algorithms: execute a function on individual elements of a sequence. 
4. Swap and Exchange Algorithms
5. Partition Algorithms
6. Sorting Algorithms
7. Binary Search Algorithms: normally used on sorted sequences
8. Set Algorithms
9. Heap Algorithms
10. Minimum/maximum Algorithms
11. Numerical Processing Algorithms
12. Permutation Algorithms

