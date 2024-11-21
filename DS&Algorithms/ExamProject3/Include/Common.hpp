/**
 * Author: Andrew Kerr
 * Date: 3/21/2022
 * Description: The definition of the Common.cpp file located in the Source subfolder.
 */

#include <iostream>
#include <string>

//A helper macro, since im not using any windows headers that would normally contain this macro.
#define max(a, b) ((a > b) ? (a) : (b))

#ifndef __AK_COMMON_HPP__
#define __AK_COMMON_HPP__

//A simple function that basically implements the same thing as int strcmp(const char*, const char*)
int strcmp(const std::string& a, const std::string& b);

//A simple class that holds the simple Node variables (data, height, left/right, parent) which both AVL and normal unbalanced tree's need.
class Node {
private:
    //The data held within this node.
    std::string oData;

    //The height of the node in terms of how far it is from the root.
    int iHeight;

    //The left child node.
    Node* lpLeft;
    //The right child node.
    Node* lpRight;
    //The parent of this node.
    Node* lpParent;

public:
    //Constructs a new node with the given string as its data.
    Node(const std::string&);
    //Destroy's the node.
    ~Node();

public:
    //The height of the node (ie the max height of either its children plus one).
    int height() const;

    //The height of the node (ie the max height of either its children plus one) in reference form.
    int& height();
    
    //The data stored in this node (whatever was passed to the constructor).
    const std::string& data() const;
    
    //The left node (in pointer format) or NULL.
    Node*& left();
    //The right node (in pointer format) or NULL.
    Node*& right();
    //The parent node (in pointer format) or NULL.
    Node*& parent();

    //The immuteable left node (in pointer format) or NULL.
    const Node* left() const;
    //The immuteable right node (in pointer format) or NULL.
    const Node* right() const;
    //The immuteable parent node (in pointer format) or NULL.
    const Node* parent() const;
};

//A virtual implementation of a binary search tree which allows for both AVL and Unbalanced Tree's to implement custom insert (and if needed removal) methods.
class BinarySearchTree {
protected:
    //A pointer to the root node in the tree.
    Node* lpRoot;

public:
    //Constructs a new blank tree.
    BinarySearchTree();
    //Destorys the tree and it's underlying nodes.
    ~BinarySearchTree();

public:
    //Insert's a string into the tree (only one copy of the string can exist in the tree at any given time).
    virtual void insert(const std::string&) = 0;
    //Remove's a string from the tree.
    //virtual void remove(const std::string&) = 0;
    //Searches the tree for the given string and returns a boolean value based on the result of that search.
    bool contains(const std::string&);
    
public:
    //Clear's the tree of all entries effectively destroying each node and their underlying data.
    void clear();
    //Returns the height of the tree based off the root.
    int height();

public:
    //An operator call to insert (used inline cause it will allow effectively cut out one call instruction).
    BinarySearchTree& operator+=(const std::string&);
    //An operator call to remove (used inline cause it will allow effectively cut out one call instruction).
    //BinarySearchTree& operator-=(const std::string&);
    //An operator call to contains (used inline cause it will allow effectively cut out one call instruction).
    bool operator==(const std::string&);

public:
    //A friend operator capable of outputting the structure of the given BinarySearchTree nodes.
    friend std::ostream& operator<<(std::ostream&, BinarySearchTree&);
};

#endif//__AK_COMMON_HPP__