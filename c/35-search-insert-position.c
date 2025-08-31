#include "leetcode.h"

int searchInsert(int* nums, int numsSize, int target) 
{
	int lIdx = 0;
	int rIdx = numsSize - 1;

	while (lIdx < rIdx)
	{
		int mIdx = (lIdx + rIdx) / 2;
		if (nums[mIdx] == target) return mIdx;
		
		if (nums[mIdx] > target) rIdx = mIdx - 1;
		else lIdx = mIdx + 1;
	}

	if (nums[lIdx] == target) return lIdx;

	return nums[lIdx] > target ? lIdx : lIdx + 1;
}

