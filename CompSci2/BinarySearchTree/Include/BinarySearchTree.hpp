#include <iostream>
#include <string>

#ifndef __BINARY_SEARCH_TREE_HPP_
#define __BINARY_SEARCH_TREE_HPP_

typedef unsigned int BST_TYPE;

class BinarySearchTree {
public:
	class Node {
	private:
		Node* _parent;
		Node* _left;
		Node* _right;
		BST_TYPE* _data;
		
	public:
		Node();
		~Node();
		
	public:
		//bool isLeaf();
		
	public:
		Node*& parent();
		Node*& left();
		Node*& right();
		BST_TYPE*& data();
		
	public:
		void print(std::ostream&, int tabs) const;
		friend std::ostream& operator<<(std::ostream&, const Node&);
	};
	
private:
	Node* _root;
	
public:
	BinarySearchTree();
	~BinarySearchTree();
	
private:
	Node* TreeInsert(const BST_TYPE&,Node*&);
	Node* TreeSearch(const BST_TYPE&,Node*);
	bool TreeRemove(const BST_TYPE&,Node*&);
	void TreeCleanup(Node*);
	
public:
	bool insert(const BST_TYPE&);
	bool contains(const BST_TYPE&);
	bool remove(const BST_TYPE&);
	void clear();
	
public:
	friend std::ostream& operator<<(std::ostream&, const BinarySearchTree&);
};

#endif//__BINARY_SEARCH_TREE_HPP_
