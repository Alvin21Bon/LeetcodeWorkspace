#include "leetcode.h"

bool check(int* nums, int numsSize) 
{
	bool discrepancyFlag = false; 
	int e1, e2;
	for (int i = 0; i < numsSize; i++)
	{
		if (nums[i] > nums[(i+1) % numsSize])
		{
			if (discrepancyFlag) return false;
			discrepancyFlag = true;
		}
	}

	return true;
}
