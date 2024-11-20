/***********************************************************
 *    FILE: GroceryItem.hpp                                *
 *  AUTHOR: S. Blythe                                      *
 *    DATE: 8/2021                                         *
 * PURPOSE: Class definition for GroceryItem, as needed in * 
 *            CSC24400 project 2                           * 
 ***********************************************************/
#ifndef _GROCERY_ITEM_HPP_
#define _GROCERY_ITEM_HPP_

#include <iostream>
#include <string>

class GroceryItem
{
private:
  std::string _name; // the name of the Grocery Item 
  int _quantity;     // amount of item needed
  bool _bought;      // has the item been bought yet? 
public:

  // default constructor
  GroceryItem(): _name(), _quantity(0), _bought(false) {}

  // constructor with name and quantity (defaults to 1 if not specified)
  GroceryItem(const std::string &name, int quantity=1):
    _name(name), _quantity(quantity), _bought(false) {}

  // accessor/modifier for name
  const std::string& name() const {return _name;}
  std::string& name() {return _name;}

  // accessor/modifier for quantity
  int quantity() const {return _quantity;}
  int& quantity() {return _quantity;}

  // accessor/modifier for bought status
  bool bought() const {return _bought;}
  bool& bought() {return _bought;} 

  // overloads that allow for increment or decrement of quantitty. 
  GroceryItem operator++() {_quantity++; return *this;}
  GroceryItem operator--() {_quantity--; return *this;} 

  // methods to print or rad to/from a stream
  std::ostream& print(std::ostream &os) const;
  std::istream& read(std::istream &is);

  // overload the << (output) and >> (input) operator
  friend std::ostream& operator<< (std::ostream &os, const GroceryItem &gi);
  friend std::istream& operator>> (std::istream &is, GroceryItem &gi);
};


#endif
