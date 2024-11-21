/********************************************************
 * File:FunctionTable.hpp                               *
 * Author: S. Blythe                                    *
 * Date: 1/2022                                         *
 * Purpose: Implementation for a FunctionTable.         *
 ********************************************************/
#include "FunctionTable.hpp"

using namespace std;

// finds a function object when given a function name.
const Function&
FunctionTable::lookup(const string name) const
{
  Function dummy(name);

  return *(_table.find(dummy));  // jsut use set's lookup
}

ostream& operator<<(std::ostream &os, const FunctionTable &ft)
{
  // go through each function in the table and print it out. 
  for (Function f:ft._table)
    os << f << endl;
  return os;
}
  
