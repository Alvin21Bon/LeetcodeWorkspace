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

void minHeapDestroy(MinHeap* heap)
{
	arrayDestroy(heap);
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
	int leftChildIdx = heapGetLeftChildIdx(rootIdx);
	int rightChildIdx = heapGetRightChildIdx(rootIdx);
	int rootValue = heap->array[rootIdx];
	int* leftChild = heapGetLeftChild(heap, rootIdx);
	int* rightChild = heapGetRightChild(heap, rootIdx);

	int leftChildDifference = 0;
	int rightChildDifference = 0;
	if (leftChild != NULL) leftChildDifference = rootValue - *leftChild;
	if (rightChild != NULL) rightChildDifference = rootValue - *rightChild;

	int swapIdx;
	if (leftChildDifference <= 0 && rightChildDifference <= 0) return;
	else if (leftChildDifference >= rightChildDifference) swapIdx = leftChildIdx;
	else swapIdx = rightChildIdx;

	arraySwap(heap, rootIdx, swapIdx);
	minHeapify(heap, swapIdx);
}

void buildMinHeap(MinHeap* heap)
{
	int lastNonLeafNodeIdx = heapGetParentIdx(heap->length - 1);
	for (int subTreeRootIdx = lastNonLeafNodeIdx; subTreeRootIdx >= 0; subTreeRootIdx--)
	{
		minHeapify(heap, subTreeRootIdx);
	}
}

void heapInsert(MinHeap* heap, int numToInsert)
{
	arrayAdd(heap, numToInsert);

	int insertedNodeIdx = heap->length - 1;
	int currentParentIdx = heapGetParentIdx(insertedNodeIdx);
	int* currentParent = heapGetParent(heap, insertedNodeIdx);

	while (currentParent != NULL && *currentParent > numToInsert)
	{
		arraySwap(heap, insertedNodeIdx, currentParentIdx);

		insertedNodeIdx = currentParentIdx;
		currentParentIdx = heapGetParentIdx(insertedNodeIdx);
		currentParent = heapGetParent(heap, insertedNodeIdx);
	}
}

int heapFind(MinHeap* heap, int numToFind)
{
	for (int i = 0; i < heap->length; i++)
	{
		if (heap->array[i] == numToFind) return i;
	}

	return -1;
}

void heapDelete(MinHeap* heap, int numToDelete)
{
	int idxOfNodeToDelete = heapFind(heap, numToDelete);
	if (idxOfNodeToDelete == -1) return;

	arraySwap(heap, idxOfNodeToDelete, heap->length - 1);
	arrayDeleteEnd(heap);
	minHeapify(heap, idxOfNodeToDelete);
}

// #############################################################

struct NumToIdxHash {
	int numKey;
	MinHeap indexMinHeap;
	UT_hash_handle hh;
};

struct NumToIdxHash* numToIdxHashCreate(int numKey)
{
	struct NumToIdxHash* hash = malloc(sizeof(*hash));
	hash->numKey = numKey;
	hash->indexMinHeap = minHeapInit();

	return hash;
}

void numToIdxHashTableDestroy(struct NumToIdxHash** hashTable)
{
	struct NumToIdxHash *currentItem, *tmpItem;
	HASH_ITER(hh, *hashTable, currentItem, tmpItem)
	{
		HASH_DEL(*hashTable, currentItem);
		minHeapDestroy(&currentItem->indexMinHeap);
		free(currentItem);
	}
}

struct IdxToNumHash {
	int idxKey;
	int num;
	UT_hash_handle hh;
};

struct IdxToNumHash* idxToNumHashCreate(int idxKey)
{
	struct IdxToNumHash* hash = malloc(sizeof(*hash));
	hash->idxKey = idxKey;

	return hash;
}

void idxToNumHashTableDestroy(struct IdxToNumHash** hashTable)
{
	struct IdxToNumHash *currentItem, *tmpItem;
	HASH_ITER(hh, *hashTable, currentItem, tmpItem)
	{
		HASH_DEL(*hashTable, currentItem);
		free(currentItem);
	}
}

// #############################################################

typedef struct {
	struct NumToIdxHash* numToIdxHashTable;
	struct IdxToNumHash* idxToNumHashTable;
} NumberContainers;


NumberContainers* numberContainersCreate() 
{
	NumberContainers* numberContainer = malloc(sizeof(*numberContainer));
	numberContainer->numToIdxHashTable = NULL;
	numberContainer->idxToNumHashTable = NULL;

	return numberContainer;
}

void numberContainersChange(NumberContainers* obj, int index, int number) 
{
	struct NumToIdxHash* numToIdxHash;
	struct IdxToNumHash* idxToNumHash;

	HASH_FIND_INT(obj->idxToNumHashTable, &index, idxToNumHash);
	if (idxToNumHash == NULL)
	{
		idxToNumHash = idxToNumHashCreate(index);
		idxToNumHash->num = number;
		HASH_ADD_INT(obj->idxToNumHashTable, idxKey, idxToNumHash);
	}
	else
	{
		if (idxToNumHash->num == number) return;
		idxToNumHash->num = number;
	}

	HASH_FIND_INT(obj->numToIdxHashTable, &number, numToIdxHash);
	if (numToIdxHash == NULL)
	{
		numToIdxHash = numToIdxHashCreate(number);
		HASH_ADD_INT(obj->numToIdxHashTable, numKey, numToIdxHash);
	}

	heapInsert(&numToIdxHash->indexMinHeap, index);
}

int numberContainersFind(NumberContainers* obj, int number) 
{
	struct NumToIdxHash* numToIdxHash;
	HASH_FIND_INT(obj->numToIdxHashTable, &number, numToIdxHash);

	if (numToIdxHash == NULL) return -1;
	while (numToIdxHash->indexMinHeap.length > 0)
	{
		int idxToCheck = numToIdxHash->indexMinHeap.array[0];
		struct IdxToNumHash* idxToNumHash;
		HASH_FIND_INT(obj->idxToNumHashTable, &idxToCheck, idxToNumHash);

		if (idxToNumHash->num == number) return idxToCheck;
		heapDelete(&numToIdxHash->indexMinHeap, idxToCheck);
	}

	HASH_DEL(obj->numToIdxHashTable, numToIdxHash);
	minHeapDestroy(&numToIdxHash->indexMinHeap);
	free(numToIdxHash);

	return -1;
}

void numberContainersFree(NumberContainers* obj) 
{
	numToIdxHashTableDestroy(&obj->numToIdxHashTable);
	idxToNumHashTableDestroy(&obj->idxToNumHashTable);
	free(obj);
}

