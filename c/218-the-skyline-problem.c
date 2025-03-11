#include "leetcode.h"

struct DynamicArray {
	void* array;
	const size_t sizeOfType;
	long length;
	long capacity;
};

struct DynamicArray dynamicArrayInit(const size_t sizeOfType, const long initialCapacity)
{
	struct DynamicArray dynamicArray = {
		.array = malloc(initialCapacity * sizeOfType),
		.sizeOfType = sizeOfType,
		.length = 0,
		.capacity = initialCapacity
	};

	return dynamicArray;
}

void dynamicArraySet(struct DynamicArray* const dynamicArray, const long index, const void* const elementToSet)
{
	if (index < 0 || index >= dynamicArray->capacity) return;

	memcpy((char*)dynamicArray->array + (index * dynamicArray->sizeOfType), elementToSet, dynamicArray->sizeOfType);
}

void dynamicArrayAdd(struct DynamicArray* const dynamicArray, const void* const elementToAdd)
{
	if (dynamicArray->length == dynamicArray->capacity)
	{
		dynamicArray->capacity *= 2;
		dynamicArray->array = realloc(dynamicArray->array, dynamicArray->capacity * dynamicArray->sizeOfType);
	}

	dynamicArraySet(dynamicArray, dynamicArray->length, elementToAdd);
	dynamicArray->length++;
}

void* dynamicArrayGet(const struct DynamicArray* const dynamicArray, const long index)
{
	if (index < 0 || index >= dynamicArray->length) return NULL;
	return (char*)dynamicArray->array + (index * dynamicArray->sizeOfType);
}

void dynamicArraySwap(struct DynamicArray* const dynamicArray, const long index1, const long index2)
{
	void* index1Ptr = dynamicArrayGet(dynamicArray, index1);
	void* index2Ptr = dynamicArrayGet(dynamicArray, index2);
	if (index1Ptr == NULL || index2Ptr == NULL) return;

	char temp[dynamicArray->sizeOfType];
	memcpy(temp, index2Ptr, dynamicArray->sizeOfType);

	dynamicArraySet(dynamicArray, index2, index1Ptr);
	dynamicArraySet(dynamicArray, index1, temp);
}

void dynamicArrayDeleteEnd(struct DynamicArray* const dynamicArray)
{
	dynamicArray->length--;
}

void dynamicArrayDestroy(struct DynamicArray* const dynamicArray)
{
	free(dynamicArray->array);
	dynamicArray->length = 0;
	dynamicArray->capacity = 0;
}

// ############################################################################################

struct Heap {
	struct DynamicArray array;

	// >0: first parameter is higher in priority than second parameter
	// 0: both parameters are equal
	// <0: second parameter is higher in priority than first parameter
	int (*const compare)(const void* const, const void* const);
};

struct Heap heapInit(const size_t sizeOfType, const long initialCapcity, int (*const compareFunction)(const void* const, const void* const))
{
	struct Heap heap = {
		.array = dynamicArrayInit(sizeOfType, initialCapcity),
		.compare = compareFunction,
	};

	return heap;
}

void heapDestroy(struct Heap* const heap) { dynamicArrayDestroy(&heap->array); }
long heapGetParentIdx(const long childIdx) { return ((childIdx + 1) / 2) - 1; }
long heapGetLeftChildIdx(const long parentIdx) { return 2 * parentIdx + 1; }
long heapGetRightChildIdx(const long parentIdx) { return 2 * parentIdx + 2; }
void* heapPeek(const struct Heap* const heap) { return heap->array.length > 0 ? heap->array.array : NULL; }
bool heapIsEmpty(const struct Heap* const heap) { return heap->array.length == 0; }

void heapify(struct Heap* const heap, const long rootIdx)
{
	const long leftChildIdx = heapGetLeftChildIdx(rootIdx);
	const long rightChildIdx = heapGetRightChildIdx(rootIdx);

	const void* const rootNode = dynamicArrayGet(&heap->array, rootIdx);
	if (rootNode == NULL) return;
	const void* const leftChildNode = dynamicArrayGet(&heap->array, leftChildIdx);
	const void* const rightChildNode = dynamicArrayGet(&heap->array, rightChildIdx);

	bool isLeftChildHigherPriority = leftChildNode != NULL ? heap->compare(leftChildNode, rootNode) > 0: false;
	bool isRightChildHigherPriority = rightChildNode != NULL? heap->compare(rightChildNode, rootNode) > 0: false;

	long swapIdx;
	if (!isLeftChildHigherPriority && !isRightChildHigherPriority) return;
	else if (isLeftChildHigherPriority && !isRightChildHigherPriority) swapIdx = leftChildIdx;
	else if (!isLeftChildHigherPriority && isRightChildHigherPriority) swapIdx = rightChildIdx;
	else
	{
		bool isLeftChildHigherPriorityThanRightChild = heap->compare(leftChildNode, rightChildNode) > 0;
		swapIdx = isLeftChildHigherPriorityThanRightChild ? leftChildIdx : rightChildIdx;
	}

	dynamicArraySwap(&heap->array, rootIdx, swapIdx);
	heapify(heap, swapIdx);
}

void heapInsert(struct Heap* const heap, const void* const nodeToInsert)
{
	dynamicArrayAdd(&heap->array, nodeToInsert);

	long idxOfInsertedNode = heap->array.length - 1;
	long idxOfParent = heapGetParentIdx(idxOfInsertedNode);
	void* parentNode = dynamicArrayGet(&heap->array, idxOfParent);

	while (parentNode != NULL && heap->compare(nodeToInsert, parentNode) > 0)
	{
		dynamicArraySwap(&heap->array, idxOfInsertedNode, idxOfParent);

		idxOfInsertedNode = idxOfParent;
		idxOfParent = heapGetParentIdx(idxOfInsertedNode);
		parentNode = dynamicArrayGet(&heap->array, idxOfParent);
	}
}

long heapFind(const struct Heap* const heap, const void* const nodeToFind)
{
	long idx = 0;
	const void* ptr = heap->array.array;
	for ( ; idx < heap->array.length; idx++, ptr = (char*)ptr + heap->array.sizeOfType)
	{
		if (heap->compare(nodeToFind, ptr) == 0) return idx;
	}

	return -1;
}

void heapDelete(struct Heap* const heap, const void* const nodeToDelete)
{
	const long idxOfNodeToDelete = heapFind(heap, nodeToDelete);
	if (idxOfNodeToDelete == -1) return;

	dynamicArraySwap(&heap->array, idxOfNodeToDelete, heap->array.length - 1);
	dynamicArrayDeleteEnd(&heap->array);
	heapify(heap, idxOfNodeToDelete);
}

// ############################################################################################

enum BuildingSides {
	LEFT,
	RIGHT
};

union BuildingCorner {
	const int e[2];
	struct {
		const int x;
		const int y;
		const enum BuildingSides side;
	};
};

union BuildingCorner buildingCorner(const int x, const int y, const enum BuildingSides side)
{
	union BuildingCorner buildingCorner = {
		.x = x,
		.y = y,
		.side = side
	};

	return buildingCorner;
}

int buildingCornerCompare(const void* const corner1, const void* const corner2)
{
	const union BuildingCorner* const castedCorner1 = (const union BuildingCorner* const)corner1;
	const union BuildingCorner* const castedCorner2 = (const union BuildingCorner* const)corner2;

	if (castedCorner1->x < castedCorner2->x) return 1;
	else if (castedCorner1->x == castedCorner2->x) 
	{
		if (castedCorner1->y > castedCorner2->y) return 1;
		else if (castedCorner1->y == castedCorner2->y) return 0;
		else return -1;
	}
	else return -1;
}

// ############################################################################################

struct HeightInstance {
	const int height;
	int numInstances;
};

struct HeightInstance heightInstance(const int height)
{
	struct HeightInstance heightInstance = {
		.height = height,
		.numInstances = 1
	};

	return heightInstance;
}

int heightInstanceCompare(const void* const height1, const void* const height2)
{
	const struct HeightInstance* const castedHeight1 = (const struct HeightInstance* const)height1;
	const struct HeightInstance* const castedHeight2 = (const struct HeightInstance* const)height2;

	if (castedHeight1->height > castedHeight2->height) return 1;
	else if (castedHeight1->height == castedHeight2->height) return 0;
	else return -1;
}

// ############################################################################################

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** getSkyline(int** buildings, int buildingsSize, int* buildingsColSize, int* returnSize, int** returnColumnSizes) 
{
	struct Heap buildingCornerHeap = heapInit(sizeof(union BuildingCorner), buildingsSize * 2, buildingCornerCompare);
	for (int idx = 0; idx < buildingsSize; idx++)
	{
		union BuildingCorner buildingCornerLeft = buildingCorner(buildings[idx][0], buildings[idx][2], LEFT);
		union BuildingCorner buildingCornerRight = buildingCorner(buildings[idx][1], buildings[idx][2], RIGHT);

		heapInsert(&buildingCornerHeap, &buildingCornerLeft);
		heapInsert(&buildingCornerHeap, &buildingCornerRight);
	}

	*returnColumnSizes = malloc(sizeof(int));
	**returnColumnSizes = 2;

	int** skyline = malloc(buildingsSize * 2 * sizeof(*skyline));
	*returnSize = 0; // will be updated as we add points to the array
	
	struct Heap heightInstanceHeap = heapInit(sizeof(struct HeightInstance), buildingsSize, heightInstanceCompare);
	while (!heapIsEmpty(&buildingCornerHeap))
	{
		union BuildingCorner cornerPoint = *(union BuildingCorner*)(heapPeek(&buildingCornerHeap));
		heapDelete(&buildingCornerHeap, &cornerPoint);

		struct HeightInstance lookup = heightInstance(cornerPoint.y);
		long idxOfHeightItem = heapFind(&heightInstanceHeap, &lookup);
		struct HeightInstance* heightItem = (struct HeightInstance*)(dynamicArrayGet(&heightInstanceHeap.array, idxOfHeightItem));

		struct HeightInstance* topHeightHeapItem = *(struct HeightInstance*)(heapPeek(&heightInstanceHeap));
		bool isValid;
		if (heightItem)
		{
		}
		else
		{
			// guaranteed to be a left side point

		}
	}
}
