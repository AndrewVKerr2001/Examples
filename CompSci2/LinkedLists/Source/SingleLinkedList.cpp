#include "SingleLinkedList.hpp"

SingleLinkedNode::SingleLinkedNode() {
	this->_next = NULL;
	this->_data = NULL;
}

SingleLinkedNode::~SingleLinkedNode() {
	if(this->_data) delete this->_data;
	this->_next = NULL;
	this->_data = NULL;
}

SingleLinkedList::SingleLinkedList() {
	this->_head = NULL;
	this->_tail = NULL;
	this->_nodeCount = 0;
}

void SingleLinkedList::append(const std::string& val) {
	SingleLinkedNode* node = new SingleLinkedNode();
	node->data() = new std::string(val);
	
	if(_tail){
		_tail->next() = node;
		_tail = node;
	} else {
		_head = node;
		_tail = node;
	}
	
	_nodeCount++;
}

void SingleLinkedList::prepend(const std::string& val) {
	SingleLinkedNode* node = new SingleLinkedNode();
	node->data() = new std::string(val);
	
	if(_head){
		node->next() = _head;
		_head = node;
	} else {
		_head = node;
		_tail = node;
	}
	
	_nodeCount++;
}

void SingleLinkedList::remove(const std::string& val) {
	SingleLinkedNode* node = _head;
	SingleLinkedNode* prev = NULL;
	while(node){
		if(node->data() && *node->data() == val){
			if(prev)
				prev->next() = node->next();
			else
				_head = node->next();
			if(node == _tail)
				_tail = prev;
			
			delete node->data();
			delete node;
			
			_nodeCount--;
			
			break;
		}
		prev = node;
		node = node->next();
	}
}

void SingleLinkedList::clear() {
	_head = NULL;
	_tail = NULL;
	_nodeCount = 0;
}

std::string* SingleLinkedList::find(const std::string& val) const {
	SingleLinkedNode* node = _head;
	while(node){
		if(node->data() && *node->data() == val){
			return node->data();
		}
		node = node->next();
	}
	return NULL;
}

std::ostream& operator<<(std::ostream& os, const SingleLinkedList& list){
	//os << "Length: " << list._nodeCount << std::endl;
	SingleLinkedNode* cur = list._head;
	while(cur){
		if(cur->data())
			os << *cur->data() << std::endl;
		cur = cur->next();
	}
	return os;
}
