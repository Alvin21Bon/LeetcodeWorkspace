#include "leetcode.h"

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* applyOperations(int* nums, int numsSize, int* returnSize) 
{
	int* returnArray = calloc(numsSize, sizeof(int));
	*returnSize = numsSize;

	for (int idx = 0, returnArrayIdx = 0; idx < numsSize; idx++)
	{
		if (idx == numsSize - 1) returnArray[returnArrayIdx] = nums[idx];
		else if (nums[idx] == 0) continue;
		else if (nums[idx] == nums[idx + 1])
		{
			returnArray[returnArrayIdx] = nums[idx] * 2;
			nums[idx + 1] = 0;
			returnArrayIdx++;
		}
		else
		{
			returnArray[returnArrayIdx] = nums[idx];
			returnArrayIdx++;
		}
	}

	return returnArray;
}
