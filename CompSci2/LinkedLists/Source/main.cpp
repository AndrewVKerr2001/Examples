#include <iostream>

#define __SINGLE__ 1
#define __DOUBLE__ 2
#define __EXAMPLE__ __SINGLE__

//Allow for seperate importing depending on which one we want to use.
#if __EXAMPLE__ == __SINGLE__
#include "SingleLinkedList.hpp"
#elif __EXAMPLE__ == __DOUBLE__
#include "DoubleLinkedList.hpp"
#else
#error __EXAMPLE__ macro must be set to either __SINGLE__ or __DOUBLE__!
#endif

int main(/*int argc, const char *argv[]*/){
	#if __EXAMPLE__ == __SINGLE__
	SingleLinkedList list;
	std::cout << "Using SingleLinkedList format..." << std::endl << std::endl;
	#elif __EXAMPLE__ == __DOUBLE__
	DoubleLinkedList list;
	std::cout << "Using DoubleLinkedList format..." << std::endl << std::endl;
	#endif
	
	list.append("Hello World");
	
	std::cout << list;
	std::cout << "=====================================" << std::endl << std::endl;
	
	list.append("Goodbye World");
	list.append("Testing");
	
	std::cout << list;
	std::cout << "=====================================" << std::endl << std::endl;
	
	list.prepend("Am I in front?");
	
	std::cout << list;
	std::cout << "=====================================" << std::endl << std::endl;
	
	list.remove("Hello World");
	list.remove("Am I in front?");
	
	std::cout << list;
	std::cout << "=====================================" << std::endl << std::endl;
	
	list.clear();
	
	std::cout << list;
	std::cout << "=====================================" << std::endl << std::endl;
	
	list.append("apple");
	list.append("orange");
	list.append("pear");
	list.append("mango");
	
	std::cout << list;
	std::cout << "=====================================" << std::endl << std::endl;
	
	std::string* data;
	
	data = list.find("apple");
	std::cout << data << ": " << (data ? *data : "NULLPTR") << std::endl;
	data = list.find("bannana");
	std::cout << data << ": " << (data ? *data : "NULLPTR") << std::endl;
	
	return 0;
}
