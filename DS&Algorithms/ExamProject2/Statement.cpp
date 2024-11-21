// !!!!!! You will have to add code inside of this class. Some of
//        the code provided here is designed to help you, but you
//         do not need to use it if you do not want to. 

#include "Statement.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;

int __nextSnum=0; // keeps track of the next statement number to use. 
int __paramValue; // this is a global to help with passing a parameter. 

/****
 **** If you are following project specifications, then you will likley
 **** find this function quite useful. When given an ActivationRecord
 **** (pointer) and a string containing an operand, this function returns
 **** the operand's value (as found in the ActivationRecord.)
 ****/
int getValue(ActivationRecord *ar, const std::string &operand)
{
  if (operand == "$$") // looking for the parameter
    return ar->parameterValue();
  else if (isdigit(operand[0]) || operand[0]=='-') // constant value (int)
    {
      return atoi(operand.c_str());
    }
  else // ID
    {
      // find the variable in the activation record
      Variable *var = (Variable *)  ar->getVariable(operand);

      // if the variable was not found, let the user know. 
      if (var==NULL)
	{
	  cerr << "Attempt to use an uninitialized variable: "
	       << operand << endl;
	  return 0;
	}
      else
	return var->value(); //return value of the associated variable 
    }
}

/****
 **** If you are following project specifications, then you will likley
 **** find this function quite useful. When given an ActivationRecord
 **** (pointer), a string containing an variable name, and a new value,
 **** this function sets that variable to the new value (in the associated
 **** ActivationRecord.) If the variable isn't found in the Activation
 ***  Record, it is added to the ActivationRecord.
 ****/
void setValue(ActivationRecord *ar, const std::string &varName, int toVal)
{
  if (varName == "$$") // parameter
    ar->parameterValue() = toVal;
  else // anything else must be a variable
    {
      // get the Variable from the ActivationRecord
      Variable *var = (Variable *)  ar->getVariable(varName);

      // no such variable? Means we need to add it to the ActivationRecord
      if (var==NULL)
	{
	  ar->addVariable(varName);
	  var = (Variable *)  ar->getVariable(varName);
	}

      // now that we know we have a variable, set its value ... 
      var->value()=toVal;
    }
}

//
// ********   YOU MUST CODE LARGE PARTS OF THIS !!!!   **********
//
// Executes a statement. See the project handout for details. Some
//  initial statements are given as examples.
//
//     withStack - The ActivationRecord (i.e. execution)  stack to use
//     ft - the table of function names & starting points
//
//  *should return the statement number (ddress) of the next statement to run
int
Statement::execute(Stack &withStack, const FunctionTable &ft) const
{
  // the next statement will be 1 after this one ... with the exception
  //  of calls, rets, and skips (see project handout.)
  int nextAddr = _snum+1; 

  if (_operation=="printS") // print string. 
    {
      cout << _operands.front();
    }
  else if (_operation=="printN") // print number
    {
      // get the value to print from the current activation record. 
      int valueOfN = getValue(withStack.top(), _operands.front());

      // print that value
      cout << valueOfN;
    }
  else if (_operation=="printNL") // drop down a line in output
    {
      cout << endl; // yep, it's that simple!
    }
  else if (_operation=="call")
    { // call "func_name"
      //Store our function name since thats our only operand.
      std::string func_name = _operands.front();
      //Create a new ActivationRecord, and push to the top.
      ActivationRecord* ar = new ActivationRecord(func_name, nextAddr, __paramValue);
      withStack.push(ar);
      //Set the nextAddr.
      nextAddr = ft.lookup(func_name).firstInstruction();
    }
  else if(_operation=="ret")
    {
      //Grab the old "top" record.
      ActivationRecord* ar = withStack.pop();

      //Goto the returned address.
      nextAddr = ar->returnAddress();

      //Cleanup after ourselves.
      delete ar;
    }
  else if(_operation=="retv")
    {
      //Set the return address of the next stack down, get the value from the variable.
      withStack.peek()->incomingReturnValue() = getValue(withStack.top(), _operands.front());

      //Basically the ret operation.
      ActivationRecord* ar = withStack.pop();

      //Goto the returned address.
      nextAddr = ar->returnAddress();

      //Delete the old record.
      delete ar;
    }
  else if(_operation=="storet")
    {
      //Store the incoming return value into the given variable.
      setValue(withStack.top(),_operands.front(), withStack.top()->incomingReturnValue());
    }
  else if(_operation=="param")
    {
      //Store the param value using the given variable name.
      __paramValue = getValue(withStack.top(),_operands.front());
    }
  else if(_operation=="sub")
    {
      //Obtain a, b, and c.
      auto iter = _operands.begin();
      int a = getValue(withStack.top(),*iter);
      std::advance(iter, 1);
      int b = getValue(withStack.top(),*iter);
      std::advance(iter, 1);

      //Set the value of the third operand.
      setValue(withStack.top(), *iter, a - b);
    }
  else if(_operation=="add")
    {
      //Obtain a, b, and c.
      auto iter = _operands.begin();
      int a = getValue(withStack.top(),*iter);
      std::advance(iter, 1);
      int b = getValue(withStack.top(),*iter);
      std::advance(iter, 1);

      //Set the value of the third operand.
      setValue(withStack.top(), *iter, a + b);
    }
  else if(_operation=="mul")
    {
      //Obtain a, b, and c.
      auto iter = _operands.begin();
      int a = getValue(withStack.top(),*iter);
      std::advance(iter, 1);
      int b = getValue(withStack.top(),*iter);
      std::advance(iter, 1);

      //Set the value of the third operand.
      setValue(withStack.top(), *iter, a * b);
    }
  else if(_operation=="div")
    {
      //Obtain a, b, and c.
      auto iter = _operands.begin();
      int a = getValue(withStack.top(),*iter);
      std::advance(iter, 1);
      int b = getValue(withStack.top(),*iter);
      std::advance(iter, 1);

      //Set the value of the third operand.
      setValue(withStack.top(), *iter, a / b);
    }
  else if(_operation=="skipz")
    {
      //Skip if the variable is 0.
      int flag = getValue(withStack.top(), _operands.front());
      if(flag == 0){
        //Advance the nextAddr the given number of positions.
        auto iter = _operands.begin();
        std::advance(iter, 1);
        nextAddr += getValue(withStack.top(), *iter);
      }
    }
  else if(_operation=="skipnz")
    {
      //Skip if the variable is not 0.
      int flag = getValue(withStack.top(), _operands.front());
      if(flag != 0){
        //Advance the nextAddr the given number of positions.
        auto iter = _operands.begin();
        std::advance(iter, 1);
        nextAddr += getValue(withStack.top(), *iter);
      }
    }
  else if(_operation=="read")
    {
      //Read in a value.
      int val;
      std::cin >> val;

      //Store the given variable into the variable.
      setValue(withStack.top(), _operands.front(), val);
    }
  else if(_operation=="dump")
    {
      //Dump the stack to cout.
      std::cout << withStack;
    }

  /////  HERE IS WHERE YOU NEED TO START ADDING CODE TO EXECUTE
  /////  THE REMAINING STATEMENTS DESCRIBED IN THE PROJECT HANDOUT

  else
    {
      cout << _operation << ": not implememnted" << endl;
    }
  return nextAddr;
}

// prints out this statement to the specified stream (overloads <<) 
ostream& operator<<(ostream &os, const Statement &s)
{
  // print the statement number in the specified width. 
  os.width(5);
  os << s._snum << " ";

  // print the operation
  os << s._operation << '\t';

  // print each operand .. 
  list<string>::const_iterator lsci = s._operands.begin();
  for (int i=0; i< s._operandCount; i++)
    {
      os <<  *lsci << " ";

      lsci++;
    }

  return os;
}
