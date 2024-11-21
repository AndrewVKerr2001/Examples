// Your implementation of the Stack class goes here!!!!

#include "Stack.hpp"

#include <iostream>

using namespace std;

ActivationRecord* Stack::pop() {
	//Attain our top record if any.
	ActivationRecord* record = NULL;
	if(this->lpTop){
		//Found entry, lets cache it for later.
		record = this->lpTop->lpRecord;

		//Move the top and then delete the old "top" entry.
		ENTRY* entry = this->lpTop;
		this->lpTop = this->lpTop->lpPrev;
		delete entry;
	}
	//Return the cached record or NULL if none.
	return record;
}

// "push" a new element onto the (top of the) stack
void Stack::push(ActivationRecord *newAR) {
	//Cache out current "top", entries.
	ENTRY* entry = this->lpTop;

	//Make new entry and lets update it to match the new "top" entry.
	this->lpTop = new ENTRY();
	this->lpTop->lpPrev = entry;
	this->lpTop->lpRecord = newAR;
}

// return top element from stack *without* removing it. 
ActivationRecord* Stack::top() const {
	//Grab top record if we have one.
	if(this->lpTop)
		return this->lpTop->lpRecord;
	//No record, lets return NULL.
	return NULL;
}

// return next to top element from stack *without* removing any elements. 
ActivationRecord* Stack::peek() const {
	//Check if the top exists and has a record beneith it.
	if(this->lpTop && this->lpTop->lpPrev)
		return this->lpTop->lpPrev->lpRecord;
	//Return NULL cause we can't get the second to top record.
	return NULL;
}

// returns true if there are no elements in teh stack,
//         false if the stack contains at least one element.
bool Stack::isEmpty() const {
	//Check if our stack has something in it.
	return this->lpTop == NULL;
}

// print out the stack element, from top to bottom ... one ActivationRecord
//   at a time (don't print the pointer values.)
std::ostream& operator<<(std::ostream &os, const Stack &s){
	//Grab our top entry.
	Stack::ENTRY* cur = s.lpTop;

	//Loop while we have an entry.
	while(cur){
		//Check if we have a record, this shouldn't be necessary cause why entry without record. :^)
		if(cur->lpRecord)
			os << *cur->lpRecord << std::endl;
		//Move to next record.
		cur = cur->lpPrev;
	}

	//Return the provided stream for chain operators.
	return os;
}
