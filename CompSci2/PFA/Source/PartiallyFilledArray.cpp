#include "PartiallyFilledArray.hpp"
#include <iostream>

//PartiallyFilledArray::[CONSTRUCTOR/DECONSTRUCTOR]

PartiallyFilledArray::PartiallyFilledArray() {
	this->_index = 0;
	this->_size = 5;
	this->_array = new std::string[this->_size];
	if(!this->_array)
		this->_size = 0;
}

PartiallyFilledArray::PartiallyFilledArray(int size) {
	this->_index = 0;
	this->_size = size;
	this->_array = new std::string[size];
	if(!this->_array)
		this->_size = 0;
}

PartiallyFilledArray::~PartiallyFilledArray() {
	if(this->_array) delete[] this->_array;
	this->_array = 0;
}

//PartiallyFilledArray::[SETTERS]

void PartiallyFilledArray::add(const std::string &newElm) {
	if(this->_index == this->_size) {
		if(!this->resize(this->_size+1))
			return;
	}
	this->_array[this->_index] = newElm;
	this->_index++;
}

bool PartiallyFilledArray::resize(int size) {
	if(size < this->_index) return false; //Shrinking past existing elements.
	
	std::string *temp = new std::string[size];
	if(!temp) return false;
	
	if(this->_array) { //Copy over elements if we got them.
		for(int i = 0; i < this->_index; i++){
			temp[i] = this->_array[i];
		}
		delete[] this->_array; //Delete the array.
	}
	
	this->_array = temp;
	this->_size = size;
	
	return true;
}

//PartiallyFilledArray::[GETTERS]

int PartiallyFilledArray::size() const { return _size; }
int PartiallyFilledArray::count() const { return _index; }

//PartiallyFilledArray::[FRIENDS]

std::ostream& operator<<(std::ostream& ostr, const PartiallyFilledArray& pfa){
	for(int i = 0; i < pfa._index; i++){
		ostr << pfa._array[i] << std::endl;
	}
	return ostr;
}
