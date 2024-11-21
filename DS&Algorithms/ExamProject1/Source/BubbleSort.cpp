#include "BubbleSort.hpp"

void bubble_sort(int nSize, std::string* pArr){
    //Check for special cases.
    if(nSize <= 1 || !pArr) return;

    //Lets begin out loop!
    bool bSorted;
    std::string temp;
    do {
        bSorted = true; //Reset bSorted so we can itterate.
        for(int i = 0; i < nSize-1; i++){
            if(strcmp(pArr[i],pArr[i+1]) > 0){
                temp = pArr[i+1];
                pArr[i+1] = pArr[i];
                pArr[i] = temp;
                bSorted = false;
                break;
            }
        }
    } while(!bSorted);
}

int BubbleSort(int nSize, std::ifstream& rInput, bool bAllowOutput){
    
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
	bubble_sort(nSize, strings);
	
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
        if(!bOffender) std::cout << "Correct sort!!!!" << std::endl;
    }
#endif
	
	//Deallocate out memory.
	delete[] strings;
	
	//Success!
	return 0;

}