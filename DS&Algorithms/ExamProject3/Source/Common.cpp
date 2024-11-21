/**
 * Author: Andrew Kerr
 * Date: 3/21/2022
 * Description: The implementation of the Common.hpp file located in the Include subfolder.
 */

#include "Common.hpp"

int strcmp(const std::string& a, const std::string& b) {
    if(a < b) return -1;
    if(a > b) return 1;
    return 0;
}

Node::Node(const std::string& s) {
    this->oData = s;
    this->iHeight = 1;
    this->lpLeft = NULL;
    this->lpRight = NULL;
    this->lpParent = NULL;
}

Node::~Node() {
    this->lpLeft = NULL;
    this->lpRight = NULL;
    this->lpParent = NULL;
    this->iHeight = 0;
    this->oData.clear();
}

//The height of the node (ie the max height of either its children plus one).
int Node::height() const {
    return this->iHeight;
}

//The height of the node (ie the max height of either its children plus one) in reference form.
int& Node::height() {
    return this->iHeight;
}

//The data stored in this node (whatever was passed to the constructor).
const std::string& Node::data() const {
    return this->oData;
}

//The left node (in pointer format) or NULL.
Node*& Node::left() {
    return this->lpLeft;
}

//The right node (in pointer format) or NULL.
Node*& Node::right() {
    return this->lpRight;
}

//The parent node (in pointer format) or NULL.
Node*& Node::parent() {
    return this->lpParent;
}

//The immuteable left node (in pointer format) or NULL.
const Node* Node::left() const {
    return this->lpLeft;
}

//The immuteable right node (in pointer format) or NULL.
const Node* Node::right() const {
    return this->lpRight;
}

//The immuteable parent node (in pointer format) or NULL.
const Node* Node::parent() const {
    return this->lpParent;
}

BinarySearchTree::BinarySearchTree() {
    this->lpRoot = NULL;
}

BinarySearchTree::~BinarySearchTree() {
    this->clear();
}

Node* node_contains(const std::string& s, Node*& cur) {
    if(cur == NULL)
        return NULL;
    if(strcmp(s,cur->data()) < 0)
        return node_contains(s, cur->left());
    else if(strcmp(s,cur->data()) > 0)
        return node_contains(s, cur->right());
    return cur;
}

bool BinarySearchTree::contains(const std::string& s){
    return node_contains(s, this->lpRoot);
}

void delete_subtree_nodes(Node*& node){
    //Check if we are trying to do something on a null subtree.
    if(!node)
        return;

    //Delete if we have any sub nodes to delete
    Node* l = node->left();
    delete_subtree_nodes(l);
    Node* r = node->right();
    delete_subtree_nodes(r);

    //Delete the node now that all subnodes are gone and set our parent's pointer to null.
    delete node;
    node = NULL;
}

void BinarySearchTree::clear() {
    delete_subtree_nodes(this->lpRoot);
}

int BinarySearchTree::height() {
    return (this->lpRoot ? this->lpRoot->height() : 0);
}

BinarySearchTree& BinarySearchTree::operator+=(const std::string& s) {
    this->insert(s);
    return *this;
}

/*BinarySearchTree& BinarySearchTree::operator-=(const std::string& s) {
    this->remove(s);
    return *this;
}*/

bool BinarySearchTree::operator==(const std::string& s){
    return this->contains(s);
}

void output_subtree(std::ostream& o, const Node* n, int rootHeight){
    //Check for null node.
    if(!n)
        return;
    output_subtree(o, n->left(), rootHeight);
    for(int i=0; i<rootHeight - n->height(); i++) o << '\t';
    o << n->data() << std::endl;
    output_subtree(o, n->right(), rootHeight);
}

std::ostream& operator<<(std::ostream& o, BinarySearchTree& bst){
    output_subtree(o, bst.lpRoot, (bst.lpRoot ? bst.lpRoot->height() : 0));
    return o;
}