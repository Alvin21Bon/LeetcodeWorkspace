#include "../leetcode.h"

// ############################################################################################

enum BuildingSides {
	LEFT,
	RIGHT
};

struct BuildingCorner {
	int x;
	int y;
	enum BuildingSides side;
};

struct BuildingCorner buildingCorner(int x, int y, enum BuildingSides side)
{
	struct BuildingCorner buildingCorner = {
		.x = x,
		.y = y,
		.side = side
	};

	return buildingCorner;
}

int buildingCornerCompare(struct BuildingCorner corner1, struct BuildingCorner corner2)
{
	if (corner1.x < corner2.x) return 1;
	if (corner1.x == corner2.x) 
	{
		if (corner1.side == LEFT && corner2.side == RIGHT) return 1;
		if (corner1.side == corner2.side) 
		{
			if (corner1.y == corner2.y) return 0;
			if (corner1.side == LEFT && corner1.y > corner2.y) return 1;
			if (corner1.side == RIGHT && corner1.y < corner2.y) return 1;

			return -1;
		}

		return -1;
	}

	return -1;
}

// ############################################################################################

struct HeightInstance {
	int height;
	int numInstances;
};

struct HeightInstance heightInstance(int height)
{
	struct HeightInstance heightInstance = {
		.height = height,
		.numInstances = 1
	};

	return heightInstance;
}

int heightInstanceCompare(struct HeightInstance height1, struct HeightInstance height2)
{
	if (height1.height > height2.height) return 1;
	else if (height1.height == height2.height) return 0;
	else return -1;
}

// ############################################################################################

struct DynamicArrayPoint {
	struct BuildingCorner* array;
	long length;
	long capacity;
};

struct DynamicArrayPoint dynamicArrayPointInit(long initialCapacity)
{
	struct DynamicArrayPoint dynamicArray = {
		.array = malloc(initialCapacity * sizeof(struct BuildingCorner)),
		.length = 0,
		.capacity = initialCapacity
	};

	return dynamicArray;
}

void dynamicArrayPointAdd(struct DynamicArrayPoint* dynamicArray, struct BuildingCorner elementToAdd)
{
	if (dynamicArray->length == dynamicArray->capacity)
	{
		dynamicArray->capacity *= 2;
		dynamicArray->array = realloc(dynamicArray->array, dynamicArray->capacity * sizeof(struct BuildingCorner));
	}

	dynamicArray->array[dynamicArray->length] = elementToAdd;
	dynamicArray->length++;
}

void dynamicArrayPointSwap(struct DynamicArrayPoint* dynamicArray, long index1, long index2)
{
	if (index1 < 0 || index1 >= dynamicArray->length || index2 < 0 || index2 >= dynamicArray->length) return;

	struct BuildingCorner temp = dynamicArray->array[index2];
	dynamicArray->array[index2] = dynamicArray->array[index1];
	dynamicArray->array[index1] = temp;
}

void dynamicArrayPointDeleteEnd(struct DynamicArrayPoint* dynamicArray)
{
	dynamicArray->length--;
}

void dynamicArrayPointDestroy(struct DynamicArrayPoint* dynamicArray)
{
	free(dynamicArray->array);
	dynamicArray->length = 0;
	dynamicArray->capacity = 0;
}

struct DynamicArrayHeight {
	struct HeightInstance* array;
	long length;
	long capacity;
};

struct DynamicArrayHeight dynamicArrayHeightInit(long initialCapacity)
{
	struct DynamicArrayHeight dynamicArray = {
		.array = malloc(initialCapacity * sizeof(struct HeightInstance)),
		.length = 0,
		.capacity = initialCapacity
	};

	return dynamicArray;
}

void dynamicArrayHeightAdd(struct DynamicArrayHeight* dynamicArray, struct HeightInstance elementToAdd)
{
	if (dynamicArray->length == dynamicArray->capacity)
	{
		dynamicArray->capacity *= 2;
		dynamicArray->array = realloc(dynamicArray->array, dynamicArray->capacity * sizeof(struct HeightInstance));
	}

	dynamicArray->array[dynamicArray->length] = elementToAdd;
	dynamicArray->length++;
}

void dynamicArrayHeightSwap(struct DynamicArrayHeight* dynamicArray, long index1, long index2)
{
	if (index1 < 0 || index1 >= dynamicArray->length || index2 < 0 || index2 >= dynamicArray->length) return;

	struct HeightInstance temp = dynamicArray->array[index2];
	dynamicArray->array[index2] = dynamicArray->array[index1];
	dynamicArray->array[index1] = temp;
}

void dynamicArrayHeightDeleteEnd(struct DynamicArrayHeight* dynamicArray)
{
	dynamicArray->length--;
}

void dynamicArrayHeightDestroy(struct DynamicArrayHeight* dynamicArray)
{
	free(dynamicArray->array);
	dynamicArray->length = 0;
	dynamicArray->capacity = 0;
}

// ############################################################################################

struct HeapPoint {
	struct DynamicArrayPoint array;
};

struct HeapPoint heapPointInit(long initialCapcity)
{
	struct HeapPoint heap = {
		.array = dynamicArrayPointInit(initialCapcity),
	};

	return heap;
}

void heapPointDestroy(struct HeapPoint* heap) { dynamicArrayPointDestroy(&heap->array); }
long heapGetParentIdx(long childIdx) { return ((childIdx + 1) / 2) - 1; }
long heapGetLeftChildIdx(long parentIdx) { return 2 * parentIdx + 1; }
long heapGetRightChildIdx(long parentIdx) { return 2 * parentIdx + 2; }
struct BuildingCorner* heapPointPeek(struct HeapPoint* heap) { return heap->array.length > 0 ? heap->array.array : NULL; }
bool heapPointIsEmpty(struct HeapPoint* heap) { return heap->array.length == 0; }

void heapifyPoint(struct HeapPoint* heap, long rootIdx)
{
	long leftChildIdx = heapGetLeftChildIdx(rootIdx);
	long rightChildIdx = heapGetRightChildIdx(rootIdx);

	if (rootIdx < 0 || rootIdx >= heap->array.length) return;
	struct BuildingCorner* rootNode = &heap->array.array[rootIdx];
	struct BuildingCorner* leftChildNode = leftChildIdx >= 0 && leftChildIdx < heap->array.length ? &heap->array.array[leftChildIdx] : NULL;
	struct BuildingCorner* rightChildNode = rightChildIdx >= 0 && rightChildIdx < heap->array.length ? &heap->array.array[rightChildIdx] : NULL;

	bool isLeftChildHigherPriority = leftChildNode != NULL ? buildingCornerCompare(*leftChildNode, *rootNode) > 0: false;
	bool isRightChildHigherPriority = rightChildNode != NULL? buildingCornerCompare(*rightChildNode, *rootNode) > 0: false;

	long swapIdx;
	if (!isLeftChildHigherPriority && !isRightChildHigherPriority) return;
	else if (isLeftChildHigherPriority && !isRightChildHigherPriority) swapIdx = leftChildIdx;
	else if (!isLeftChildHigherPriority && isRightChildHigherPriority) swapIdx = rightChildIdx;
	else
	{
		bool isLeftChildHigherPriorityThanRightChild = buildingCornerCompare(*leftChildNode, *rightChildNode) > 0;
		swapIdx = isLeftChildHigherPriorityThanRightChild ? leftChildIdx : rightChildIdx;
	}

	dynamicArrayPointSwap(&heap->array, rootIdx, swapIdx);
	heapifyPoint(heap, swapIdx);
}

void heapPointInsert(struct HeapPoint* heap, struct BuildingCorner nodeToInsert)
{
	dynamicArrayPointAdd(&heap->array, nodeToInsert);

	long idxOfInsertedNode = heap->array.length - 1;
	long idxOfParent = heapGetParentIdx(idxOfInsertedNode);
	struct BuildingCorner* parentNode = idxOfParent >= 0 && idxOfParent < heap->array.length ? &heap->array.array[idxOfParent] : NULL;

	while (parentNode != NULL && buildingCornerCompare(nodeToInsert, *parentNode) > 0)
	{
		dynamicArrayPointSwap(&heap->array, idxOfInsertedNode, idxOfParent);

		idxOfInsertedNode = idxOfParent;
		idxOfParent = heapGetParentIdx(idxOfInsertedNode);
		parentNode = idxOfParent >= 0 && idxOfParent < heap->array.length ? &heap->array.array[idxOfParent] : NULL;
	}
}

long heapPointFind(struct HeapPoint* heap, struct BuildingCorner nodeToFind)
{
	for (long idx = 0; idx < heap->array.length; idx++)
	{
		if (buildingCornerCompare(nodeToFind, heap->array.array[idx]) == 0) return idx;
	}

	return -1;
}

void heapPointDelete(struct HeapPoint* heap, struct BuildingCorner nodeToDelete)
{
	long idxOfNodeToDelete = heapPointFind(heap, nodeToDelete);
	if (idxOfNodeToDelete == -1) return;

	dynamicArrayPointSwap(&heap->array, idxOfNodeToDelete, heap->array.length - 1);
	dynamicArrayPointDeleteEnd(&heap->array);
	heapifyPoint(heap, idxOfNodeToDelete);
}

struct HeapHeight {
	struct DynamicArrayHeight array;
};

struct HeapHeight heapHeightInit(long initialCapcity)
{
	struct HeapHeight heap = {
		.array = dynamicArrayHeightInit(initialCapcity),
	};

	return heap;
}

void heapHeightDestroy(struct HeapHeight* heap) { dynamicArrayHeightDestroy(&heap->array); }
struct HeightInstance* heapHeightPeek(struct HeapHeight* heap) { return heap->array.length > 0 ? heap->array.array : NULL; }
bool heapHeightIsEmpty(struct HeapHeight* heap) { return heap->array.length == 0; }

void heapifyHeight(struct HeapHeight* heap, long rootIdx)
{
	long leftChildIdx = heapGetLeftChildIdx(rootIdx);
	long rightChildIdx = heapGetRightChildIdx(rootIdx);

	if (rootIdx < 0 || rootIdx >= heap->array.length) return;
	struct HeightInstance* rootNode = &heap->array.array[rootIdx];
	struct HeightInstance* leftChildNode = leftChildIdx >= 0 && leftChildIdx < heap->array.length ? &heap->array.array[leftChildIdx] : NULL;
	struct HeightInstance* rightChildNode = rightChildIdx >= 0 && rightChildIdx < heap->array.length ? &heap->array.array[rightChildIdx] : NULL;

	bool isLeftChildHigherPriority = leftChildNode != NULL ? heightInstanceCompare(*leftChildNode, *rootNode) > 0: false;
	bool isRightChildHigherPriority = rightChildNode != NULL? heightInstanceCompare(*rightChildNode, *rootNode) > 0: false;

	long swapIdx;
	if (!isLeftChildHigherPriority && !isRightChildHigherPriority) return;
	else if (isLeftChildHigherPriority && !isRightChildHigherPriority) swapIdx = leftChildIdx;
	else if (!isLeftChildHigherPriority && isRightChildHigherPriority) swapIdx = rightChildIdx;
	else
	{
		bool isLeftChildHigherPriorityThanRightChild = heightInstanceCompare(*leftChildNode, *rightChildNode) > 0;
		swapIdx = isLeftChildHigherPriorityThanRightChild ? leftChildIdx : rightChildIdx;
	}

	dynamicArrayHeightSwap(&heap->array, rootIdx, swapIdx);
	heapifyHeight(heap, swapIdx);
}

void heapHeightInsert(struct HeapHeight* heap, struct HeightInstance nodeToInsert)
{
	dynamicArrayHeightAdd(&heap->array, nodeToInsert);

	long idxOfInsertedNode = heap->array.length - 1;
	long idxOfParent = heapGetParentIdx(idxOfInsertedNode);
	struct HeightInstance* parentNode = idxOfParent >= 0 && idxOfParent < heap->array.length ? &heap->array.array[idxOfParent] : NULL;

	while (parentNode != NULL && heightInstanceCompare(nodeToInsert, *parentNode) > 0)
	{
		dynamicArrayHeightSwap(&heap->array, idxOfInsertedNode, idxOfParent);

		idxOfInsertedNode = idxOfParent;
		idxOfParent = heapGetParentIdx(idxOfInsertedNode);
		parentNode = idxOfParent >= 0 && idxOfParent < heap->array.length ? &heap->array.array[idxOfParent] : NULL;
	}
}

long heapHeightFind(struct HeapHeight* heap, struct HeightInstance nodeToFind)
{
	for (long idx = 0; idx < heap->array.length; idx++)
	{
		if (heightInstanceCompare(nodeToFind, heap->array.array[idx]) == 0) return idx;
	}

	return -1;
}

void heapHeightDelete(struct HeapHeight* heap, struct HeightInstance nodeToDelete)
{
	long idxOfNodeToDelete = heapHeightFind(heap, nodeToDelete);
	if (idxOfNodeToDelete == -1) return;

	dynamicArrayHeightSwap(&heap->array, idxOfNodeToDelete, heap->array.length - 1);
	dynamicArrayHeightDeleteEnd(&heap->array);
	heapifyHeight(heap, idxOfNodeToDelete);
}
// #############################################################3

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** getSkyline(int** buildings, int buildingsSize, int* buildingsColSize, int* returnSize, int** returnColumnSizes) 
{
	struct HeapPoint buildingCornerHeap = heapPointInit(buildingsSize * 2);
	for (int idx = 0; idx < buildingsSize; idx++)
	{
		struct BuildingCorner buildingCornerLeft = buildingCorner(buildings[idx][0], buildings[idx][2], LEFT);
		struct BuildingCorner buildingCornerRight = buildingCorner(buildings[idx][1], buildings[idx][2], RIGHT);

		heapPointInsert(&buildingCornerHeap, buildingCornerLeft);
		heapPointInsert(&buildingCornerHeap, buildingCornerRight);
	}


	int** skyline = malloc(buildingsSize * 2 * sizeof(*skyline));
	*returnSize = 0; // will be updated as we add points to the array
	*returnColumnSizes = malloc(buildingsSize * 2 * sizeof(int));

	struct HeapHeight heightInstanceHeap = heapHeightInit(buildingsSize);
	while (!heapPointIsEmpty(&buildingCornerHeap))
	{
		struct BuildingCorner cornerPoint = *heapPointPeek(&buildingCornerHeap);
		heapPointDelete(&buildingCornerHeap, cornerPoint);

		struct HeightInstance heightItem = heightInstance(cornerPoint.y);
		struct HeightInstance* greatestHeightInstance = heapHeightPeek(&heightInstanceHeap);
		int heightItemToGreatestInHeapCompare = greatestHeightInstance != NULL ? heightInstanceCompare(heightItem, *greatestHeightInstance) : 1;

		long idxOfHeightItemIfInHeap = heapHeightFind(&heightInstanceHeap, heightItem);
		struct HeightInstance* heightItemInHeap = idxOfHeightItemIfInHeap != -1 ? &heightInstanceHeap.array.array[idxOfHeightItemIfInHeap] : NULL;

		// manipulating height heap
		if (heightItemInHeap)
		{
			if (cornerPoint.side == LEFT) heightItemInHeap->numInstances++;
			else if (cornerPoint.side == RIGHT) heightItemInHeap->numInstances--;

			if (heightItemInHeap->numInstances == 0) heapHeightDelete(&heightInstanceHeap, *heightItemInHeap);
		}
		else
		{
			heapHeightInsert(&heightInstanceHeap, heightItem);
		}

		// invalid point checks
		bool isPointValid = true;
		if (heightItemToGreatestInHeapCompare < 0) isPointValid = false;
		else if (heightItemToGreatestInHeapCompare == 0)
		{
			if (cornerPoint.side == LEFT) isPointValid = false;
			else if (cornerPoint.side == RIGHT)
			{
				// if greatest height in heap is still the same height, then this point is invalid
				struct HeightInstance* currentGreatestHeightInstance = heapHeightPeek(&heightInstanceHeap);
				if (currentGreatestHeightInstance != NULL && heightInstanceCompare(heightItem, *currentGreatestHeightInstance) == 0) isPointValid = false;
			}
		}

		// adding new point to return array if it is valid
		if (isPointValid)
		{
			skyline[*returnSize] = malloc(2 * sizeof(int));
			skyline[*returnSize][0] = cornerPoint.x;
			skyline[*returnSize][1] = cornerPoint.y;
			if (cornerPoint.side == RIGHT) skyline[*returnSize][1] = heapHeightIsEmpty(&heightInstanceHeap) ? 0 : heapHeightPeek(&heightInstanceHeap)->height;

			(*returnColumnSizes)[*returnSize] = 2;
			(*returnSize)++;
		}
	}

	skyline = realloc(skyline, *returnSize * sizeof(*skyline));
	*returnColumnSizes = realloc(*returnColumnSizes, *returnSize * sizeof(int));
	heapPointDestroy(&buildingCornerHeap);
	heapHeightDestroy(&heightInstanceHeap);

	return skyline;
}

