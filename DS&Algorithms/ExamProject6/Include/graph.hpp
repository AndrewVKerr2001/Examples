//Author: Andrew Kerr
//Date: 04/27/2022

//===[ INCLUDES ]==================================================================================================================
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdint.h>

//===[ MACROS ]====================================================================================================================

#define DBG(msg) //std::cout << "DBG[" << __FILE__ << "@" << __FUNCTION__ << "(LN" << __LINE__ << ")]: " << msg << std::endl

//===[ CLASSES ]===================================================================================================================

#ifndef __AK_GRAPH_HPP__
#define __AK_GRAPH_HPP__

class Computer;

//An undirectional pathway between two computers, with a given cost associated with it.
class UndirectedPath {
private:
    //One of the two computers, since undirected neither has a direction.
    Computer& _a;
    //One of the two computers, since undirected neither has a direction.
    Computer& _b;
    //The cost for the two computers to be connected.
    int _cost;

public:
    //Constructes an undirected path using the two computers and a cost.
    UndirectedPath(Computer&, Computer&, int);

public:
    //One of the two computers, since undirected neither has a direction.
    Computer& a() { return _a; };
    //One of the two computers, since undirected neither has a direction.
    Computer& b() { return _b; };
    //The cost for the two computers to be connected.
    int cost() const { return _cost; };

public:
    //One of the two computers, since undirected neither has a direction.
    const Computer& a() const { return _a; };
    //One of the two computers, since undirected neither has a direction.
    const Computer& b() const { return _b; };
};

//A class that implementes the basic functionality of a linked list with special sorting methods.
class PathList {
private:
    //The actual node used to link paths together.
    struct PathNode {
        UndirectedPath* _path;
        PathNode* _next;
        PathNode* _prev;
    };
    //The head of the list, the starting node.
    PathNode* _head;
    //The tail of the list, the ending node.
    PathNode* _tail;
    //True if this PathList is responsible for maintaining the memory management for every UndirectedPath in its list, otherwise False.
    bool _owner_of_paths;

public:
    //Constructs a new PathList defaulted to cleanup the paths when deallocated, can be disabled.
    PathList(bool own_paths=true);
    //Destroys the linkedlist when its no longer needed.
    ~PathList();

public:
    //Inserts a given UndirectedPath into the list at its appropriate index.
    void operator+=(UndirectedPath* path);
    //Goes through the given list and inserts all of the given lists nodes into this list at their appropriate index.
    void operator+=(PathList& list);
    
public:
    //Pushes the given path into the list at the appropriate index.
    bool push(UndirectedPath* path);
    //Pops the topmost path from the list (always the lowest cost path).
    UndirectedPath* pop();

public:
    //Checks if the list has no more entries.
    bool isEmpty() const { return _head == NULL; };

public:
    //Pretty prints out the list entries.
    friend std::ostream& operator<<(std::ostream&, const PathList&);
};

//Represents a computer which has a name and a list of paths.
class Computer {
private:
    //The name of the computer.
    std::string _name;
    //A list of UndirectedPaths.
    PathList _paths;

public:
    //Constructs a computer with a given name.
    Computer(const char* name);

public:
    //The name of the computer.
    const std::string name() const { return _name; };
    //A list of UndirectedPaths.
    PathList& paths() { return _paths; };

public:
    //Connects a given computer with this computer given a cost.
    bool connect(Computer& other, int cost);

public:
    //Pretty prints out the computer and its connections.
    friend std::ostream& operator<<(std::ostream&, const Computer&);
};

//A linkedlist of computers.
class Computers {
private:
    //A node that links multiple computers together.
    struct ListNode {
        //A pointer to the computer in the list.
        Computer* _object;
        //The next node in the list.
        ListNode* _next;
    };

private:
    //The head of the list.
    ListNode* _head = NULL;

public:
    //Destroy's the linked list.
    ~Computers();

public:
    //The top most computer in the list.
    Computer* head() const { return (_head ? _head->_object : NULL); };

public:
    //Adds a computer to the list.
    Computers& operator+=(Computer*);
    //Removes a given computer from the list.
    Computers& operator-=(Computer*);
    //Gets a given computer using the given name.
    Computer* operator[](const std::string&);
    //Computer& operator[](int index); //Could be useful for other purposes.

public:
    //Pretty prints the list of computers.
    friend std::ostream& operator<<(std::ostream&, const Computers&);
};

//Attempts to find the solution to the minimum path cost problem.
int solve(Computers&);

#endif//__AK_GRAPH_HPP__