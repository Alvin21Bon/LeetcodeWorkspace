#include "leetcode.h"

int countSubarrays(int* nums, int numsSize) 
{
	int ret = 0;
	for (int i = 0; i < numsSize - 2; i++)
	{
		int sum = nums[i] + nums[i + 2];
		ret += sum == (float)nums[i + 1] / 2;
	}

	return ret;
}

