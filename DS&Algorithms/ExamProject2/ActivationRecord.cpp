/********************************************************
 * File:ActivationRecord.cpp                            *
 * Author: S. Blythe                                    *
 * Date: 1/2022                                         *
 * Purpose: Implementation for an Activation Record     *
 ********************************************************/
#include "ActivationRecord.hpp"

using namespace std;


bool
ActivationRecord::addVariable(const string &varName)
{
  // go through all local variables looking for the one specified ...
  for(Variable *vptr : _variables)
    {
      // ... if you can find it, there's no need to add it. 
      if (vptr->name() == varName)
	{
	  cerr << "Warning: attempt to add previously existing local variable ("
	    << varName << ") ignored." << endl;
	  return false;
	}
    }

  // if we get here, the variable name didn't exist. Just add it.
  Variable *vptr = new Variable(varName);
  _variables.push_back(vptr);
  return true;
}


Variable*
ActivationRecord::getVariable (const std::string &varName)
{
  //look for the request variable by traversing the list of such 
  for(Variable *vptr : _variables)
    {
      // if found, return the variable. 
      if (vptr->name() == varName)
	return vptr;
    }

  // can't find it? just add it ...
  addVariable(varName);

  // ... and then return the associated varuiable
  return getVariable(varName);  
}

ostream& operator<<(std::ostream &os, const ActivationRecord &ar)
{
  // straightforward ... just print out each component
  os << ar._functionName <<endl;
  
  os << "Return Address:" << ar._returnAddress << endl;
  
  os << "Parameter Value:" << ar._param;
  if (ar._param == -1)
    os << " (possibly the default value)";
  os << endl;
  
  os << "Local Variables:";

  // local variables are a list ... so we traverse the list and print each one
  bool first=true;
  for (Variable *v: ar._variables)
    {
      if (!first)
	os << ", ";
      os << *v;
      first=false;
    }
  os << endl;

  os << "Incoming Return:" << ar._incomingReturn;
  if (ar._incomingReturn == -1)
    os << " (possibly the default value)";

  return os;
}

