#include "BinarySearchTree.hpp"

BinarySearchTree::Node::Node() {
	_parent = NULL;
	_left = NULL;
	_right = NULL;
	_data = NULL;
}

BinarySearchTree::Node::~Node() {
	if(_data) delete _data;
	_parent = NULL;
	_left = NULL;
	_right = NULL;
	_data = NULL;
}

BinarySearchTree::Node*& BinarySearchTree::Node::parent() { return _parent; }
BinarySearchTree::Node*& BinarySearchTree::Node::left() { return _left; }
BinarySearchTree::Node*& BinarySearchTree::Node::right() { return _right; }
BST_TYPE*& BinarySearchTree::Node::data() { return _data; }

BinarySearchTree::BinarySearchTree() {
	_root = NULL;
}

BinarySearchTree::~BinarySearchTree() {
	this->clear();
	_root = NULL; //Memory leak, needs gc implementation.
}

BinarySearchTree::Node* BinarySearchTree::TreeInsert(const BST_TYPE& target, Node*& node) {
	if(!node) {
		node = new Node();
		node->data() = new BST_TYPE(target);
		return node;
	}
	
	if(*node->data() > target) {
		Node* fNode = TreeInsert(target, node->left());
		if(fNode && !fNode->parent()) fNode->parent() = node;
		return fNode;
	}
	
	if(*node->data() == target)
		return node;
		
	if(*node->data() < target) {
		Node* fNode = TreeInsert(target, node->right());
		if(fNode && !fNode->parent()) fNode->parent() = node;
		return fNode;
	}
	
	//Should never get here...
	return NULL;
}

BinarySearchTree::Node* BinarySearchTree::TreeSearch(const BST_TYPE& target, Node* node) {
	if(!node) return NULL;
	
	if(*node->data() > target)
		return TreeSearch(target, node->left());
	if(*node->data() == target)
		return node;
	if(*node->data() < target)
		return TreeSearch(target, node->right());
	//Should never get here!!!
	return NULL;
}

bool BinarySearchTree::TreeRemove(const BST_TYPE& target, Node*& node) {
	if(!node) return false;
	
	if(*node->data() > target)
		return TreeRemove(target, node->left());
	if(*node->data() < target)
		return TreeRemove(target, node->right());
		
	if(node->left() || node->right()){
		if(node->left() && !node->right()){
			node->left()->parent() = node->parent();
			node = node->left();
		} else {
			//TODO: Find out how to do a BinarySearchTree removal with two children.
			Node* replacer = node->right();
			while(replacer->left())
				replacer = replacer->left();
			
			node->data() = replacer->data();
			if(replacer == node->right()){
				if(replacer->right()){
					node->right() = replacer->right();
					if(node->right()) node->right()->parent() = node;
				}else{
					delete node->right();
					node->right() = NULL;
				}
			}else{
				if(replacer->right()) {
					replacer->parent()->left() = replacer->right();
					if(replacer->right()) replacer->right()->parent() = replacer->parent();
				} else {
					replacer->parent()->left() = NULL;
				}
				delete replacer;
			}
			replacer = NULL;
		}
	} else {
		delete node;
		node = NULL;
	}
	return true;
		
	/*Node* leftmost = node->right();
	if(leftmost == NULL){ // no right nodes to search down, use left node in single child mode.
		if(node->left()){
			node->left()->parent() = node->parent();
			node = node->left();
		}else{
			delete node;
			node = NULL;
		}
	} else {
		while(leftmost->left())
			leftmost = leftmost->left();
		
		if(leftmost == node->right()){
			node->right() = NULL;
		}
		
		if(leftmost->right()){
			leftmost->parent()->left() = leftmost->right();
			leftmost->right()->parent() = leftmost->parent();
		}else{
			leftmost->parent()->left() = NULL;
		}
		leftmost->right() = NULL;
		leftmost->parent() = NULL;
		node->data() = leftmost->data();
	}*/
}

void BinarySearchTree::TreeCleanup(Node* node) {
	if(!node) return;
	
	//Cleanup...
	this->TreeCleanup(node->left());
	this->TreeCleanup(node->right());
	delete node;
}
	
bool BinarySearchTree::insert(const BST_TYPE& data){
	return this->TreeInsert(data, _root);
}

bool BinarySearchTree::contains(const BST_TYPE& data){
	return this->TreeSearch(data,_root);
}

bool BinarySearchTree::remove(const BST_TYPE& data){
	return this->TreeRemove(data, _root);
}

void BinarySearchTree::clear() {
	//Pointer manipulation so that the tree is immediately made unavailable.
	Node* tmp = _root;
	_root = NULL;
	this->TreeCleanup(tmp);
}

void BinarySearchTree::Node::print(std::ostream& o, int tabs) const {
	/*for(int i = 0; i < tabs; i++) o << " ";
	o << "{" << std::endl;
	
	for(int i = 0; i < tabs; i++) o << " ";
	o << " Data: " << *_data << std::endl;
	
	if(_left){
		for(int i = 0; i < tabs; i++) o << " ";
		o << " Left:" << std::endl;
		_left->print(o, tabs+1);
	}
	
	if(_right){
		for(int i = 0; i < tabs; i++) o << " ";
		o << " Right:" << std::endl;
		_right->print(o, tabs+1);
	}
	
	for(int i = 0; i < tabs; i++) o << " ";
	o << "}," << std::endl << std::endl;*/
	
	
	if(_right){
		_right->print(o, tabs+1);
	}
	
	const char* identifier = (_left || _right ? "+> " : "> ");
	
	for(int i = 0; i < tabs-1; i++) o << "| ";
	o << (_parent ? "|-" : "") << identifier << *_data << std::endl;
	
	if(_left){
		_left->print(o, tabs+1);
	}
	
}

std::ostream& operator<<(std::ostream& o, const BinarySearchTree::Node& node){
	node.print(o,0);
	return o;
}

std::ostream& operator<<(std::ostream& o, const BinarySearchTree& bst) {
	if(bst._root)
		o << *bst._root;
	else
		o << "BinarySearchTree is empty." << std::endl;
	return o;
}


