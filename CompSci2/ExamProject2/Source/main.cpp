/**
 * Author: Andrew Kerr
 * Date: Oct 29 2021
 */
#include <iostream>
#include <fstream>
#include <string>
#include "GroceryItem.hpp"
#include "GroceryList.hpp"


#define _TEST_CASE_ 10

using namespace std;

#if _TEST_CASE_ == 8
void performLookup(const GroceryList &gl, const string &item)
{
  GroceryItem *ptr = gl[item];
  if (ptr!=NULL)
  { 
    cout << "Found: " << *ptr << endl;
  }
  else
  {
    cout << "Sorry, " << item << " was not found." << endl;
  }
}
#endif

#if _TEST_CASE_ == 10
void checkConst(const GroceryList &cgl)
{
  cout << cgl.getLength() << " " << cgl.numEmptySlots() << endl;
  cout << cgl << endl;
  GroceryList temp;
  temp=cgl;
  temp+=cgl;
  GroceryItem *ptr = cgl["Robot"];
  if (ptr!=NULL)
    cout << "found:" << *ptr << endl;
}
#endif

int main(int argc, char *argv[]){
	

#if _TEST_CASE_ == 1
	{
		GroceryList shopping;
		cout << "This should be zero:" << shopping.getLength() << endl;
		cout << "This should be five:" << shopping.numEmptySlots() << endl;
	}
#endif
	
#if _TEST_CASE_ == 2
	{
		GroceryList shopping;
		cout << "This should be zero:" << shopping.getLength() << endl;
		cout << "This should be five:" << shopping.numEmptySlots() << endl;
		cout << "There should be nothing between the two ============= lines!"
		   << endl
		   << "====================" << endl
		   << shopping
		   << "====================" << endl;

		GroceryItem cookies("Oreos", 4);

		shopping+=cookies;
		cout << "There should be only oreos (4)  between the two ============= lines!"
		   << endl
		   << "====================" << endl
		   << shopping
		   << "====================" << endl;
		cout << "This should be one:" << shopping.getLength() << endl;
		cout << "This should be four:" << shopping.numEmptySlots() << endl;
		GroceryItem candy("Halloween Candy", 137);

		shopping+=candy;
		cout << "There should be only oreos and halloween candy between the =========== lines!"
		   << endl
		   << "====================" << endl
		   << shopping
		   << "====================" << endl;
		cout << "This should be two:" << shopping.getLength() << endl;
		cout << "This should be three:" << shopping.numEmptySlots() << endl;
	}
#endif
	
#if _TEST_CASE_ == 3
	{
		GroceryList shopping;
		GroceryItem cookies("Oreos", 4);

		shopping+=cookies;
		GroceryItem candy("Halloween Candy", 137);

		shopping+=candy;
		shopping+=candy;
		cout << "There should be 274 Halloween Candy and 4 Oreos in this list:"
		   << endl
		   << "================================="
		   << endl
		   << shopping
			   << "================================="
		   << endl;

		cout << "This should be two:" << shopping.getLength() << endl;
		cout << "This should be three:" << shopping.numEmptySlots() << endl;
		GroceryItem moreOreos("Oreos", 2);
		shopping+=moreOreos;

		cout << "There should be 274 Halloween Candy and 6 Oreos in this list:"
		   << endl
		   << "================================="
		   << endl
		   << shopping
			   << "================================="
		   << endl;
		cout << "This should be two:" << shopping.getLength() << endl;
		cout << "This should be three:" << shopping.numEmptySlots() << endl;
	}
#endif

#if _TEST_CASE_ == 4

	{
		GroceryList shopping;
		cout << "This should be zero:" << shopping.getLength() << endl;
		cout << "This should be five:" << shopping.numEmptySlots() << endl;
		cout << "There should be nothing between the two ============= lines!"
		<< endl
		<< "====================" << endl
		<< shopping
		<< "====================" << endl;

		GroceryItem cookies("Oreos", 4);

		shopping+=cookies;
		GroceryItem candy("Halloween Candy", 137);

		shopping+=candy;
		cout << "There should be only oreos and halloween candy between the =========== lines!"
		<< endl
		<< "====================" << endl
		<< shopping
		<< "====================" << endl;
		cout << "This should be two:" << shopping.getLength() << endl;
		cout << "This should be three:" << shopping.numEmptySlots() << endl << endl;
		GroceryList copy;
		copy=shopping; 
		cout << "There should be only oreos and halloween candy between the =========== lines!"
		<< endl
		<< "====================" << endl
		<< copy
		<< "====================" << endl;
		cout << "This should be two:" << copy.getLength() << endl;
		cout << "This should be three:" << copy.numEmptySlots() << endl << endl;
		GroceryItem copyAdd("Celery" , 1);
		copy += copyAdd;

		cout << "oreos, halloween candy, and celery between the =========== lines!"
		<< endl
		<< "====================" << endl
		<< copy
		<< "====================" << endl;
		cout << "This should be three:" << copy.getLength() << endl;
		cout << "This should be two:" << copy.numEmptySlots() << endl << endl;
		cout << "There should be only oreos and halloween candy between the =========== lines!"
		<< endl
		<< "====================" << endl
		<< shopping
		<< "====================" << endl;
		cout << "This should be two:" << shopping.getLength() << endl;
		cout << "This should be three:" << shopping.numEmptySlots() << endl << endl;

	}

#endif

#if _TEST_CASE_ == 5

	{
		GroceryList shopping;
		GroceryItem cookies("Oreos", 4);

		shopping+=cookies;
		GroceryItem candy("Halloween Candy", 137);

		shopping+=candy;
		GroceryList copy, copy2;
		copy2=copy=shopping; 
		GroceryItem copyAdd("Celery" , 1);
		copy += copyAdd;

		cout << "oreos, halloween candy, and celery between the =========== lines!"
		   << endl
		   << "====================" << endl
		   << copy
		   << "====================" << endl;
		cout << "This should be three:" << copy.getLength() << endl;
		cout << "This should be two:" << copy.numEmptySlots() << endl << endl;
		copyAdd.name()="Ranch";
		copyAdd.quantity() =2;

		cout << "oreos, halloween candy, ranch between the =========== lines!"
		   << endl
		   << "====================" << endl
		   <<   (copy2+=copyAdd)
		   << "====================" << endl;
		cout << "This should be three:" << copy2.getLength() << endl;
		cout << "This should be two:" << copy2.numEmptySlots() << endl << endl;
	}

#endif

#if _TEST_CASE_ == 6

	{
		string fname;
		cout << "Enter name of input file: ";
		cin >> fname;
		ifstream ifile(fname.c_str());
		GroceryList shopping;
		GroceryItem temp;

		ifile >> temp;

		while (ifile)
		{
		  shopping+=temp;
		  ifile>>temp;
		}
		ifile.close();
		cout << "Number of items in list: "<< shopping.getLength()<<endl;
		cout << "Number of empty slots  in list: "<< shopping.numEmptySlots()<<endl;
		cout << "The resulting shopping list:" << endl;
		cout << shopping;
	}

#endif

#if _TEST_CASE_ == 7
	
	{
		ifstream ifile("test2.txt");
		GroceryList shopping;
		GroceryItem temp;

		ifile >> temp;

		while (ifile)
		{
		  shopping+=temp;
		  ifile>>temp;
		}
		ifile.close();

		cout << "Number of items in list BEFORE removing Robot: "
		   << shopping.getLength()<<endl;
		shopping-="Robot";
		cout << "Number of items in list AFTER removing Robot: "
		   << shopping.getLength()<<endl;
		shopping-="Klingon";

		cout << "Number of items in list AFTER removing Klingon: "
		   << shopping.getLength()<<endl;
	}

#endif

#if _TEST_CASE_ == 8
	{
		ifstream ifile("test2.txt");
		GroceryList shopping;
		GroceryItem temp;

		ifile >> temp;

		while (ifile)
		{
		  shopping+=temp;
		  ifile>>temp;
		}
		ifile.close();
		performLookup(shopping, "Robot");
		performLookup(shopping, "Klingon");
		shopping["Robot"]->name() = "Android";
		cout << "===Robot should be replaced with Android in the list:===" << endl
		   << shopping;
	}
#endif

#if _TEST_CASE_ == 9
	{
		ifstream ifile("test2.txt");
		GroceryList shopping;
		GroceryItem temp;

		ifile >> temp;

		while (ifile)
		{
		  shopping+=temp;
		  ifile>>temp;
		}
		ifile.close();
		shopping.checkOff("Robot");
		cout << "===Robot should be PURCHASED in the list:===" << endl
		   << shopping;
		cout << "The following should not cause a crash:" << endl;
		shopping.checkOff("PowerballWinner");
		cout << "Good - it did not crash!"  << endl;
	}
#endif

#if _TEST_CASE_ == 10
	{
		cout << "If you're seeing this print at run time,"
		   << " this test case compiled ..." << endl
		   << " ... which is the main pont of this test case." << endl;

		ifstream ifile("test2.txt");
		GroceryList shopping;
		GroceryItem temp;

		ifile >> temp;

		while (ifile)
		{
		  shopping+=temp;
		  ifile>>temp;
		}
		ifile.close();
		checkConst(shopping);
	}
#endif

#if _TEST_CASE_ == 11
	{
		GroceryItem item1("burger", 3);
		GroceryItem item2("shampoo", 4);
		
		GroceryList list;
		list += item1;
		list += item2;
		
		std::cout << list << std::endl;
		
		list.checkOff("burger");
		
		std::cout << list << std::endl;
		
		list += item2;
		
		std::cout << list << std::endl;
		
		list -= item1.name();
		
		std::cout << list << std::endl;
	}	
#endif
	return 0;		
}
