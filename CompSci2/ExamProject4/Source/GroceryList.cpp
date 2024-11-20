#include "GroceryList.hpp"

//===[ CONSTRUCTORS ]=========================================================
Node::Node() {
	_parent = NULL;
	_left = NULL;
	_right = NULL;
	_data = NULL;
}

GroceryList::GroceryList() {
	_root = NULL;
	_nodeCount = 0;
	_maxNodeDepth = 0;
}

//===[ DECONSTRUCTORS ]=======================================================

Node::~Node() {
	if(_data) delete _data;
	_parent = NULL;
	_left = NULL;
	_right = NULL;
	_data = NULL;
}

GroceryList::~GroceryList() {
	this->clear();
	_root = NULL;
	_nodeCount = 0;
	_maxNodeDepth = 0;
}

//===[ GETTERS ]==============================================================

bool Node::isLeaf() const { return !(_left || _right); }
int Node::depth() const {
	int count = 0;
	const Node* parent = _parent;
	while(parent){
		count++;
		parent = parent->_parent;
	}
	return count;
}

Node*& Node::parent() { return _parent; }
Node*& Node::left() { return _left; }
Node*& Node::right() { return _right; }
GroceryItem*& Node::data() { return _data; }

int GroceryList::maxSearchLength() const {
	return _maxNodeDepth;
}

int GroceryList::getLength() const {
	return _nodeCount;
}

//===[ RECURSION ]============================================================

Node* GroceryList::TreeInsert(const GroceryItem& target, Node*& node) {
	if(!node) {
		node = new Node();
		node->data() = new GroceryItem(target);
		_nodeCount++;
		return node;
	}
	
	if(*node > target) {
		Node* fNode = TreeInsert(target, node->left());
		if(fNode && !fNode->parent()) {
			fNode->parent() = node;
			int depth = fNode->depth();
			if(depth > _maxNodeDepth)
				_maxNodeDepth = depth;
		}
		return fNode;
	}
	
	if(*node == target)
		return node;
	
	Node* fNode = TreeInsert(target, node->right());
	if(fNode && !fNode->parent()) {
		fNode->parent() = node;
		int depth = fNode->depth();
		if(depth > _maxNodeDepth)
			_maxNodeDepth = depth;
	}
	return fNode;
}

Node* GroceryList::TreeSearch(const std::string& target, Node* node) {
	if(!node) return NULL;
	
	if(node->data()->name() > target)
		return TreeSearch(target, node->left());
	if(node->data()->name() == target)
		return node;
	return TreeSearch(target, node->right());
}

Node* GroceryList::TreeRemove(const std::string& target) {
	Node* node = TreeSearch(target, _root);
	if(!node) return NULL;
	
	if(node->isLeaf()) {
		if(node == _root)
			_root=NULL;
		else {
			Node* parent = node->parent();
			if(parent->left() == node)
				parent->left() = NULL;
			else
				parent->right() = NULL;
		}
	} else {
		if(!node->left()){ //Have only right node.
			if(node->parent()->left() == node)
				node->parent()->left() = node->right();
			else
				node->parent()->right() = node->right();
		} else { //Have left node and possbly right node. TODO: Incomplete....
			
			//Examine the tree and retrieve the next largest value...
			Node* replacement = node->left();
			while(replacement->right())
				replacement = replacement->right();
				
			//Replace the node in the tree with the replacement node.
			if(node->parent()->left() == node)
				node->parent()->left() = replacement;
			else
				node->parent()->right() = replacement;
				
			//Copy over replacement
			replacement->left() = node->left();
			replacement->right() = node->right();
			
			//Clear the replacements parent before updating its parent.
			if(replacement->parent()){
				if(replacement->parent()->left() == replacement)
					replacement->parent()->left() = NULL;
				else
					replacement->parent()->right() = NULL;
			}
			replacement->parent() = node->parent();
		}
	}
	
	//Decrement count.
	_nodeCount--;
	
	//Clear node
	node->parent() = node->left() = node->right() = NULL;
	
	_maxNodeDepth = TreeDepthCalc(_root);
	
	//Return removed node.
	return node;
}

void GroceryList::TreeCleanup(Node* node) {
	if(!node) return;
	
	//Cleanup...
	this->TreeCleanup(node->left());
	this->TreeCleanup(node->right());
	delete node;
}

int GroceryList::TreeDepthCalc(Node* n) {
	if(!n) return 0;
	int depth = 0;
	if(n->left()) depth = TreeDepthCalc(n->left());
	if(n->right()) {
		int temp = TreeDepthCalc(n->right());
		if(temp > depth) depth = temp;
	} 
	if(depth == 0) depth = n->depth();
	return depth;
}

//===[ WRAPPERS ]=============================================================
	
bool GroceryList::insert(const GroceryItem& data){
	return this->TreeInsert(data, _root);
}

bool GroceryList::contains(const std::string& data){
	return this->TreeSearch(data,_root);
}

bool GroceryList::remove(const std::string& data){
	return this->TreeRemove(data);
}

void GroceryList::clear() {
	//Pointer manipulation so that the tree is immediately made unavailable.
	Node* tmp = _root;
	_root = NULL;
	_nodeCount = 0;
	_maxNodeDepth = 0;
	this->TreeCleanup(tmp);
}

void Node::print(std::ostream& o, int tabs) const {
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

void GroceryList::checkOff(const std::string& s) {
	Node* node = TreeSearch(s,_root);
	if(node){
		node->data()->bought() = true;
	}
}

//===[ OPERATORS ]============================================================

std::ostream& operator<<(std::ostream& o, const Node& node){
	node.print(o,0);
	return o;
}

std::ostream& operator<<(std::ostream& o, const GroceryList& bst) {
	if(bst._root)
		o << *bst._root;
	else
		o << "GroceryList is empty." << std::endl;
	return o;
}

bool Node::operator<(const GroceryItem& item){
	return _data->name() < item.name();
}

bool Node::operator==(const GroceryItem& item){
	return _data->name() == item.name();
}

bool Node::operator>(const GroceryItem& item){
	return _data->name() > item.name();
}

GroceryList& GroceryList::operator=(const GroceryList& o) {
	this->clear();
	
	return *this;
}

GroceryList& GroceryList::operator+=(const GroceryList&) {
	
	return *this;	
}

GroceryList& GroceryList::operator+=(const GroceryItem& item) {
	this->insert(item);
	return *this;
}

GroceryList& GroceryList::operator-=(const std::string& s) {
	this->remove(s);
	return *this;
}

GroceryItem* GroceryList::operator[](const std::string& s) {
	Node* node = TreeSearch(s, _root);
	if(node) return node->data();
	return NULL;
}

