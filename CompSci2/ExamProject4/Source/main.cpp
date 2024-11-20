#include <iostream>
#include "GroceryList.hpp"

int main(int argc, char *argv[]){
	GroceryList list;
	
	{
		GroceryItem item("Hello",2);
		list.insert(item);
	}
	{
		GroceryItem item("World",4);
		list.insert(item);
	}
	{
		GroceryItem item("This",3);
		list.insert(item);
	}
	{
		GroceryItem item("is",8);
		list.insert(item);
	}
	{
		GroceryItem item("A",12);
		list.insert(item);
	}
	{
		GroceryItem item("test",10);
		list.insert(item);
	}
	
	GroceryList saved = list;

	std::cout << list;
	std::cout << "========================================" << std::endl << "Searching for keywords: (expected result in parenthesis)" << std::endl;
	
	{
		std::cout << "\ttest: " << list.contains("test") << "(true)" << std::endl;
	}
	{
		std::cout << "\tThis: " << list.contains("This") << "(true)" << std::endl;
	}
	{
		std::cout << "\tworld: " << list.contains("world") << "(false)" << std::endl;
	}
	std::cout << "========================================" << std::endl;
	std::cout << list;
	std::cout << "========================================" << std::endl << "Removing keywords:" << std::endl;
	{
		std::cout << "\ttest: " << list.remove("test") << "(true)" << std::endl;
	}
	{
		std::cout << "\tThis: " << list.remove("This") << "(true)" << std::endl;
	}
	{
		std::cout << "\tWorld: " << list.remove("World") << "(true)" << std::endl;
	}
	std::cout << "========================================" << std::endl;
	std::cout << list;
	std::cout << "========================================" << std::endl << "Searching for removed keywords: (expecting false)" << std::endl;
	
	{
		std::cout << "\ttest: " << list.contains("test") << std::endl;
	}
	{
		std::cout << "\tThis: " << list.contains("This") << std::endl;
	}
	{
		std::cout << "\tworld: " << list.contains("world") << std::endl;
	}
	
	std::cout << "\tCount: " << list.getLength() << std::endl;
	std::cout << "\tSearch: " << list.maxSearchLength() << std::endl;
	
	return 0;
}
