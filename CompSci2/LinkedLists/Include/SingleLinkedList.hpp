#ifndef __SINGLELINKEDLIST_HPP_
#define __SINGLELINKEDLIST_HPP_

#include <string>
#include <iostream>

class SingleLinkedNode {
private:
	/**
	 * The data to store, this type simply needs to be changed to work with any and all data.
	 */
	std::string* _data;
	/**
	 * The next node in the sequence.
	 */
	SingleLinkedNode* _next;
	
public:
	/**
	 * Constructs a new blank node.
	 */
	SingleLinkedNode();
	
	/**
	 * Deconstructs the node, freeing up the data.
	 */
	~SingleLinkedNode();
	
public:
	
	/**
	 * Returns a reference to this nodes data pointer.
	 */
	std::string*& data() { return _data; };
	
	/**
	 * Returns a reference to this nodes next node pointer.
	 */
	SingleLinkedNode*& next() { return _next; };
};

class SingleLinkedList {
private:
	/**
	 * The node that starts off the list.
	 */
	SingleLinkedNode* _head;
	/**
	 * The node that ends off the list.
	 */
	SingleLinkedNode* _tail;
	/**
	 * A cached value representing the number of linked nodes, used to speed up counting nodes.
	 */
	int _nodeCount;
	
public:
	/**
	 * Constructs an empty SingleLinkedList.
	 */
	SingleLinkedList();
	
public:
	/**
	 * Returns the number of nodes in the list.
	 */
	int nodeCount() const { return _nodeCount; };

public:
	/**
	 * Appends a value to the end of the list.
	 * parameters:
	 *  - const std::string& - The value to append.
	 */
	void append(const std::string&);
	/**
	 * Prepend a value to the start of the list.
	 * parameters:
	 *  - const std::string& - The value to prepend.
	 */
	void prepend(const std::string&);
	/**
	 * Removes a value from list.
	 * parameters:
	 *  - const std::string& - The value to remove.
	 */
	void remove(const std::string&);
	/**
	 * Clears the list of any and all values.
	 */
	void clear();
	
public:
	/**
	 * Searches for a value.
	 * parameters:
	 *  - const std::string& - The value to search for.
	 */
	std::string* find(const std::string&) const;
	
public:
	friend std::ostream& operator<<(std::ostream&,const SingleLinkedList&);
};

#endif//__SINGLELINKEDLIST_HPP_
