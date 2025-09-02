#include "leetcode.h"

#define MAX_NUM_CLASSES 100000

struct HeapNode {
	double deltaPR;
	int numPass;
	int numTotal;
};

struct MaxHeap {
	struct HeapNode* heap[MAX_NUM_CLASSES];
	int size;
	int capacity;
};

struct MaxHeap createHeap()
{
	struct MaxHeap heap;
	heap.size = 0;
	heap.capacity = MAX_NUM_CLASSES;

	return heap;
}

int getParentIdx(int currentIdx)
{
	return (currentIdx + 1) / 2 - 1;
}

int getLeftChildIdx(int currentIdx)
{
	return currentIdx * 2 + 1;
}

int getRightChildIdx(int currentIdx)
{
	return currentIdx * 2 + 2;
}

int isIdxInvalid(struct MaxHeap* heap, int idx)
{
	return idx < 0 || idx >= heap->size;
}

void swap(struct MaxHeap* heap, int swapIdx1, int swapIdx2)
{
	struct HeapNode* temp = heap->heap[swapIdx1];
	heap->heap[swapIdx1] = heap->heap[swapIdx2];
	heap->heap[swapIdx2] = temp;
}

void heapify(struct MaxHeap* heap, int startingIdx)
{
	int leftChildIdx = getLeftChildIdx(startingIdx);
	int rightChildIdx = getRightChildIdx(startingIdx);
	
	bool noLeftChild = isIdxInvalid(heap, leftChildIdx);
	bool noRightChild = isIdxInvalid(heap, rightChildIdx);

	if (noLeftChild && noRightChild) return;

	double startingDelta = heap->heap[startingIdx]->deltaPR;
	double leftChildDelta = -1;
	double rightChildDelta = -1;
	if (!noLeftChild) leftChildDelta = heap->heap[leftChildIdx]->deltaPR;
	if (!noRightChild) rightChildDelta = heap->heap[rightChildIdx]->deltaPR;

	int biggerChildIdx = leftChildDelta > rightChildDelta ? leftChildIdx : rightChildIdx;
	double biggerChildDelta = heap->heap[biggerChildIdx]->deltaPR;

	if (biggerChildDelta > startingDelta)
	{
		swap(heap, startingIdx, biggerChildIdx);
		heapify(heap, biggerChildIdx);
	}

	return;
}

void insert(struct MaxHeap* heap, int numPass, int numTotal)
{
	double PR = (double)numPass / numTotal;
	double deltaPR = (((double)numPass + 1) / (numTotal + 1)) - PR;
	struct HeapNode* node = malloc(sizeof(struct HeapNode));
	node->deltaPR = deltaPR;
	node->numPass = numPass;
	node->numTotal = numTotal;

	int idx = heap->size;
	heap->heap[heap->size++] = node;

	int parentIdx = getParentIdx(idx);
	struct HeapNode* parent;
	while (!isIdxInvalid(heap, parentIdx) && (parent = heap->heap[parentIdx])->deltaPR < node->deltaPR)
	{
		swap(heap, idx, parentIdx);
		idx = parentIdx;
		parentIdx = getParentIdx(idx);
	}
}

void insertNoMalloc(struct MaxHeap* heap, struct HeapNode* node)
{
	int idx = heap->size;
	heap->heap[heap->size++] = node;

	int parentIdx = getParentIdx(idx);
	struct HeapNode* parent;
	while (!isIdxInvalid(heap, parentIdx) && (parent = heap->heap[parentIdx])->deltaPR < node->deltaPR)
	{
		swap(heap, idx, parentIdx);
		idx = parentIdx;
		parentIdx = getParentIdx(idx);
	}
}

struct HeapNode* extract(struct MaxHeap* heap)
{
	struct HeapNode* ret = heap->heap[0];
	swap(heap, 0, heap->size - 1);
	heap->size--;
	heapify(heap, 0);
	return ret;
}

void increaseDelta(struct HeapNode* node)
{
	node->numPass++;
	node->numTotal++;
	double PR = (double)node->numPass / node->numTotal;
	double deltaPR = (((double)node->numPass + 1) / (node->numTotal + 1)) - PR;
	node->deltaPR = deltaPR;
}

double maxAverageRatio(int** classes, int classesSize, int* classesColSize, int extraStudents) 
{
	struct MaxHeap heap = createHeap();

	// construct MaxHeap
	for (int i = 0; i < classesSize; i++)
	{
		int* class = classes[i];
		int numPass = class[0];
		int numTotal = class[1];

		insert(&heap, numPass, numTotal);
	}

	// add students to class with biggest deltaPR
	while (extraStudents > 0)
	{
		struct HeapNode* highestDeltaNode = extract(&heap);
		increaseDelta(highestDeltaNode);
		extraStudents--;
		insertNoMalloc(&heap, highestDeltaNode);
	}

	// calculate final APR
	double APR = 0;
	for (int i = 0; i < heap.size; i++)
	{
		struct HeapNode* class = heap.heap[i];
		double PR = (double)class->numPass / class->numTotal;
		APR += PR;
	}

	APR /= classesSize;

	return APR;
}

