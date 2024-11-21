/**
 * Author: Andrew Kerr
 * Date: 3/21/2022
 * Description: Defines an UnbalancedTree which is a subclass of a BinarySearchTree.
 */

#include "Common.hpp"

#ifndef __AK_BINARYTREE_HPP__
#define __AK_BINARYTREE_HPP__

//An implementation of an unbalanced form of a binary search tree.
class UnbalancedTree : public BinarySearchTree {
public:
    //Insert's a string into the tree (only one copy of the string can exist in the tree at any given time).
    virtual void insert(const std::string&);
    //Remove's a string from the tree.
    //virtual void remove(const std::string&);
    //Searches the tree for the given string and returns a boolean value based on the result of that search.
    //virtual bool contains(const std::string&);
};

#endif//__AK_BINARYTREE_HPP__