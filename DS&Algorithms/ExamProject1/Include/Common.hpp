#include <iostream>
#include <fstream>
#include <string.h>

#ifndef __COMMON_HPP__
#define __COMMON_HPP__

//#define __EXTRA_OUTPUT__

/*We want to use the strcmp(const char*, const char*) instead of s1 <=> s2, cause it only gives us boolean values which 
isn't always what we want.*/
int strcmp(const std::string& s1, const std::string& s2);

//When defined this will cause the algorithm function's to be double checked.
//#define __TEST_FOR_CORRECT__

#endif//__COMMON_HPP__
