/********************************************************
 * File:Variable.cpp                                    *
 * Author: S. Blythe                                    *
 * Date: 1/2022                                         *
 * Purpose: Implementation for a Variable.              *
 ********************************************************/
#include "Variable.hpp"

using namespace std;

// note: most implementation is trivial and found in Variable.hpp

ostream& operator<<(ostream &os, const Variable &v)
{
  // print the variable's name and its contents. Nothing tricky. 
  os << v._name << ":" << v._value;
  return os;
}
