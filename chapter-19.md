## Chapter 19: String Localization and Regular Expressions

Regular expression are a powerful feature. They are a special mini-language for string processing. Regular expressions can be used for several string-related operations.

- Validation: Check if an input string is well-formed (Is the input string a well-formed phone number)
- Decision: Check what kind of string an input represents. (Is the input string the name of a JPEG or a PNG file?)
- Parsing: Extract information from an input string (From a full filename, extract the filename part without the full path and without its extension)
- Transformation: Search substrings and replace them with a new formatted substring (search all occurences of C++17 and replace them with C++)
- Iteration: Search all occurences of a substring (Extract all phone numbers from an input string)
- Tokenization: Split a string into substrings based on a set of delimiters. (Split a string on whitespace, commas, periods and so son to extract the individual words)

There is some important terminology you need to know. 

- Pattern: the actual regular expression is a pattern represented by a string
- Match: determines whether there is a match between a given regular expression and all of the characters in a given sequence [first, last).
- Search: Determines whether there is some substring within a given sequence [first, last) that matches a given regular expression.
- Replace: Identifies substrings in a given sequence, and replaces them with a corresponding new substring computer from another pattern, called a substitution pattern.

Different grammars:

- ECMAScript
- basic: the basic POSIX grammar
- extended: the extended POSIX grammar
- awk: the grammar used by the POSIX awk utility
- grep: the grammar used by the POSIX grep utility
- egrep: the grammar used by the POSIX grep utility with the -E parameter

ECMAScript Syntax

A regular expression pattern is a sequence of characters representing what you want to match. Any character in the regular expression matches itself except for the following special characters:

Anchors

The special characters ^ and $ are called anchors. The ^ character matches the position immediately following a line termination character, and $ matches the position of a line termination character. 

For example, `^test$` matches only the string `test`, and not strings that contain `test` somewhere in the line, such as `1test`, `test2`, `test abc`,...

Wildcards

The wildcard character `.` can be used to match any character except a newline character. For example, the regular expression `a.c` will match `abc`, but not `ab5c`.

Alternation

The `|` character can be used to specify the "or" relationship. For example `a|b` matches a or b.

Grouping

Parentheses `()` are used to mark subexpressions, also called capture groups. Capture groups can be used for several purposes:

- Capture groups can be used to identify individual subsequences of the original string; each marked subexpression (capture group) is returned in the result. For example, take the following regular expression: `(.)(ab|cd)(.)`. It has three marked subexpressions. Running a `regex_search()` with this regular expression on `1cd4` results in a match with four entries. The first entry is the entire match, `1cd4`, followed by three entries for the three marked subexpressions. 

Repetition

Parts of a regular expression can be repeated by using one of four repeats

- `*` matches the preceding part zero or more times. For example, `a*b` matches `b`, `ab`, `aab`,...
- `+` matches the preceding part one or more times. 
- `?` matches the preceding part zero or one time
  
Precedence

Character Set Matches

Instead of having to write `(a|b|...|z)`, which is clumsy and introduces a capture group, a special syntax for specifying sets of characters or ranges of characters is available. A character set is specified between square brackets, and allows you to write [c1c2c3...cn]

`[a-zA-Z]` which recognizes all the letters.

Word Boundaries

A word boundary can mean the following:

The regex library

Everything for the regular expression library is in the `<regex>` header file and in the `std` namespace. The basic template types defined by the regular expression library are

- `basic_regex`: an object representing a specific regular expression
- `match_results`: a substring that matches a regular expression, including all the captured groups.
- `sub_match`: an object containing a pair of iterators into the input sequence. 

The library provides three key algorithms: `regex_match()`, `regex_search()`, and `regex_replace()`. 

**regex_match()**

The `regex_match()` algorithm can be used to compare a given source string with a regular expression pattern. 

