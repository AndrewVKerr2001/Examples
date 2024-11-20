//Author: Andrew Kerr
//Date: Nov 3 2021

#ifndef __GROCERY_LIST_HPP_
#define __GROCERY_LIST_HPP_

#include <iostream>
#include "GroceryItem.hpp"

class GroceryNode {
private:
	//The item associated/stored in this node.
	GroceryItem* _item;
	//The next node in the chain of nodes, or NULL if no node is available.
	GroceryNode* _next;
	
public:
	//Retrieves the underlying item as a pointer object in constant form.
	const GroceryItem* item() const;
	//Retrieves the underlying item as a pointer object.
	GroceryItem*& item();
	//Retrieves the underlying next node as a pointer object in constant form.
	const GroceryNode* next() const;
	//Retrieves the underlying next node as a pointer object.
	GroceryNode*& next();
};

class GroceryList
{
private:
	//The head of the list.
	GroceryNode* _head;
	//The tail of the list, used for quick appends.
	GroceryNode* _tail;
	//The stored length of the list, cached so we don't have to calculate it each time getLength is called.
	int _length;
	
public:
	//A constructor for the GroceryList
	GroceryList();
	//A deconstructor for the GroceryList
	~GroceryList();
	
private:
	//Clears the list, deleting all nodes+data.
	void clear();
	
public:
	//The length of the array.
	int getLength() const;

public:
	//Checks off the item given by the name provided.
	void checkOff(const std::string&) const;
	
public:
	//Sets this list equal to the given list.
	GroceryList& operator=(const GroceryList&);
	//Appends the given list to the end of this list (or merges items if already exists).
	GroceryList& operator+=(const GroceryList&);
	//Appends/Merges the given item to the end of the list or into the existing entry if one exists.
	GroceryList& operator+=(const GroceryItem&);
	//Removes the given item (by name) from the list.
	GroceryList& operator-=(const std::string&);
	//Attempts to get the given item (by name) from the list.
	GroceryItem* operator[](const std::string&) const;
	
public:
	//Print's the given list's items to the ostream.
	friend std::ostream& operator<<(std::ostream&, const GroceryList&);
	
};

#endif
