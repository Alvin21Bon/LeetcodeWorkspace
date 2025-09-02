#include "leetcode.h"

#define MAX_POINTS 10000

struct HeapNode {
	double distance;
	int index;
};

struct MinHeap {
	struct HeapNode distances[MAX_POINTS];
	int size;
};

struct MinHeap createHeap()
{
	struct MinHeap heap;
	heap.size = 0;
	return heap;
}

double getDistance(double x, double y)
{
	return sqrt((x * x) + (y * y));
}

void swap(struct MinHeap* heap, int idx1, int idx2)
{
	struct HeapNode temp = heap->distances[idx1];
	heap->distances[idx1] = heap->distances[idx2];
	heap->distances[idx2] = temp;
}

int getParentIdx(int currentIdx)
{
	return ((currentIdx + 1) / 2) - 1;
}

bool isIdxInvalid(struct MinHeap* heap, int idx)
{
	return idx < 0 || idx >= heap->size;
}

void insert(struct MinHeap* heap, int x, int y, int index)
{
	double distanceFromOrigin = getDistance(x, y);
	struct HeapNode node = {
		.distance = distanceFromOrigin,
		.index = index
	};

	int currentIdx = heap->size;
	heap->distances[heap->size++] = node;

	int currentParentIdx = getParentIdx(currentIdx);
	double parent;
	while (!isIdxInvalid(heap, currentParentIdx) && (parent = heap->distances[currentParentIdx].distance) > node.distance)
	{
		swap(heap, currentIdx, currentParentIdx);
		currentIdx = currentParentIdx;
		currentParentIdx = getParentIdx(currentIdx);
	}
}

int getLeftChildIdx(int idx)
{
	return 2 * idx + 1;
}
int getRightChildIdx(int idx)
{
	return 2 * idx + 2;
}

void heapify(struct MinHeap* heap, int startingIdx)
{
	int leftChildIdx = getLeftChildIdx(startingIdx);
	int rightChildIdx = getRightChildIdx(startingIdx);

	bool hasNoLeftChild = isIdxInvalid(heap, leftChildIdx);
	bool hasNoRightChild = isIdxInvalid(heap, rightChildIdx);

	if (hasNoLeftChild && hasNoRightChild) return;

	double leftChildDistance = DBL_MAX;
	double rightchildDistance = DBL_MAX;
	if (!hasNoLeftChild) leftChildDistance = heap->distances[leftChildIdx].distance;
	if (!hasNoRightChild) rightchildDistance = heap->distances[rightChildIdx].distance;

	int smallerChildIdx = leftChildDistance < rightchildDistance ? leftChildIdx : rightChildIdx;
	double smallerChildDistance = heap->distances[smallerChildIdx].distance;

	double currentDistance = heap->distances[startingIdx].distance;
	if (smallerChildDistance < currentDistance)
	{
		swap(heap, startingIdx, smallerChildIdx);
		heapify(heap, smallerChildIdx);
	}

	return;
}

struct HeapNode extract(struct MinHeap* heap)
{
	struct HeapNode node = heap->distances[0];
	heap->distances[0] = heap->distances[heap->size - 1];
	heap->size--;
	heapify(heap, 0);
	return node;
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** kClosest(int** points, int pointsSize, int* pointsColSize, int k, int* returnSize, int** returnColumnSizes) 
{
	// get return values out of the way
	int** ret = malloc(sizeof(int*) * k);
	*returnColumnSizes = malloc(sizeof(int) * k);
	for (int i = 0; i < k; i++)
	{
		ret[i] = malloc(sizeof(int) * 2);
		(*returnColumnSizes)[i] = 2;
	}
	*returnSize = k;

	// loop through points, add to heap
	struct MinHeap heap = createHeap();
	for (int i = 0; i < pointsSize; i++)
	{
		int* point = points[i];
		int x = point[0];
		int y = point[1];
		insert(&heap, x, y, i);
	}

	// extract k-first nodes of heap to return array
	for (int i = 0; i < k; i++)
	{
		ret[i] = points[extract(&heap).index];
	}

	return ret;
}

