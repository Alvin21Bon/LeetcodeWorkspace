#include "leetcode.h"

int longestNiceSubarray(int* nums, int numsSize) 
{
	int largestLength = 1;
	int niceCheck = 0;

	int startIdx = 0;
	for (int idx = 0; idx <= numsSize; idx++)
	{
		if (idx == numsSize)
		{
			int currentLength = idx - startIdx;
			largestLength = currentLength > largestLength ? currentLength : largestLength;
			break;
		}
		int num = nums[idx];
		bool isStillNice = (niceCheck & num) == 0;

		if (isStillNice)
		{
			niceCheck |= num;
		}
		else
		{
			niceCheck = 0;
			int curLength = idx - startIdx;
			largestLength = curLength > largestLength ? curLength : largestLength;

			int mishapIdx = 0;
			for (int innerIdx = startIdx; innerIdx < idx; innerIdx++)
			{
				int innerNum = nums[innerIdx];
				if ((innerNum & num) != 0) 
				{
					mishapIdx = innerIdx;
					break;
				}
			}

			if (mishapIdx >= numsSize - largestLength) break;
			startIdx = mishapIdx + 1;
			idx = mishapIdx;
		}
	}

	return largestLength;
}
