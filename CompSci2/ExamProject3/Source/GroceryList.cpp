//Author: Andrew Kerr
//Date: Nov 3 2021

//==[ Include('s) ]=======================================================================================================================================================

#include "GroceryList.hpp"

//==[ Constructor('s) ]===================================================================================================================================================

GroceryList::GroceryList() {
	//Init our variables to a default value.
	_head = NULL;
	_tail = NULL;
	_length = 0;
}

//==[ Deconstructor ]=====================================================================================================================================================

GroceryList::~GroceryList() {
	this->clear();

	//Default our values to null so we "cleanup" our data (ie. keep data from being garbage values later).
	_head = NULL;
	_tail = NULL;
	_length = 0;
}

//==[ Accessors ]=========================================================================================================================================================

const GroceryItem* GroceryNode::item() const { return _item; }
GroceryItem*& GroceryNode::item() { return _item; }

const GroceryNode* GroceryNode::next() const { return _next; }
GroceryNode*& GroceryNode::next() { return _next; }

int GroceryList::getLength() const { 
	return _length;
}

//==[ Modifiers ]=========================================================================================================================================================

void GroceryList::checkOff(const std::string& name) const {
	//Search for our item.
	GroceryItem* item = this->operator[](name);
	if(item){ //If found, set bought to true.
		item->bought() = true;
	}
}

void GroceryList::clear() {
	GroceryNode* node = _head;
	GroceryNode* next;
	
	_head = NULL;
	_tail = NULL;
	_length = 0;
	
	while(node){
		next = node->next();
		delete node->item();
		delete node;
		node = next;
	}
}

//==[ Operators ]=========================================================================================================================================================

//Sets this list equal to the given list.
GroceryList& GroceryList::operator=(const GroceryList& o) {
	this->clear();

	const GroceryNode* node = o._head;
	while(node){
		this->operator+=(*node->item());
		node = node->next();
	}
	return *this;
}

//Appends the given list to the end of this list (or merges items if already exists).
GroceryList& GroceryList::operator+=(const GroceryList& o) {
	const GroceryNode* node = o._head;
	while(node){
		this->operator+=(*node->item());
		node = node->next();
	}
	return *this;
}

//Appends/Merges the given item to the end of the list or into the existing entry if one exists.
GroceryList& GroceryList::operator+=(const GroceryItem& o) {
	GroceryItem* old = this->operator[](o.name());
	if(old){
		old->quantity() += o.quantity();
	} else {
		GroceryNode* node = new GroceryNode();
		node->item() = new GroceryItem(o);
		node->next() = NULL;
		
		if(!_head) {
			_head = node;
			_tail = node;
			_length = 1;
		} else {
			_tail->next() = node;
			_tail = node;
			_length++;
		}
	}
	return *this;
}

//Removes the given item (by name) from the list.
GroceryList& GroceryList::operator-=(const std::string& name) {
	//Loop through each item and find it one with the name given.
	GroceryNode* prv = NULL;
	GroceryNode* cur = _head;
	while(cur) {
		if(cur->item()->name() == name){
		
			if(prv)//We had a previous item, remove this element
				prv->next() = cur->next();
			else //We had no previous item, update our head.
				_head = cur->next();
				
			//Check explicitely for if we match tail and update accordingly.
			if(_tail == cur) {
				_tail = cur->next();
				if(_tail == NULL)
					_tail = prv;
			}
			
			delete cur->item();
			delete cur;
			
			//Done.
			break;
		}
		prv = cur;
		cur = cur->next();
	}
	//Return this converted to a reference.
	return *this;
}

//Attempts to get the given item (by name) from the list.
GroceryItem* GroceryList::operator[](const std::string& name) const {
	//Loop through each item.
	GroceryNode* cur = _head;
	while(cur) {
		if(cur->item()->name() == name) //Check if the name is identical.
			return cur->item(); //Return a pointer to the item.
		cur = cur->next();
	}
	return NULL; // Return null if item was not found.
}

//Print's the given list's items to the ostream.
std::ostream& operator<<(std::ostream& os, const GroceryList& list) {
	//Loop through and print out each item.
	GroceryNode* cur = list._head;
	while(cur) {
		os << *cur->item() << std::endl;
		cur = cur->next();
	}
	//Return os to continue operator chaining.
	return os;
}
