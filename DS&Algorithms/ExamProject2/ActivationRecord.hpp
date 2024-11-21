/********************************************************
 * File:ActivationRecord.hpp                            *
 * Author: S. Blythe                                    *
 * Date: 1/2022                                         *
 * Purpose: Declaration for an Activation Record, which *
 *      is the core structure for maintaining execution *
 *      data for any running program's function calls   *
 ********************************************************/

#ifndef _ACTIVATION_RECORD_HPP_
#define _ACTIVATION_RECORD_HPP_

#include <iostream>
#include <string>
#include <list>

#include "Variable.hpp"

class ActivationRecord
{
private:
  std::string _functionName;        // name of unerlying function
  int _returnAddress;               // number of statement to return to 
  int _param;                       // value of incoming parameter
  std::list<Variable*> _variables;  // list of all local variables (and values)
  int _incomingReturn;              // return value from other function 

public:
  // constructor that can optionally specify return address and parameter value
  ActivationRecord(std::string fname,
		   int returnAddress=-1,
		   int parameterValue=-1):
    _functionName(fname),
    _returnAddress(returnAddress),
    _param(parameterValue),
    _variables(), _incomingReturn(0) {}

  // accessors/modifiers for core concepts
  const std::string& functionName() const {return _functionName;}
  
  int returnAddress() const {return _returnAddress;}
  int& returnAddress() {return _returnAddress;}

  int parameterValue() const {return _param;}
  int& parameterValue() {return _param;}

  int incomingReturnValue() const {return _incomingReturn;}
  int& incomingReturnValue() {return _incomingReturn;}

  // add in a new variable to list of locals. returns true if the variable
  //   is a new variable name, false if it is a duplicate. 
  bool addVariable(const std::string &varName);

  // gets the variable structure for a given varibale name within this
  //   activation record. Returns NULL if the variable does not exist
  Variable* getVariable (const std::string &varName);

  // overload output stream operator
  friend std::ostream& operator<<(std::ostream &os, const ActivationRecord &ar);
};

#endif
