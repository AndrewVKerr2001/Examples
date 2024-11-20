#include "DoubleLinkedList.hpp"

DoubleLinkedNode::DoubleLinkedNode() {
	this->_next = NULL;
	this->_prev = NULL;
	this->_data = NULL;
}

DoubleLinkedNode::~DoubleLinkedNode() {
	if(this->_data) delete this->_data;
	this->_next = NULL;
	this->_prev = NULL;
	this->_data = NULL;
}

DoubleLinkedList::DoubleLinkedList() {
	this->_head = NULL;
	this->_tail = NULL;
	this->_nodeCount = 0;
}

void DoubleLinkedList::append(const std::string& val) {
	DoubleLinkedNode* node = new DoubleLinkedNode();
	node->data() = new std::string(val);
	
	if(_tail){
		node->prev() = _tail;
		_tail->next() = node;
		_tail = node;
	} else {
		_head = node;
		_tail = node;
	}
	
	_nodeCount++;
}

void DoubleLinkedList::prepend(const std::string& val) {
	DoubleLinkedNode* node = new DoubleLinkedNode();
	node->data() = new std::string(val);
	
	if(_head){
		node->next() = _head;
		_head->prev() = node;
		_head = node;
	} else {
		_head = node;
		_tail = node;
	}
	
	_nodeCount++;
}

void DoubleLinkedList::remove(const std::string& val) {
	DoubleLinkedNode* node = _head;
	while(node){
		if(node->data() && *node->data() == val){
			if(node->prev())
				node->prev()->next() = node->next();
			else
				_head = node->next();
			if(node == _tail)
				_tail = node->prev();
				
			delete node->data();
			delete node;
			
			_nodeCount--;
			break;
		}
		node = node->next();
	}
}

void DoubleLinkedList::clear() {
	_head = NULL;
	_tail = NULL;
	_nodeCount = 0;
}

std::string* DoubleLinkedList::find(const std::string& val) const {
	DoubleLinkedNode* node = _head;
	while(node){
		if(node->data() && *node->data() == val){
			return node->data();
		}
		node = node->next();
	}
	return NULL;
}

std::ostream& operator<<(std::ostream& os, const DoubleLinkedList& list){
	//os << "Length: " << list._nodeCount << std::endl;
	DoubleLinkedNode* cur = list._head;
	while(cur){
		if(cur->data())
			os << *cur->data() << std::endl;
		cur = cur->next();
	}
	return os;
}
