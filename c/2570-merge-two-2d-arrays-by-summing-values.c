#include "leetcode.h"

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** mergeArrays(int** nums1, int nums1Size, int* nums1ColSize, int** nums2, int nums2Size, int* nums2ColSize, int* returnSize, int** returnColumnSizes) 
{
	int** outputArray = malloc((nums1Size + nums2Size) * sizeof(int*));

	int nums1Ptr = 0, nums2Ptr = 0, outputArrayPtr = 0;
	while (nums1Ptr < nums1Size || nums2Ptr < nums2Size)
	{
		if (nums1Ptr >= nums1Size)
		{
			outputArray[outputArrayPtr] = nums2[nums2Ptr];

			nums2Ptr++;
		}
		else if (nums2Ptr >= nums2Size)
		{
			outputArray[outputArrayPtr] = nums1[nums1Ptr];

			nums1Ptr++;
		}
		else if (nums1[nums1Ptr][0] == nums2[nums2Ptr][0])
		{
			outputArray[outputArrayPtr] = nums1[nums1Ptr];
			outputArray[outputArrayPtr][1] += nums2[nums2Ptr][1];

			nums1Ptr++; nums2Ptr++;
		}
		else if (nums1[nums1Ptr][0] < nums2[nums2Ptr][0])
		{
			outputArray[outputArrayPtr] = nums1[nums1Ptr];

			nums1Ptr++;
		}
		else
		{
			outputArray[outputArrayPtr] = nums2[nums2Ptr];

			nums2Ptr++;
		}

		outputArrayPtr++;
	}

	*returnSize = outputArrayPtr;
	outputArray = realloc(outputArray, *returnSize * sizeof(int*));

	*returnColumnSizes = malloc(*returnSize * sizeof(int));
	for (int idx = 0; idx < *returnSize; idx++) (*returnColumnSizes)[idx] = 2;

	return outputArray;
}

