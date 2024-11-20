#include <iostream>
#include "BinarySearchTree.hpp"

int main(/*int argc, const char *argv[]*/){
	
	BinarySearchTree tree;
	tree.insert(2);
	tree.insert(1);
	tree.insert(3);
	tree.insert(0);
	tree.insert(3);
	tree.insert(8);
	tree.insert(6);
	tree.insert(5);
	tree.insert(9);
	tree.insert(-1);
	tree.insert(30);
	tree.insert(21);
	tree.insert(10);
	tree.insert(15);
	tree.insert(14);
	tree.insert(100);
	std::cout << tree;
	
	std::cout << "=====================================" << std::endl;
	
	//tree.remove(3);
	tree.remove(2);
	tree.remove(1);
	tree.remove(6);
	
	std::cout << tree;
	
	return 0;
}
