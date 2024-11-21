/********************************************************
 * File:FunctionTable.hpp                               *
 * Author: S. Blythe                                    *
 * Date: 1/2022                                         *
 * Purpose: Declaration for a FunctionTable. The        *
 *      prinary use is to keep track of all functions   *
 *      ina program.                                    *
 ********************************************************/
#ifndef _FUNCTION_TABLE_HPP_
#define _FUNCTION_TABLE_HPP_

#include <iostream>
#include <set>
#include <string>

#include "Function.hpp"

class FunctionTable
{
private:
  std::set<Function> _table; // use STL set to keep a collection of functions
public:

  // given a function name, find full function info ...
  const Function& lookup(const std::string name) const;

  // add a function to our collection 
  void add(const Function &f) {_table.insert(f);}

  // overload output stream for a FunctionTable. 
  friend std::ostream& operator<<(std::ostream &os, const FunctionTable &ft);
};

#endif
