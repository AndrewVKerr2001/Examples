#ifndef __PARTIALLY_FILLED_ARRAY_HPP_
#define __PARTIALLY_FILLED_ARRAY_HPP_

#include <ostream>
#include <string>

class PartiallyFilledArray {
private:
	int _size;
	int _index;
	std::string* _array;
	
public:
	PartiallyFilledArray();
	PartiallyFilledArray(int size);
	~PartiallyFilledArray();
	
public:
	void add(const std::string &newElm);
	bool resize(int size);
	
public:
	int size() const;
	int count() const;
	
public:
	friend std::ostream& operator<<(std::ostream&, const PartiallyFilledArray&);
};

#endif//__PARTIALLY_FILLED_ARRAY_HPP_
