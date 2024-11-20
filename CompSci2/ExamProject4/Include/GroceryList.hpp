#include <iostream>
#include "GroceryItem.hpp"

#ifndef __GROCERYLIST_HPP_
#define __GROCERYLIST_HPP_

class Node {
private:
	Node* _parent;
	Node* _left;
	Node* _right;
	GroceryItem* _data;
	
public:
	Node();
	~Node();
	
public:
	bool isLeaf() const;
	int depth() const;
	
public:
	Node*& parent();
	Node*& left();
	Node*& right();
	GroceryItem*& data();
	
public:
	void print(std::ostream&, int tabs) const;
	friend std::ostream& operator<<(std::ostream&, const Node&);
	bool operator<(const GroceryItem&);
	bool operator==(const GroceryItem&);
	bool operator>(const GroceryItem&);
};

class GroceryList {
private:
	Node* _root;
	int _nodeCount;
	int _maxNodeDepth;
	
public:
	GroceryList();
	~GroceryList();
	
private:
	Node* TreeInsert(const GroceryItem&,Node*&);
	Node* TreeSearch(const std::string&,Node*);
	Node* TreeRemove(const std::string&);
	int TreeDepthCalc(Node*);
	void TreeCleanup(Node*);
	
public:
	bool insert(const GroceryItem&);
	bool contains(const std::string&);
	bool remove(const std::string&);
	void clear();
	
public:
	int maxSearchLength() const;
	int getLength() const;
	void checkOff(const std::string&);
	
public:
	GroceryList& operator=(const GroceryList&);
	GroceryList& operator+=(const GroceryList&);
	GroceryList& operator+=(const GroceryItem&);
	GroceryList& operator-=(const std::string&);
	GroceryItem* operator[](const std::string&);
	
public:
	friend std::ostream& operator<<(std::ostream&, const GroceryList&);
};

#endif//__GROCERYLIST_HPP_
