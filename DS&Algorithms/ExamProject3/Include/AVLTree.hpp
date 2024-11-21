/**
 * Author: Andrew Kerr
 * Date: 3/21/2022
 * Description: Defines an AVLTree which is a subclass of a BinarySearchTree.
 */

#include "Common.hpp"

#ifndef __AK_AVLTREE__
#define __AK_AVLTREE__

//An implementation of a balanced AVLTree.
class AVLTree : public BinarySearchTree {
public:
    //Insert's a string into the tree (only one copy of the string can exist in the tree at any given time).
    virtual void insert(const std::string&);
    //Remove's a string from the tree.
    //virtual void remove(const std::string&);
    //Searches the tree for the given string and returns a boolean value based on the result of that search.
    //virtual bool contains(const std::string&);
};

#endif//__AK_AVLTREE__