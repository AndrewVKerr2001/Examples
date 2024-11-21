/**
 * Author: Andrew Kerr
 * Date: 3/21/2022
 * Description: The implementation of the AVLTree.hpp and BinaryTree.hpp file's located in the Include subfolder.
 *              Both header files are being implemented here because the both have similar "implementations" (really
 *                it's cause they are both subclasses of BinarySearchTree class).
 */

#include "AVLTree.hpp"
#include "UnbalancedTree.hpp"

//A useful wrapper function that when called will check if the node is valid and will return its height or 0.
int height(Node* n)
{
    if (n == NULL)
        return 0;
    return n->height();
}

//This will rotate the nodes to the right.
Node* rightRotate(Node* p0)
{
    //Declare our two pivot nodes.
    Node* p1 = p0->left();
    Node* p2 = p1->right();

    //Move about the nodes such that they rotate right.
    p1->right() = p0;
    p0->left() = p2;

    //Recalculate our height.
    p0->height() = max(height(p0->left()),
        height(p0->right())) + 1;
    p1->height() = max(height(p1->left()),
        height(p1->right())) + 1;
    return p1;
}

//This will rotate the nodes to the left.
Node* leftRotate(Node* p0)
{
    //Declare our two pivot nodes.
    Node* p1 = p0->right();
    Node* p2 = p1->left();

    //Move about the nodes such that they rotate left.
    p1->left() = p0;
    p0->right() = p2;

    //Recalcualte our height.
    p0->height() = max(height(p0->left()),
        height(p0->right())) + 1;
    p1->height() = max(height(p1->left()),
        height(p1->right())) + 1;
    return p1;
}

//Determines the balance of the given node if not NULL.
int getBalance(Node* n)
{
    if (n == NULL)
        return 0;
    return height(n->left()) -
        height(n->right());
}

//The actual recursive avl insert.
Node* avl_insert(Node* node, const std::string& key)
{
    //Add a new node if we finally reach a NULL entry.
    if (node == NULL)
        return new Node(key);

    //Recursively enter the left or right nodes if we the key is different.
    if (strcmp(key,node->data()) < 0)
        node->left() = avl_insert(node->left(), key);
    else if (strcmp(key, node->data()) > 0)
        node->right() = avl_insert(node->right(), key);
    else //We found and exact match so return this node.
        return node;

    //Update this nodes height.
    node->height() = 1 + max(height(node->left()),
        height(node->right()));

    //Get the balance between the two nodes.
    int balance = getBalance(node);

    // Right Right Case
    if (balance < -1 && key > node->right()->data())
        return leftRotate(node);

     // Left Left Case
    if (balance > 1 && key < node->left()->data())
        return rightRotate(node);

    // Right Left Case
    if (balance < -1 && key < node->right()->data())
    {
        node->right() = rightRotate(node->right());
        return leftRotate(node);
    }

    // Left Right Case
    if (balance > 1 && key > node->left()->data())
    {
        node->left() = leftRotate(node->left());
        return rightRotate(node);
    }
    return node;
}

void AVLTree::insert(const std::string& s) {
    this->lpRoot = avl_insert(this->lpRoot, s);
}

Node* unbalanced_insert(const std::string& s, Node*& cur) {
    //Check if we finally reached a null entry.
    if(cur == NULL){
        cur = new Node(s);
        return cur;
    }

    //Compare if we are going to the left or right for recursive inserts.
    if(strcmp(s,cur->data()) < 0){
        //Attempt to insert to the left.
        Node* res = unbalanced_insert(s, cur->left());
        if(res == cur->left()) //Inserted to the left, update the link's.
            cur->left()->parent() = cur;
        //Recalculate the height.
        cur->height() = max(cur->height(), cur->left()->height()+1);
        return res;
    }
    else if(strcmp(s,cur->data()) > 0){
        //Attempt to insert to the right.
        Node* res = unbalanced_insert(s, cur->right());
        if(res == cur->right()) //Inserted to the right, update the link's.
            cur->right()->parent() = cur;
        //Recalculate the height.
        cur->height() = max(cur->height(), cur->right()->height()+1);
        return res;
    }
    //Nothing to do, we have found an existing match.
    return NULL;
}

void UnbalancedTree::insert(const std::string& s) {
    unbalanced_insert(s, this->lpRoot);
}