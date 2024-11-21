/********************************************************
 * File:Function.hpp                                    *
 * Author: S. Blythe                                    *
 * Date: 1/2022                                         *
 * Purpose: Implementation for a Function.              *
 ********************************************************/
#include "Function.hpp"

#include <sstream>

using namespace std;

// straightforward implememtation for overloaded opertor
ostream& operator<< (ostream &os, const Function &f)
{
  os << "Name: " << f.name()
     << ", Address: " << f.firstInstruction() << endl;

  return os;
}

