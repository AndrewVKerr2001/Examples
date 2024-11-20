 #include <iostream>
 #include "complex.hpp"

int main(/*int argc, const char *argv[]*/){
	
	Complex zero;
	
	Complex myNum(1,2), myNum2(1,-2), mySum;
	
	mySum = myNum + myNum2;
	
	zero.print(std::cout);
	std::cout << std::endl;
	
	myNum.print(std::cout);
	std::cout << std::endl;
	
	myNum2.print(std::cout);
	std::cout << std::endl;
	
	double* real = (double*)((size_t)&myNum);
	double* imaginary = (double*)((size_t)&myNum)+1;
	std::cout << "Forced access to myNum: " << *real << (*imaginary < 0 ? "" : "+") << *imaginary << "i" << std::endl;
	
	mySum.print(std::cout);
	std::cout << std::endl;
	
	Complex multByValue;
	multByValue = myNum2 * 10.0;
	
	multByValue.print(std::cout);
	std::cout << std::endl;
	
	//multByValue = 10.0 * myNum2;
	
	multByValue.print(std::cout);
	std::cout << std::endl;
	
	std::cout << multByValue << std::endl;
	
	return 0;
}
