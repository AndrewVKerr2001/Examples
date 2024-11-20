//Author: Andrew Kerr
//Date: Oct 20 2021

#ifndef __GROCERY_LIST_HPP_
#define __GROCERY_LIST_HPP_

#include <iostream>
#include "GroceryItem.hpp"

//#define __DEBUG_CRASH_ON_NULLPTR //Uncomment if you wish for exit on null pointer.

class GroceryList
{
private:
	//The size of the array.
	int _size;
	//The current index of the array (this is the first empty index).
	int _index;
	//An array of GroceryItem's.
	GroceryItem *_array;
	
public:
	//A constructor for the GroceryList
	GroceryList();
	//A deconstructor for the GroceryList
	~GroceryList();
	
public:
	//The length of the array.
	int getLength() const;
	//The number of empty slots in the array.
	int numEmptySlots() const;

public:
	//Checks off the item given by the name provided.
	void checkOff(const std::string&) const;
	
private:
	//Attempts to resize the array, returns False if this is not possible.
	bool resize();
	
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
