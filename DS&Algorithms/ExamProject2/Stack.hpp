/********************************************************
 * File:Stack.hpp                                       *
 * Author: (TBD)                                        *
 * Date: 1/2022                                         *
 * Purpose: Declaration for a Stack of ActivationRecord *
 *      *pointers*.                                     *
 ********************************************************/
#ifndef _STACK_HPP_
#define _STACK_HPP_

#include <iostream>
#include <list>

#include "ActivationRecord.hpp"

// This one is 100% up to you, other than the public interface
//   implementing what you see here!!!!
class Stack: private std::list<ActivationRecord*>
{
private:
  struct ENTRY {
    ActivationRecord* lpRecord = NULL;
    ENTRY* lpPrev = NULL;
  };
  ENTRY* lpTop = NULL;

public:
  // default constructor should give empty stack ...
  
  // remove *and* return top element in stack
  ActivationRecord* pop();

  // "push" a new element onto the (top of the) stack
  void push(ActivationRecord *newAR);

  // return top element from stack *without* removing it. 
  ActivationRecord* top() const;

  // return next to top element from stack *without* removing any elements. 
  ActivationRecord* peek() const;

  // returns true if there are no elements in teh stack,
  //         false if the stack contains at least one element.
  bool isEmpty() const;

  // print out the stack element, from top to bottom ... one ActivationRecord
  //   at a time (don't print the pointer values.)
  friend std::ostream& operator<<(std::ostream &os, const Stack &s);
};

#endif
