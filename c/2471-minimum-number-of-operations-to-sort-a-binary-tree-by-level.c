#include "leetcode.h"

struct TreeNode {
	int val;
	struct TreeNode* left;
	struct TreeNode* right;
};

int cmp(const void* ptr1, const void* ptr2)
{
	int* castedPtr1 = (int*)ptr1;
	int* castedPtr2 = (int*)ptr2;

	if (*castedPtr1 < *castedPtr2) return -1;
	return 1;
}

void swapNodeValues(struct TreeNode* nodeA, struct TreeNode* nodeB)
{
	int temp = nodeA->val;
	nodeA->val = nodeB->val;
	nodeB->val = temp;
}

int getMinSteps(struct TreeNode** array, int n)
{
	int indexHash[100001];
	int sortedArray[n];
	for (int i = 0; i < n; i++)
	{
		indexHash[array[i]->val] = i;
		sortedArray[i] = array[i]->val;
	}

	qsort((void*)sortedArray, n, sizeof(int), cmp);

	int numSteps = 0;
	for (int i = 0; i < n; i++)
	{
		int val = sortedArray[i];
		if (indexHash[val] != i)
		{
			int idx1 = indexHash[array[i]->val];
			int idx2 = indexHash[val];

			numSteps++;
			indexHash[array[i]->val] = indexHash[val];
			indexHash[val] = i;

			swapNodeValues(array[idx1], array[idx2]);
		}
	}

	return numSteps;
}

int minimumOperations(struct TreeNode* root) 
{
	int numSteps = 0;
	int numValues = 0;
	struct TreeNode* nodeArray[65536];
	struct TreeNode* nodeQueue[65536];
	nodeQueue[0] = root;
	int queueSize = 1;

	while (queueSize)
	{
		for (int qIdx = 0; qIdx < queueSize; qIdx++)
		{
			nodeArray[numValues++] = nodeQueue[qIdx];
		}
		queueSize = 0;

		for (int idx = 0; idx < numValues; idx++)
		{
			struct TreeNode* left = nodeArray[idx]->left;
			struct TreeNode* right = nodeArray[idx]->right;
			if (left) nodeQueue[queueSize++] = left;
			if (right) nodeQueue[queueSize++] = right;
		}

		numSteps += getMinSteps(nodeArray, numValues);
		numValues = 0;
	}

	return numSteps;
}

