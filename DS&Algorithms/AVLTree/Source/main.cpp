#include "AVLTree.hpp"
#include <iostream>

int main(){

    AK::STORAGE::AVLTree<int> tree;

    tree.insert(50);
    tree.insert(30);
    tree.insert(80);
    tree.insert(10);
    tree.insert(60);
    tree.insert(90);
    tree.insert(55);
    tree.insert(70);
    tree.insert(85);
    tree.insert(100);

    std::cout << tree;

    return 0;
}