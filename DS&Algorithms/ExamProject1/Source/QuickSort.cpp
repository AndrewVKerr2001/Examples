#include "QuickSort.hpp"

void quick_sort(std::string* arr, int size) {
	//Recursion inbound, base case is stated.
	if(size <= 1 || !arr) return;
	
	//Pick our "middle" value.
	int middle_index = (size/2);
	std::string middle = *(arr + middle_index);
	int n_size = size-1;
	
	//Construct two sub arrays for quick sort.
	std::string* a1 = new std::string[n_size];
	std::string* a2 = new std::string[n_size];
	int s1 = 0;
	int s2 = 0;
	
	//Loop through and sort the array into the two sub arrays.
	for(int i = 0; i < size; i++){
		if(i == middle_index) continue;
		std::string& val = arr[i];
		int cmp = strcmp(val.c_str(),middle.c_str());
		if(cmp > 0) {
			a2[s2++] = val;
		}
		else {
			a1[s1++] = val;
		}
	}
	
	//Quick sort our two sub arrays.
	quick_sort(a1, s1);
	quick_sort(a2, s2);
	
	//Merge the two sub arrays back into the array.
	{
		int k = 0;
		for(int i = 0; i < s1; i++){
			arr[k++] = a1[i];
		}
		arr[k++] = middle;
		for(int i = 0; i < s2; i++){
			arr[k++] = a2[i];
		}
	}
	
	//Cleanup our memory usage as we go.
	delete[] a1;
	delete[] a2;
}

int QuickSort(int nSize, std::istream& rInput, bool bAllowOutput) {

	//Attempt to create a buffer for quick sort.
	std::string* strings = new std::string[nSize];
	if(!strings) return ENOMEM; //Pointer not allocated, out of memory.
	
	//Read in our strings.
	{
		std::string* temp = strings;
		for(int i = 0; i < nSize; i++)
			rInput >> temp[i]; //Found this trick out when I was working with Audio buffers, indexing is too slow.
	}
	
	//Sort the array...
	quick_sort(strings, nSize);
	
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
	
	//Success!
	return 0;

}
