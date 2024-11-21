/********************************************************
 * File:Statement.hpp                                   *
 * Author: S. Blythe & (TBD)                            *
 * Date: 1/2022                                         *
 * Purpose:  stores a single program statement          *
 ********************************************************/

#ifndef _STATEMENT_HPP_
#define _STATEMENT_HPP_

#include <iostream>
#include <list>
#include <string>

#include "FunctionTable.hpp"
#include "Stack.hpp"

extern int __nextSnum; // used to give each statement a unique line number. 

class Statement
{
private:
  int _snum;                         // this statement's number/address
  std::string _operation;            // operation/command name
  int _operandCount;                 // number of operands
  std::list <std::string> _operands; // list of the operands
public:
  // empty statement
  Statement() {}

  // build a statement from a statement number/address
  Statement(int addr): _snum(addr), _operation(), _operands() {}

  // build a statement from an operation and operand list
  Statement(std::string op, std::list<std::string> ops)
    : _snum(__nextSnum++),
      _operation(op),
      _operandCount(ops.size()), _operands(ops) {}

  // get the statement number/address for this statement
  int number() const {return _snum;}

  // get the operation/command from this statement
  const std::string& operation() const {return _operation;}

  // How many operands does this statemment have?
  int operandCount() const {return _operandCount;}

  // Wat are this statements operands?
  const std::list<std::string>& operands() const {return _operands;}

  // actually perform what this statement is supposed to do,
  //   using the given activation record stack and
  //   function table (needed for call statements)
  int execute(Stack &withStack, const FunctionTable &ft) const;

  // used internally to keep statements sorted by number/address
  bool operator<(const Statement &other) const {return _snum < other._snum;}
  
  // overload the output stream operator
  friend std::ostream& operator<<(std::ostream &os, const Statement &s);
};

#endif
