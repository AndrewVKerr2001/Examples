#include <iostream>

struct HeapEntry {
	HeapEntry* parent;
	HeapEntry* left;
	HeapEntry* right;
	int key;
};

struct Heap {
	HeapEntry* root;
	HeapEntry* last_parent;
};

void QuickSort(int *array, int size) {
	//Recursion inbound, base case is stated.
	if(!size || !array) return;
	
	//Pick our "middle" value.
	int middle_index = 0;
	int middle = *array;
	int n_size = size-1;
	
	//Construct two sub arrays for quick sort.
	int* a1 = new int[n_size];
	int* a2 = new int[n_size];
	int s1 = 0;
	int s2 = 0;
	
	//Loop through and sort the array into the two sub arrays.
	for(int i = 0; i < size; i++){
		if(middle_index == i) continue;
		if(array[i] > middle){
			a2[s2++] = array[i];
		}else{
			a1[s1++] = array[i];
		}
	}
	
	//Quick sort our two sub arrays.
	QuickSort(a1, s1);
	QuickSort(a2, s2);
	
	//Merge the two sub arrays back into the array.
	int k = 0;
	for(int i = 0; i < s1; i++){
		array[k++] = a1[i];
	}
	array[k++] = middle;
	for(int i = 0; i < s2; i++){
		array[k++] = a2[i];
	}
	
	//Cleanup our memory usage as we go.
	delete a1;
	delete a2;
}

void UpHeap(HeapEntry* child) {
	if(!child->parent) return;
	if(child->parent->key < child->key){
		int temp = child->parent->key;
		child->parent->key = child->key;
		child->key = temp;
		UpHeap(child->parent);
	}
}

void InsertNode(Heap& heap, int key) {
	if(!heap.last_parent) {
		if(!heap.root) {
			heap.root = new HeapEntry();
			heap.root->key = key;
			heap.root->parent = NULL;
			heap.root->left = NULL;
			heap.root->right = NULL;
			heap.last_parent = heap.root;
		}
		else
			std::cout << "Unknown situation when inserting node!" << std::endl;
	}else{
		HeapEntry* temp;
		if(heap.last_parent->left == NULL) {
			temp = heap.last_parent->left = new HeapEntry();
			temp->key = key;
			temp->left = NULL;
			temp->right = NULL;
			temp->parent = heap.last_parent;
		}
		else if(heap.last_parent->right == NULL) {
			temp = heap.last_parent->right = new HeapEntry();
			temp->key = key;
			temp->left = NULL;
			temp->right = NULL;
			temp->parent = heap.last_parent;
		}
		heap.last_parent = temp;
		UpHeap(temp);
	}
}

void PrintHeapEntry(HeapEntry* entry) {
	if(!entry) return;
	if(entry->left) PrintHeapEntry(entry->left);
	std::cout << entry->key << ", ";
	if(entry->right) PrintHeapEntry(entry->right);
}

void PrintHeap(Heap& heap) {
	PrintHeapEntry(heap.root);
}

void Print(int* array, int size){
	for(int i = 0; i < size; i++)
		std::cout << *array++ << (i+1 < size ? "," : "\n");
}

int main(){

	std::cout << "QuickSort: " << std::endl;
	{
		int array[8] = {7,10,3,5,14,701,4,8};
		std::cout << "Unsorted: " << std::endl;
		Print(array,8);
		
		QuickSort(array,8);
		
		std::cout << "Sorted:" << std::endl;
		Print(array,8);
	}
	std::cout << std::endl;
	
	std::cout << "Heap: " << std::endl;
	{
		Heap heap;
		heap.root = NULL;
		heap.last_parent = NULL;
		InsertNode(heap, 10);
		InsertNode(heap, 12);
		InsertNode(heap, 18);
		PrintHeap(heap);
		std::cout << std::endl;
		
		InsertNode(heap, 9);
		PrintHeap(heap);
	}
	std::cout << std::endl;

	return 0;
}
