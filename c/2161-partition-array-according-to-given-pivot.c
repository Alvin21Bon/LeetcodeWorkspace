#include "leetcode.h"

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* pivotArray(int* nums, int numsSize, int pivot, int* returnSize) 
{
	int lessThanArray[numsSize];
	int greaterThanArray[numsSize];
	int equalArray[numsSize];
	int equalArraySize = 0;
	int lessThanArraySize = 0;
	int greaterThanArraySize = 0;

	for (int idx = 0; idx < numsSize; idx++)
	{
		int num = nums[idx];
		if (num == pivot)
		{
			equalArray[equalArraySize] = pivot;
			equalArraySize++;
		}
		else if (num < pivot)
		{
			lessThanArray[lessThanArraySize] = num;
			lessThanArraySize++;
		}
		else
		{
			greaterThanArray[greaterThanArraySize] = num;
			greaterThanArraySize++;
		}
	}

	int* outputArray = malloc(numsSize * sizeof(int));
	*returnSize = numsSize;

	memcpy(outputArray, lessThanArray, lessThanArraySize * sizeof(int));
	memcpy(outputArray + lessThanArraySize, equalArray, equalArraySize * sizeof(int));
	memcpy(outputArray + lessThanArraySize + equalArraySize, greaterThanArray, greaterThanArraySize * sizeof(int));

	return outputArray;
}
