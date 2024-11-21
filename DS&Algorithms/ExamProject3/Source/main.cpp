/**
 * Author: Andrew Kerr
 * Date: 3/21/2022
 * Description: Simple main implementation used to actually run this program, nothing more nothing less.
 */

#include "AVLTree.hpp"
#include "UnbalancedTree.hpp"
#include "Common.hpp"
#include <iostream>
#include <fstream>

//The program's main entry point (not actually the entry point cause there is a function called "entry" that calls main).
int main(int argc, char* argv[]){
    //Check the number of parameters.
    if(argc > 3){
        std::cerr << "Error: Invalid number of parameters." << std::endl
                  << "Usage: " << argv[0] << " filename model" << std::endl
                  << "\tfilename - The name of the file to use." << std::endl
                  << "\tmodel - Which model to use either \"avl\" or \"unbalanced\"." << std::endl;
        return EINVAL;
    }

    //We know we will need a BinarySearchTree object (either AVLTree or UnbalancedTree) so lets keep a generic pointer to it.
    BinarySearchTree* bst = NULL;

    //Using our filename lets open a stream for us to read input from.
    const char* filename = argv[1];
    std::ifstream stream(filename);

    //Check if we have a valid, open stream.
    if(!stream.is_open()){
        std::cerr << "Error: Failed to open the given file \"" << filename << "\"" << std::endl;
        return EIO;
    }

    //Grab our model type and check against our two valid choices.
    std::string model = argv[2];
    if(model == "avl")
        bst = new AVLTree();
    else if(model == "unbalanced")
        bst = new UnbalancedTree();
    else{
        std::cerr << "Error: Second parameter \"model\" is not a valid choice, use \"avl\" or \"unbalanced\"." << std::endl
                  << "Usage: " << argv[0] << " filename model" << std::endl
                  << "\tfilename - The name of the file to use." << std::endl
                  << "\tmodel - Which model to use either \"avl\" or \"unbalanced\"." << std::endl;
        return EINVAL;
    }

    //Keep track of the number of entries that we will have to process.
    int entries;
    stream >> entries;

    //Read in every string from the string and insert it into the tree.
    std::string strInput;
    stream >> strInput;
    for(int i = 0; i < entries && stream; i++) {
        bst->operator+=(strInput);
        stream >> strInput;
    }

    //Print out the height of the tree.
    std::cout << "Height: " << bst->height() << std::endl;
    //std::cout << *bst << std::endl;

    //Reset the stream to the start of the file.
    stream.close();
    stream.open(filename);

    //Read in the number of entries again.
    stream >> entries;

    //Prime and then go through to check for each item's existance.
    stream >> strInput;
    for(int i = 0; i < entries && stream; i++){
        if(!bst->operator==(strInput)){
            std::cerr << "Warn: Couldn't find \"" << strInput << "\" in the " << model << " tree." << std::endl;
            return EINVAL;
        }
        stream >> strInput;
    }

    //Cleanup before we exit.
    delete bst;

    //std::cout << "Success!" << std::endl;
    return 0;
}