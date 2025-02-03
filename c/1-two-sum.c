#include "leetcode.h"

struct HashTable {
	int number; // key
	int index;
	UT_hash_handle hh;
};

void FREE_HASH(struct HashTable** hashTable);
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
	struct HashTable *hashTable = NULL, *item;

	int numNeededToSumToTarget;
	for (int i = 0; i < numsSize; i++)
	{
		numNeededToSumToTarget = target - nums[i];
		HASH_FIND_INT(hashTable, &numNeededToSumToTarget, item);
		if (item)
		{
			*returnSize = 2;
			int* result = malloc(*returnSize * sizeof(int));
			result[0] = item->index;
			result[1] = i;
			FREE_HASH(&hashTable);
			return result;
		}

		item = malloc(sizeof(struct HashTable));
		item->number = nums[i];
		item->index = i;
		HASH_ADD_INT(hashTable, number, item);
	}

	FREE_HASH(&hashTable);
	*returnSize = 0;
	return NULL;
}

void FREE_HASH(struct HashTable** hashTable)
{
	struct HashTable *currentItem, *tmpItem;
	HASH_ITER(hh, *hashTable, currentItem, tmpItem)
	{
		HASH_DEL(*hashTable, currentItem);
		free(currentItem);
	}
}
