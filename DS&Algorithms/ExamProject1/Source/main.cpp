#include "Common.hpp"
#include "QuickSort.hpp"
#include "BubbleSort.hpp"
#include "MergeSort.hpp"
#include "HeapSort.hpp"

#include <vector>
#include <algorithm>

static bool bAllowLog = false;

int strcmp(const std::string& s1, const std::string& s2){
	return strcmp(s1.c_str(), s2.c_str());
}

void usage(const char* filename) {
	std::cout << "Usage: " << filename << " input-file algorithm [-print]" << std::endl;
}

int get_algo(const char* str) {
	if(!strcmp(str, "bubble")) 
		return 0;
	if(!strcmp(str, "merge")) 
		return 1;
	if(!strcmp(str, "quick")) 
		return 2;
	if(!strcmp(str, "heap")) 
		return 3;
	if(!strcmp(str, "sys")) 
		return 4;
	return -1;
	
}

int main(int argc, char *argv[]){
	if(argc < 3){
		usage(argv[0]);
		return -1;
	}else if(argc >= 4) {
		if(strcmp(argv[3],"-print") == 0) {
			bAllowLog = true;
		}
	}
	
	int algo;
		
	if((algo = get_algo(argv[2])) != -1){
		std::ifstream file(argv[1]);
		if(!file.is_open()) {
			#ifdef __EXTRA_OUTPUT__
			if(bAllowLog) std::cout << "Invalid Argument: Expected a valid filepath to a valid file for first argument.\n";
			#endif
			return ENOENT;
		}
		
		int size;
		file >> size;
		if(!file) return EIO;

		switch(algo){
		case 0: //Bubble
			return BubbleSort(size, file, bAllowLog);
		case 1: //Merge
			return MergeSort(size, file, bAllowLog);
		case 2: //Quick
			return QuickSort(size, file, bAllowLog);
		case 3: //Heap
			return HeapSort(size, file, bAllowLog);
		case 4: //Sys
			{
				std::vector<std::string> vect(size);
				for(int i = 0; i < size; i++)
					file >> vect[i];

				std::sort(vect.begin(), vect.end());

				if(bAllowLog)
					for(int i = 0; i < size; i++)
						std::cout << vect[i] << std::endl;
			}
			break;
		}
		
	}
	else {
		#ifdef __EXTRA_OUTPUT__
		if(bAllowLog) std::cout << "Invalid Argument: Expected 'bubble', 'merge', 'quick', 'heap' or 'sys' for second argument.\n";
		#endif
		return EINVAL;
	}
	
	return 0;
}
