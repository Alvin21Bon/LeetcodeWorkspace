#include "leetcode.h"

struct DynamicArray {
	int* array;
	int length;
	int allocatedLength;
};

struct DynamicArray dynamicArray() 
{
	const int INITIAL_SIZE = 64;
	struct DynamicArray array;
	array.array = malloc(INITIAL_SIZE * sizeof(int));
	array.allocatedLength = INITIAL_SIZE;
	array.length = 0;

	return array;
}

void arrayAdd(struct DynamicArray* array, const int num)
{
	if (array->length == array->allocatedLength)
	{
		array->allocatedLength *= 2;
		array->array = realloc(array->array, array->allocatedLength * sizeof(int));
	}

	array->array[array->length] = num;
	array->length++;
}

void arrayDeleteEnd(struct DynamicArray* array)
{
	array->length--;
}

void arraySwap(struct DynamicArray* array, const int idx1, const int idx2)
{
	int temp = array->array[idx1];
	array->array[idx1] = array->array[idx2];
	array->array[idx2] = temp;
}

void arrayDestroy(struct DynamicArray* array)
{
	free(array->array);
	array->length = -1;
	array->allocatedLength = 0;
}

// #############################################################

typedef struct DynamicArray MinHeap;

MinHeap minHeapInit()
{
	return dynamicArray();
}

int heapGetParentIdx(int childIdx) { return ((childIdx + 1) / 2) - 1; }
int heapGetLeftChildIdx(int parentIdx) { return 2 * parentIdx + 1; }
int heapGetRightChildIdx(int parentIdx) { return 2 * parentIdx + 2; }
int* heapGetParent(MinHeap* heap, int childIdx)
{
	int parentIdx = heapGetParentIdx(childIdx);
	if (parentIdx < 0) return NULL;

	return &heap->array[parentIdx];
}
int* heapGetLeftChild(MinHeap* heap, int parentIdx)
{
	int childIdx = heapGetLeftChildIdx(parentIdx);
	if (childIdx >= heap->length) return NULL;

	return &heap->array[childIdx];
}
int* heapGetRightChild(MinHeap* heap, int parentIdx)
{
	int childIdx = heapGetRightChildIdx(parentIdx);
	if (childIdx >= heap->length) return NULL;

	return &heap->array[childIdx];
}

void minHeapify(MinHeap* heap, int rootIdx)
{
	int* leftChild = heapGetLeftChild(heap, rootIdx);
	int* rightChild = heapGetRightChild(heap, rootIdx);
	int rootValue = heap->array[rootIdx];
	while (leftChild != NULL && rightChild != NULL)
	{
		int swapIdx;
		int lowestValue;
	} 
}

void buildMinHeap(MinHeap* heap)
{

}

typedef struct {
    
} NumberContainers;


NumberContainers* numberContainersCreate() 
{
    
}

void numberContainersChange(NumberContainers* obj, int index, int number) 
{
    
}

int numberContainersFind(NumberContainers* obj, int number) 
{
    
}

void numberContainersFree(NumberContainers* obj) {
    
}

/**
 * Your NumberContainers struct will be instantiated and called as such:
 * NumberContainers* obj = numberContainersCreate();
 * numberContainersChange(obj, index, number);
 
 * int param_2 = numberContainersFind(obj, number);
 
 * numberContainersFree(obj);
*/
