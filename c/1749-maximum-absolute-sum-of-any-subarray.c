#include "leetcode.h"

int maxAbsoluteSum(int* nums, int numsSize) 
{
	int minPrefixSum = INT_MAX;
	int maxPrefixSum = INT_MIN;
	int prefixSum = 0;
	int maxAbsSum = 0;

	for (int idx = 0; idx < numsSize; idx++)
	{
		prefixSum += nums[idx];
		minPrefixSum = prefixSum < minPrefixSum ? prefixSum : minPrefixSum;
		maxPrefixSum = prefixSum > maxPrefixSum ? prefixSum : maxPrefixSum;

		if (prefixSum > 0)
		{
			maxAbsSum = prefixSum > maxAbsSum ? prefixSum : maxAbsSum;
			maxAbsSum = prefixSum - minPrefixSum > maxAbsSum ? prefixSum - minPrefixSum : maxAbsSum;
		}
		else if (prefixSum < 0)
		{
			maxAbsSum = abs(prefixSum) > maxAbsSum ? abs(prefixSum) : maxAbsSum;
			maxAbsSum = abs(prefixSum - maxPrefixSum) > maxAbsSum ? abs(prefixSum - maxPrefixSum) : maxAbsSum;
		}
	}

	return maxAbsSum;
}
