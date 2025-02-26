#include "leetcode.h"

struct SubarrayHash {
	int L;
	int R;
	int sum;
	UT_hash_handle hh;
};

void addHashItem(struct SubarrayHash** hashTable, int L, int R, int sum)
{
	size_t hashKeyLen = offsetof(struct SubarrayHash, R)
				+ sizeof(int)
				- offsetof(struct SubarrayHash, L);

	struct SubarrayHash* hashItem = calloc(1, sizeof(*hashItem));
	hashItem->L = L;
	hashItem->R = R;
	hashItem->sum = sum;

	HASH_ADD(hh, *hashTable, L, hashKeyLen, hashItem);
}

struct SubarrayHash* hashFind(struct SubarrayHash** hashTable, int L, int R)
{
	size_t hashKeyLen = offsetof(struct SubarrayHash, R)
				+ sizeof(int)
				- offsetof(struct SubarrayHash, L);

	struct SubarrayHash lookup = {0}, *hashItem;
	lookup.L = L;
	lookup.R = R;

	HASH_FIND(hh, *hashTable, &lookup, hashKeyLen, hashItem);
	return hashItem;
}

void hashDestroy(struct SubarrayHash** hashTable)
{
	struct SubarrayHash *currentItem, *tmpItem;
	HASH_ITER(hh, *hashTable, currentItem, tmpItem)
	{
		HASH_DEL(*hashTable, currentItem);
		free(currentItem);
	}
}

int maxAbsoluteSum(int* nums, int numsSize) 
{
	struct SubarrayHash* hashTable = NULL;
	int largestAbsSum = 0;

	for (int R = 0; R < numsSize; R++)
	{
		for (int L = R; L >= 0; L--)
		{
			int currentSum;
			if (L == R) currentSum = nums[R];
			else
			{
				struct SubarrayHash* prev = hashFind(&hashTable, L, R - 1);
				currentSum = prev->sum + nums[R];
			}

			addHashItem(&hashTable, L, R, currentSum);
			int currentAbsSum = abs(currentSum);
			if (currentAbsSum > largestAbsSum) largestAbsSum = currentAbsSum;
		}
	}

	return largestAbsSum;
}
