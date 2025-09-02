#include "leetcode.h"

#define MAX_LADDERS 100000

struct HeapNode {
	int ladderHeight;
};

struct MinHeap {
	struct HeapNode ladders[MAX_LADDERS];
	int size;
};

struct MinHeap createHeap()
{
	struct MinHeap heap;
	heap.size = 0;

	return heap;
}

void swap(struct MinHeap* heap, int idx1, int idx2)
{
	struct HeapNode temp = heap->ladders[idx1];
	heap->ladders[idx1] = heap->ladders[idx2];
	heap->ladders[idx2] = temp;
}

int getParentIdx(int i)
{
	return ((i + 1) / 2) - 1;
}
int getLeftChildIdx(int i)
{
	return i * 2 + 1;
}
int getRightChildIdx(int i)
{
	return i * 2 + 2;
}

bool isInvalidIdx(struct MinHeap* heap, int i)
{
	return i < 0 || i >= heap->size;
}

void heapify(struct MinHeap* heap, int startingIdx)
{
	int leftChildIdx = getLeftChildIdx(startingIdx);
	int rightchildIdx = getRightChildIdx(startingIdx);

	bool hasNoLeftChild = isInvalidIdx(heap, leftChildIdx);
	bool hasNoRightChild = isInvalidIdx(heap, rightchildIdx);

	if (hasNoLeftChild && hasNoRightChild) return;

	int leftChildHeight = INT_MAX;
	int rightChildHeight = INT_MAX;
	if (!hasNoLeftChild) leftChildHeight = heap->ladders[leftChildIdx].ladderHeight;
	if (!hasNoRightChild) rightChildHeight = heap->ladders[rightchildIdx].ladderHeight;

	int smallerChildIdx = leftChildHeight < rightChildHeight ? leftChildIdx : rightchildIdx;
	int smallerChildHeight = heap->ladders[smallerChildIdx].ladderHeight;

	int startingHeight = heap->ladders[startingIdx].ladderHeight;
	if (smallerChildHeight < startingHeight)
	{
		swap(heap, startingIdx, smallerChildIdx);
		heapify(heap, smallerChildIdx);
	}

	return;
}

void insert(struct MinHeap* heap, int height)
{
	struct HeapNode node;
	node.ladderHeight = height;

	int currentIdx = heap->size;
	heap->ladders[heap->size++] = node;

	int currentParentIdx = getParentIdx(currentIdx);
	struct HeapNode parent;
	while (!isInvalidIdx(heap, currentParentIdx) && (parent = heap->ladders[currentParentIdx]).ladderHeight > height)
	{
		swap(heap, currentIdx, currentParentIdx);
		currentIdx = currentParentIdx;
		currentParentIdx = getParentIdx(currentIdx);
	}
}

struct HeapNode extract(struct MinHeap* heap)
{
	if (heap->size <= 0)
	{
		struct HeapNode node = {INT_MAX};
		return node;
	}
	struct HeapNode node = heap->ladders[0];
	heap->ladders[0] = heap->ladders[heap->size - 1];
	heap->size--;
	heapify(heap, 0);
	return node;
}

struct HeapNode peek(struct MinHeap* heap)
{
	if (heap->size <= 0)
	{
		struct HeapNode node = {INT_MAX};
		return node;
	}

	return heap->ladders[0];
}

int furthestBuilding(int* heights, int heightsSize, int bricks, int ladders) 
{
	struct MinHeap heap = createHeap();
	for (int i = 0; i < heightsSize - 1; i++)
	{
      		int curHeight = heights[i];
      		int nextHeight = heights[i + 1];
      		int deltaHeight = nextHeight - curHeight;
		if (curHeight >= nextHeight) continue;

		if (ladders > 0)
		{
			ladders--;
			insert(&heap, deltaHeight);
			continue;
		}

		struct HeapNode ladder = peek(&heap);
		if (bricks < ladder.ladderHeight || ladder.ladderHeight > deltaHeight)
		{
			if (bricks < deltaHeight) return i;

			bricks -= deltaHeight;
			continue;
		}

		extract(&heap);
		bricks -= ladder.ladderHeight;
		insert(&heap, deltaHeight);
	}

	return heightsSize - 1;
}

