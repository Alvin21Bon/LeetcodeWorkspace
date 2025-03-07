#include "leetcode.h"

int firstMissingPositive(int* nums, int numsSize) 
{
	// turn nums array into a mock hash map
	for (int idx = 0; idx < numsSize; idx++)
	{
		while (true)
		{
			bool isElementAValidNumber = nums[idx] >= 1 && nums[idx] <= numsSize;
			if (!isElementAValidNumber) break;

			int swapIdx = nums[idx] - 1;
			bool shouldPerformASwap = isElementAValidNumber && nums[swapIdx] != nums[idx];

			if (!shouldPerformASwap) break;

			int temp = nums[swapIdx];
			nums[swapIdx] = nums[idx];
			nums[idx] = temp;
		}
	}

	for (int idx = 0, currentSmallestNum = 1; idx < numsSize; idx++, currentSmallestNum++)
	{
		if (nums[idx] != currentSmallestNum) return currentSmallestNum;
	}

	return numsSize + 1;
}
