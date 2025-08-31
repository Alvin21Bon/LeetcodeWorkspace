#include "leetcode.h"

int cmp(const void* ptr1, const void* ptr2)
{
	int int1 = *(int*)ptr1;
	int int2 = *(int*)ptr2;

	if (int1 < int2) return -1;
	if (int1 == int2) return 0;
	return 1;
}

long long minimalKSum(int* nums, int numsSize, int k) 
{
	qsort(nums, numsSize, sizeof(int), cmp);

	long long sum = 0;
	for (int i = 0; i < numsSize && k != 0; i++)
	{
		int elem = nums[i];
		int prevElem  = i != 0 ? nums[i - 1] : 0;
		int delta = elem - prevElem;

		if (delta > 1)
		{
			for (int j = prevElem + 1; j < elem && k != 0; j++)
			{
				sum += j;
				k--;
			}
		}
	}

	// reached end of array
	if (k != 0)
	{
		int lastElem = nums[numsSize - 1];
		for (int i = lastElem + 1; k != 0; i++, k--)
		{
			sum += i;
		}
	}

	return sum;
}

