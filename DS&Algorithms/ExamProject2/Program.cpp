/********************************************************
 * File:Program.cpp                                     *
 * Author: S. Blythe                                    *
 * Date: 1/2022                                         *
 * Purpose: Implementation for a Program.               *
 ********************************************************/
#include "Program.hpp"

using namespace std;

// lookup a statement based on its address (statement number)
const Statement &
Program::lookup(int address) const
{
  // use STL's set search 
  Statement searchFor(address);
  set<Statement>::const_iterator ssci = find(searchFor);

  // if STL's set search did not work, indicate such ...
  if (ssci==end())
    cerr << "No statement with address=" << address << " was found" << endl;

  // return the found statement
  return *ssci;
}


std::ostream& operator<< (std::ostream &os, const Program &p)
{
  // go through every statement and print it out. 
  for (Statement s:p)
    {
      os << s << endl;
    }
  return os;
}
