#include "HeapSort.hpp"

void siftDown(std::string* arr, int start, int end);
void heapify(std::string* arr, int size);

void heapsort(std::string* arr, int size) {
	heapify(arr, size);
	
	int end = size - 1;
	while(end > 0){
		std::string temp = arr[end];
		arr[end] = arr[0];
		arr[0] = temp;
		
		end -= 1;
		
		siftDown(arr, 0, end);
	}

}

void heapify(std::string* arr, int size) {
	int start = (size - 1) / 2;
	
	while(start >= 0) {
		siftDown(arr, start, size - 1);
		start -= 1;
	}
}

void siftDown(std::string* arr, int start, int end) {
	int root = start;
	while ((2 * root + 1) <= end) {
		int child = (2 * root + 1);
		int swap = root;
		if(arr[swap] < arr[child])
			swap = child;
			
		if(child+1 <= end && arr[swap] < arr[child+1])
			swap = child + 1;
			
		if(swap == root)
			return;
			
		{//Actual swap...
			std::string temp = arr[root];
			arr[root] = arr[swap];
			arr[swap] = temp;
			root = swap;
		}
	}
}

int HeapSort(int nSize, std::istream& rInput, bool bAllowOutput) {
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
	heapsort(strings, nSize);
	
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
