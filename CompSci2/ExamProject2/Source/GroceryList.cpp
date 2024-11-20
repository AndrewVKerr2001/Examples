//Author: Andrew Kerr
//Date: Oct 18 2021

//==[ Include('s) ]=======================================================================================================================================================

#ifdef __DEBUG_CRASH_ON_NULLPTR //This include is only meant for the debug crashes, ie. exit(int).
#include <stdlib.h> //Provides exit function.
#endif

#include "GroceryList.hpp" //Provides GroceryList class definition.

//==[ Constructor('s) ]===================================================================================================================================================

GroceryList::GroceryList() {
	//Init our variables to a default value.
	_index = 0;
	_size = 5;
	_array = new GroceryItem[5];

	//Check if array is null.
	if(!_array) {
		std::cerr << "ERROR: NULLPTR in Constructor" << std::endl;
#ifdef __DEBUG_CRASH_ON_NULLPTR //IF debug crash then exit
		exit(139); //139 = Seg Fault
#endif
	}
}

//==[ Deconstructor ]=====================================================================================================================================================

GroceryList::~GroceryList() {
	//Delete array if we got one (I feel bad if I dont do this, :P)
	if(_array) delete[] _array;
	
	//Default our values to null so we "cleanup" our data (ie. keep data from being garbage values later).
	_array = 0;
	_size = 0;
	_index = 0;
}

//==[ Accessors ]=========================================================================================================================================================

int GroceryList::getLength() const { 
	return _index; //Index = Allocated Slots.
}

int GroceryList::numEmptySlots() const {
	return _size - _index; //Size minus Index = Empty Slots.
}

//==[ Modifiers ]=========================================================================================================================================================

void GroceryList::checkOff(const std::string& name) const {
	//Search for our item.
	GroceryItem* item = this->operator[](name);
	if(item){ //If found, set bought to true.
		item->bought() = true;
	}
}

bool GroceryList::resize() {
	if(_size > _index) return true; //Still got space, we don't need to resize.
	
	//Attempt to allocate a new array.
	GroceryItem *temp = new GroceryItem[_size * 2];
	if(!temp) return false; //If array was null then bail.
	_size = _size * 2; //Double our size on success.
	
	if(_array){ //If we had an array lets copy it over.
		for(int i = 0; i < _index; i++) {
			temp[i] = _array[i];
		}
		delete[] _array; //Delete the array (again feel bad not doing this).
	}
	//Set our array to the temporary array.
	_array = temp;
	
	return true; //We resized, yay.
}

//==[ Operators ]=========================================================================================================================================================

//Sets this list equal to the given list.
GroceryList& GroceryList::operator=(const GroceryList& o) {
	if(_array) delete[] _array; //Destroy our array if we got one.
	
	//Overwrite our parameters.
	_size = o._size;
	_index = o._index;
	
	//Attempt to allocate a new array/
	_array = new GroceryItem[_size];

	if(!_array) { //We failed?
		std::cerr << "ERROR: NULL pointer in operator=" << std::endl;
#ifdef __DEBUG_CRASH_ON_NULLPTR //Seg Fault exit
		exit(139);
#endif
		//If we failed and are not exiting, lets default our values to prevent a Seg Fault, but at some point this will fail anyways.
		_size = 0;
		_index = 0;
	}
	
	//Copy over our array information.
	for(int i = 0; i < _index; i++) {
		_array[i] = o._array[i];
	}
	
	//Return this converted to a reference.
	return *this;
}

//Appends the given list to the end of this list (or merges items if already exists).
GroceryList& GroceryList::operator+=(const GroceryList& o) {
	//Loop through each item and add it to our list.
	for(int i = 0; i < o._index; i++){
		this->operator+=(o._array[i]);
	}
	//Return this converted to a reference.
	return *this;
}

//Appends/Merges the given item to the end of the list or into the existing entry if one exists.
GroceryList& GroceryList::operator+=(const GroceryItem& o) {
	//Look for an existing entry.
	GroceryItem* entry = this->operator[](o.name());
	if(entry) { //Found one, add to quantity.
		entry->quantity() += o.quantity();
	} else {
		//Didn't find one, lets resize if needed then append to the array.
		if(!this->resize()) {
			std::cerr << "ERROR: Unable to resize the array." << std::endl;
#ifdef __DEBUG_CRASH_ON_NULLPTR
			exit(139);
#endif
		}
		//Append item to array.
		this->_array[_index++] = o;
	}
	//Return this converted to a reference.
	return *this;
}

//Removes the given item (by name) from the list.
GroceryList& GroceryList::operator-=(const std::string& name) {
	//Loop through each item and find it one with the name given.
	for(int i = 0; i < _index; i++) {
		if(_array[i].name() == name) { //Did we find our exact item with the name given.
			_index--; //Decrement index, were losing one item.
			for(int j = i; j < _index; j++) { //Loop through the remaining items and shift them down by one.
				_array[j] = _array[j+1];
			}
			break; //Break out cause were done.
		}
	}
	//Return this converted to a reference.
	return *this;
}

//Attempts to get the given item (by name) from the list.
GroceryItem* GroceryList::operator[](const std::string& name) const {
	//Loop through each item.
	for(int i = 0; i < _index; i++) {
		if(_array[i].name() == name) //Check if the name is identical.
			return &_array[i]; //Return a pointer to the item.
	}
	return 0; // Return null if item was not found.
}

//Print's the given list's items to the ostream.
std::ostream& operator<<(std::ostream& os, const GroceryList& list) {
	//Loop through and print out each item.
	for(int i = 0; i < list._index; i++) {
		os << list._array[i] << std::endl;
	}
	//Return os to continue operator chaining.
	return os;
}
