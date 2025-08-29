#include "leetcode.h"

// USED HEAP FOR PRACTICE

struct HeapNode {
	int group;
	int count;
};

struct Heap {
	struct HeapNode* heap;
	int size;
	int capacity;
};

void swap(struct Heap* heap, int idx1, int idx2)
{
	struct HeapNode temp = heap->heap[idx1];
	heap->heap[idx1] = heap->heap[idx2];
	heap->heap[idx2] = temp;
}

struct Heap heap()
{
	struct Heap heap;
	heap.size = 0;
	heap.capacity = 32;
	heap.heap = malloc(sizeof(struct HeapNode) * heap.capacity);

	return heap;
}

int find(struct Heap* heap, int group)
{
	for (int i = 0; i < heap->size; i++)
	{
		if (heap->heap[i].group == group) return i;
	}

	return -1;
}

int getParentIdx(int currentIdx)
{
	return ((currentIdx + 1) / 2) - 1;
}

int getLeftChildIdx(int currentIdx)
{
	return currentIdx * 2 + 1;
}
int getRightChildIdx(int currentIdx)
{
	return currentIdx * 2 + 2;
}

void reverseHeapify(struct Heap* heap, int startIdx)
{
	int parentIdx = getParentIdx(startIdx);

	while (parentIdx >= 0 && heap->heap[parentIdx].count < heap->heap[startIdx].count)
	{
		swap(heap, startIdx, parentIdx);
		parentIdx = getParentIdx(parentIdx);
	}
}

void update(struct Heap* heap, int idx, int newCount)
{
	heap->heap[idx].count = newCount;
	reverseHeapify(heap, idx);
}

void insert(struct Heap* heap, int group)
{
	int foundIdx = find(heap, group);
	if (foundIdx != -1)
	{
		update(heap, foundIdx, heap->heap[foundIdx].count + 1);
		return;
	}

	if (heap->size >= heap->capacity / 2)
	{
		heap->capacity *= 2;
		heap->heap = realloc(heap->heap, sizeof(struct HeapNode) * heap->capacity);
	}

	struct HeapNode node = { .group = group, .count = 1 };
	heap->heap[heap->size++] = node;
}

int getNumMax(struct Heap* heap)
{
	int maxValue = heap->heap[0].count;
	int numMax = 0;

	for (int i = 0, levelSize = 1, levelCtr = 1, maxFoundOnLevel = 0; i < heap->size; i++, levelCtr++)
	{
		if (heap->heap[i].count == maxValue)
		{
			numMax++;
			maxFoundOnLevel = 1;
		}

		if (levelCtr == levelSize)
		{
			if (!maxFoundOnLevel) return numMax;

			levelCtr = 0;
			levelSize *= 2;
			maxFoundOnLevel = 0;
		}
	}

	return numMax;
}

int countLargestGroup(int n) 
{
	struct Heap maxHeap = heap();
	for (int i = 1; i <= n; i++)
	{
		int temp = i;
		int group = 0;
		while (temp > 0)
		{
			int digit = temp % 10;
			group += digit;
			temp /= 10;
		}

		insert(&maxHeap, group);
	}

	int numMax = getNumMax(&maxHeap);
	free(maxHeap.heap);
	return numMax;
}

