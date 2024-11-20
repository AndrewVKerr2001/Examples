#include <iostream>

#include "PartiallyFilledArray.hpp"

int main(int argc, char *argv[]){
	
	std::string str1 = "Hello";
	std::string str2 = "World";
	std::string str3 = "Goodbye";
	
	PartiallyFilledArray words;

	words.add(str1);
	words.add(str2);
	
	std::cout << words << std::endl;
	
	std::cout << "Size: " << words.size() << ", Count: " << words.count() << std::endl;
	
	words.resize(words.count());
	
	std::cout << "Size: " << words.size() << ", Count: " << words.count() << std::endl;
	
	words.add(str3);
	
	std::cout << "Did add \"" << str3 << "\"?" << std::endl << std::endl;
	
	std::cout << words << std::endl;
	
	std::cout << "Size: " << words.size() << ", Count: " << words.count() << std::endl;
	
	return 0;
}
