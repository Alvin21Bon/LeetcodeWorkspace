#include "leetcode.h"

int maxAdjacentDistance(int* nums, int numsSize) 
{
	int maxDiff = 0;
	for (int i = 0; i < numsSize - 1; i++)
	{
      		int diff = abs(nums[i] - nums[i + 1]);
		maxDiff = diff > maxDiff ? diff : maxDiff;
	}

	int lastDiff = abs(nums[numsSize - 1] - nums[0]);
	maxDiff = lastDiff > maxDiff ? lastDiff : maxDiff;

	return maxDiff;
}
