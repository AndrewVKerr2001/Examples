/********************************************************
 * File:Program.hpp                                     *
 * Author: S. Blythe                                    *
 * Date: 1/2022                                         *
 * Purpose: Declaration for a Program. The primary      *
 *      use is to find the statement nnumber of the     *
 *      first statement in the function                 *
 ********************************************************/
#ifndef _PROGRAM_HPP_
#define _PROGRAM_HPP_

#include <set>

#include "Statement.hpp"

class Program: public std::set<Statement>
{
public:
  // default constructor available from inheriting STL's set

  // many other methods also available due to STL's set being inherited:
  //   * insert  - use in main to add a Statement to this Program

  // given an address (statement number), find the corresponding statement
  const Statement &lookup(int address) const;

  // overload output stream operator for a Program
  friend std::ostream& operator<< (std::ostream &os, const Program &p);
};

#endif
