#include "MergeSort.hpp"
#include <unistd.h>

void top_down_merge(std::string* src, std::string* dst, int begin, int middle, int end) {
	int i = begin;
	int j = middle;
	
	for(int k = begin; k < end; k++){
		if(i < middle && ( j >= end || strcmp(src[i].c_str(), src[j].c_str()) <= 0)){
			dst[k] = src[i];
			i = i + 1;
		}
		else {
			dst[k] = src[j];
			j = j + 1;
		}
	}
}

void split_merge(std::string* src, std::string* dst, int begin, int end) {
	if((end - begin) <= 1) return; //BASE CASE (Recursion inbound)... :^)
	
	int middle = (end + begin) / 2;
	
	split_merge(dst, src, begin, middle);
	split_merge(dst, src, middle, end);
	
	top_down_merge(src, dst, begin, middle, end);
}

void prime_merge_sort(int size, std::string* arr) {
	if(size <= 1 || !arr) return;
	std::string* work_arr = new std::string[size];
	if(!work_arr) return;
	
	//Copy over our array to the work array.
	{
		std::string* src = arr;
		std::string* dst = work_arr;
		for(int i = 0; i < size; i++)
			*dst++ = *src++;
	}
	
	//Once done we should be sorted.
	split_merge(work_arr, arr, 0, size);
	
	//Done with this, lets clean up.
	delete[] work_arr;
}

int MergeSort(int nSize, std::istream& rInput, bool bAllowOutput) {	
	//Attempt to create a buffer for merge sort.
	std::string* strings = new std::string[nSize];
	if(!strings) return ENOMEM; //Pointer not allocated, out of memory.
	
	//Read in our strings.
	{
		std::string* temp = strings;
		for(int i = 0; i < nSize; i++)
			rInput >> temp[i];
	}
	
	//Sort the array...
	prime_merge_sort(nSize, strings);
	
	//Check if output then output if needed.
	if(bAllowOutput){
		std::string* temp = strings;
		for(int i = 0; i < nSize; i++)
			std::cout << temp[i] << std::endl;
	}

#ifdef __TEST_FOR_CORRECT__
    //Testing for correct entries!
    if(nSize >= 1){
        bool bOffender = false;
        for(int i = 0; i < nSize-1; i++){
            if(strings[i] > strings[i+1]) {
                bOffender = true;
                std::cout << "\nIncorrect entry:\n\tIndex: " << i << ", value: " << strings[i] << std::endl;
                break;
            }
        }
        if(!bOffender) std::cout << "Correctly sorted!!!!" << std::endl;
    }
#endif
	
	//Deallocate out memory.
	delete[] strings;
	
	return 0;
}
