#include "leetcode.h"

enum SlopeStates
{
	FLAT,
	INCREASING,
	DECREASING
};

int longestMonotonicSubarray(int* nums, int numsSize) 
{
	if (numsSize == 1) return 1;
	int currentLength = 0;
	int longestLength = 1;
	enum SlopeStates slope = FLAT;

	for (int i = 1; i < numsSize; i++)
	{
		int prevElement = nums[i - 1];
		int currElement = nums[i];

		switch (slope)
      		{
      			case FLAT:
      				if (currElement > prevElement)
      				{
      					slope = INCREASING;
      					currentLength = 2;
      				}
      				else if (currElement < prevElement)
      				{
      					slope = DECREASING;
      					currentLength = 2;
      				}
      				break;
      			case INCREASING:
      				if (currElement == prevElement)
      				{
      					slope = FLAT;
      					longestLength = currentLength > longestLength ? currentLength : longestLength;
      					currentLength = 0;
      					continue;
      				}
      				else if (currElement < prevElement)
      				{
      					slope = DECREASING;
      					longestLength = currentLength > longestLength ? currentLength : longestLength;
      					currentLength = 2;
      					continue;
      				}

      				currentLength++;
      				break;
      			case DECREASING:
      				if (currElement == prevElement)
      				{
      					slope = FLAT;
      					longestLength = currentLength > longestLength ? currentLength : longestLength;
      					currentLength = 0;
      					continue;
      				}
      				else if (currElement > prevElement)
      				{
      					slope = INCREASING;
      					longestLength = currentLength > longestLength ? currentLength : longestLength;
      					currentLength = 2;
      					continue;
      				}

      				currentLength++;
      				break;
      		}
	}

	longestLength = currentLength > longestLength ? currentLength : longestLength;
	return longestLength;
}
