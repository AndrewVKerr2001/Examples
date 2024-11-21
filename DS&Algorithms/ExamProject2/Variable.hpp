/********************************************************
 * File:Variable.hpp                                    *
 * Author: S. Blythe                                    *
 * Date: 1/2022                                         *
 * Purpose: Declaration for a Variable. A Variabe is a  *
 *      (string, value) pair.                           *
 ********************************************************/
#ifndef _VARIABLE_HPP_
#define _VARIABLE_HPP_

#include <iostream>
#include <string>

class Variable
{
private:
  std::string _name; // name of the variable
  int _value;        // variable's value
public:
  // constructor. The default vaue of a variable is 0. 
  Variable(const std::string &name): _name(name), _value(0) {}

  //accessors
  const std::string & name() const {return _name;}
  int value() const {return _value;}

  // modifier (makes no sense to modify this variable's name. 
  int& value() {return _value;}

  // overload output operator for a Variable. 
  friend std::ostream& operator<<(std::ostream &os, const Variable &v);
};

#endif
