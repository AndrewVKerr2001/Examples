# Basic Commands
These commands are simple and just require their command letter.

#### n - Prints a line seperator used to keep the output nice and tidy.
#### q - Quits the program.

# Advanced Commands
These commands require additional data to be passed to them to work.

## a &lt;type&gt; ...
Adds a new entry into the database for a particular entity __type__.
### a s - Adds a student to the database.
    a s <first-name> <last-name> <phone-#>

    Example: a s Andrew Kerr XXX-XXX-XXXX

### a c - Adds a course to the database.
    a c <course-prefix> <course-number> <course-title> <course-credit-hours>

    Example: a c ABC 1234 Learning_The_Alphabet 2

### a g - Adds a grade letter to the database.
    a g <grade-letter> <grade-point>

    Example: a g A+ 4.0

### a m - Adds a semester to the database.
    a m <semester-code> <year> <time-of-year>

    Example: a m F08 2008 Fall

### a t - Adds a transcript entry to the database.
    a t <first-name> <last-name> <course-prefix> <course-number> <grade-letter> <semester-code>

    Example: a t Andrew Kerr ABC 1234 A+ F08

## l &lt;type [s,c,g,m,t]&gt;
Lists the entries in the database.

## t &lt;first-name&gt; &lt;last-name&gt;
Prints out a particular student's transcript's.

