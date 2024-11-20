//Author: Andrew Kerr
//Date: Nov 3 2021

#include <iostream>
#include <fstream>
#include <string>
#include "GroceryItem.hpp"
#include "GroceryList.hpp"

int main(int argc, char *argv[]) {

	GroceryList list;
	GroceryItem item1("Carrot",2);
	GroceryItem item2("Apple",5);
	GroceryItem item3("Pear",3);
	
	list += item1;
	
	std::cout << list;
	std::cout << "==================================" << std::endl;
	
	list += item2;
	
	std::cout << list;
	std::cout << "==================================" << std::endl;
	
	GroceryList list2;
	
	std::cout << list2;
	std::cout << "==================================" << std::endl;
	
	list2 += item1;
	list2 += list;
	
	std::cout << list2;
	std::cout << "==================================" << std::endl;
	
	list2 -= "Apple";
	
	std::cout << list2;
	std::cout << "==================================" << std::endl;
	
	list2 = list;
	
	std::cout << list2;
	std::cout << "==================================" << std::endl;
	
	list2 += item3;
	
	std::cout << list2;
	std::cout << "==================================" << std::endl;
	
	list2.checkOff("Apple");
	
	std::cout << list2;
	std::cout << "==================================" << std::endl;
	
	std::cout << list.getLength() << ", " << list2.getLength() << std::endl;
	
	return 0;		
}
