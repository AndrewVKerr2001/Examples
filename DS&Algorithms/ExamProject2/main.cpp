/********************************************************
 * File:main.cpp                                        *
 * Author: S. Blythe                                    *
 * Date: 1/2022                                         *
 * Purpose: core of project2. It's all provided for     *
 *    you, so you should not need to mdify this file.   *
 ********************************************************/
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>

#include "FunctionTable.hpp"
#include "Program.hpp"
#include "Stack.hpp"
#include "Statement.hpp"

using namespace std;


int main(int argc, char *argv[])
{
  Stack executionStack;  // the stack of Actvation Records

  ifstream ifile(argv[1]); // input file from command line

  FunctionTable functionInfo; // table of Function startign statement numbers
  
  Program prog; // the "code" we are going to execute

  string opName;  // the operator name for the next statement ...
  ifile >>opName; // read in first statement

  // as long as there are more statements to add to the code ...
  while(ifile)
    {
      // in this case, the "statement" is not code, but rather indiciates that
      //   a new function begins here. 
      if (opName == "func")
	{
	  // get the new function's name
	  string name;
	  ifile >> name;

	  // build a Function object with the next statement number
	  Function newFunc(name,  __nextSnum);

	  // add the function object to the Function Table. 
	  functionInfo.add(newFunc);
	}
      else // this is a real statement ...
	{
	  // skip any whitespace before the operands start
	  char ch = ifile.get();
	  while(ch==' ' || ch=='\t')
	    ch = ifile.get();
	  ifile.unget();


	  // stores all of  the operands for the statement.
	  string operandString;
	  // read in all of the operands. 
	  getline(ifile, operandString);
	  
	  // build a stringstream object to help extract the individual
	  //   operands from the string containing all of them.
	  //   Fortunately, the operands are comma separated with no spacing. 
	  stringstream strStream(operandString);

	  list<string> operands; //when done, this will be the list of operands.

	  // pick the next operand by reading up to the next ',' or newline
	  string nextOperand;
	  getline(strStream, nextOperand, ',');

	  // as long as the last read worked ...
	  while(strStream)
	    {
	      // we've got one more operand ...
	      operands.push_back(nextOperand);

	      // (try to) get the next operand. 
	      getline(strStream, nextOperand, ',');
	    }

	  // build a statement out of the operation code and operand list. 
	  Statement stmt(opName, operands);

	  // add the new statement to our code. 
	  prog.insert(stmt);
	}
      
      ifile >> opName;
    }

  // find the main function's first statement number
  Function starter = functionInfo.lookup("main");

  // get the statement associated with that statement number
  Statement s = prog.lookup(starter.firstInstruction());

  // we need a new ActivationRecord for the invocation of the main function
  ActivationRecord *mainAR = new ActivationRecord("main", -1, 0);

  // put main's ActivationRecord on the execution stack. 
  executionStack.push(mainAR);

  // as long as the statement wasn't "exit" ...
  while (s.operation()!="exit")
    {
      // execute the statement and figuer out which statement number comes next
      int nextAddr = s.execute(executionStack, functionInfo);

      // use the statement number we just got to find the next statement. 
      s = prog.lookup(nextAddr);
    }
  
  return 0;
}
