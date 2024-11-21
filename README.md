# Examples
A repository with a collection of C/C++ projects that I made during my college courses. Imported from my laptop much later down the line. These projects are listed in chronological order.

## Projects (CompSci1)
These projects were lost to time as they used to exist on my older desktop computer. They were all written using Java and were building upon the basic concepts of a programming language. These concepts include, but aren't limited to: code syntax & grammer, if/elif/else block statements, and function arguments & return values. 

## Projects (CompSci2)

### Fibo
One of the first projects that I made for a college course, it predates the [Template](#template) system that I later implemented. This was before college courses introduced the Makefile build system and thus lacks a makefile, however, this doesn't matter much since it's all in a single file. This project simply prompts the user for a number, up to 200, as the project specification had originally specified, and calculates the fibonacci sequence for that number.

### Template
This isn't really a "project" per se, rather it's a Makefile template that was used to create all the other projects.

### Complex
This project simply implements a class, called Complex, that represents complex/imaginary numbers. This project takes in no command line arguments, and simply displays the results of some built-in tests.

### PFA (Partially Filled Array)
A simple Array implementation, allows for an array to be allocated, elements added to it, and dynamic resize when necessary. This was the first introductory project into C/C++ style memory management, as well as the first data structure that I was taught.

### LinkedLists
A simple LinkedList implementation, both single and double linked lists. This project takes in no command line arguments, and simply displays the results of some built-in tests.

### BinarySearchTree
A simple Binary Search Tree implementation. This project takes in no command line arguments, and simply displays the results of some built-in tests.

### Sort
A simple QuickSort implementation. This project takes in no command line arguments, and simply displays the results of some built-in tests.

### TickTackToe
A simple TickTackToe board solver, takes in a file that contains one or more 4x4 tick-tack-toe boards and will attempt to find a move for player 'X' such that player 'O' cannot make a winning move.

    Each file command must be prefaced with either a ? or !.
    
    - ? will cause the solver to read in a 4x4 board, where each character can either be an 'x' character for player 'X', 'o' character for player 'O' and a '.' (dot) character for an empty board space.

    - ! will cause the solver to exit, regardless of if the file still has board data, this was a project requirement.

    Example:
    ?
    [xo.][xo.][xo.][xo.]
    [xo.][xo.][xo.][xo.]
    [xo.][xo.][xo.][xo.]
    [xo.][xo.][xo.][xo.]
    ...
    !

### EightQueens / QueenSolver
These two "projects" are identical, and build off the idea of the [TickTackToe](#ticktacktoe) project. The only difference between them is that EightQueens was written without using a C++ class, while QueenSolver was written with a class. Both of these projects had the same project specification, which was to take an 8x8 board and to try and place 8 chess queens on the board such that all 8 queens can exist on the board with no 2 queens putting either in check. The project specification stated that the size of the board, and the number of queens, must be made into macros so that we (the students) could easily mess with these values to see which combinations would work, and which would fail.

### PrintF
A __very__ simple printf implementation, takes in no command line arguments besides the command pathname; runs a quick example of the printf implementation, and outputs it to the command line. This project wasn't apart of this college course, rather this was an extra project I worked on during my free time in this course.

### ExamProject1
This was the 1st exam project that I had to complete in this class, sadly I don't actually have the original source files. This was lost to time as I transfered my project files from the school computers to my laptop.

### ExamProject2
This was the 2nd exam project that I had to complete in this class, it uses what we learned in the [PFA](#pfa-partially-filled-array) project to implement a grocery list.

### ExamProject3
This was the 3rd exam project that I had to complete in this class, it builds upon the [2nd exam project](#examproject2) and utilizes what was learned in the [LinkedLists](#linkedlists) project to implement the grocery list.

### ExamProject4
This was the 4th exam project that I had to complete in this call, it builds upon the [3rd exam project](#examproject3) and utilizes what was learned in the [BinarySearchTree](#binarysearchtree) project to implement the grocery list.

## Projects (PrinciplesOfDatabaseDesign)

### Project1-1
This was my first attempt at the first project in this class, which was to design a file based database system. This attempt involved using a block based approach, where the file would be split into various blocks with 128 bytes in each block. These blocks would then store the relevant data and/or the relational links between them. The project specification was to have the database setup to store a made up college's data for students, courses, grade-letters, and transcript grades. The specification also stated that both file and command line input had to be supported. This iteration was not used in the final submission as it had been too ambitious of a setup to work.

### Project1-2
This was my second attempt at the first project in this class, the details of which are described in [Project1-1](#project1-1). This attempt went much better and I was able to submit this code without any issues.