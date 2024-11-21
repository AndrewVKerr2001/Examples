/********************************************************
 * File:Function.hpp                                    *
 * Author: S. Blythe                                    *
 * Date: 1/2022                                         *
 * Purpose: Declaration for a Function. The primary     *
 *      use is to find the statement nnumber of the     *
 *      first statement in the function                 *
 ********************************************************/
#ifndef _FUNCTION_HPP_
#define _FUNCTION_HPP_

#include <iostream>
#include <list>
#include <string>

class Function
{
private:
  std::string _name;       // name of the function
  int _firstInstruction;   // statement number of the 1st instruction 
public:

  // constructors
  Function() : _name(), _firstInstruction(-1) {}
  Function(const std::string &name, int address=-1)
    : _name(name), _firstInstruction(address) {}

  //accesors
  const std::string &name() const {return _name;}
  int firstInstruction() const {return _firstInstruction;}

  // overloaded comparator operators
  bool operator==(const Function &other) const {return _name==other._name;}
  bool operator==(const std::string &name) const {return _name==name;}

  bool operator<(const Function &other) const {return _name<other._name;}
  bool operator<(const std::string &name) const {return _name<name;}

  // ovreload the output stream operator
  friend std::ostream& operator<< (std::ostream &os, const Function &f); 
};


#endif
