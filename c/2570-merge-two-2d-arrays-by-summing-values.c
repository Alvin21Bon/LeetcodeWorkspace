#include "leetcode.h"

// KEEPS RETURNING A HEAP OVERFLOW IN LEETCODE WHEN IT OUTPUTS FINE HERE. I DONT KNOW WHY.

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** mergeArrays(int** nums1, int nums1Size, int* nums1ColSize, int** nums2, int nums2Size, int* nums2ColSize, int* returnSize, int** returnColumnSizes) 
{
	int** outputArray = malloc((nums1Size + nums2Size) * sizeof(int*));
	*returnColumnSizes = malloc(sizeof(int));
	**returnColumnSizes = 2;

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

	return outputArray;
}

int main()
{
	int** nums1 = malloc(3 * sizeof(int*));
	int** nums2 = malloc(3 * sizeof(int*));

	for (int idx = 0; idx < 3; idx++)
	{
		nums1[idx] = malloc(2 * sizeof(int));
		nums2[idx] = malloc(2 * sizeof(int));
	}

	nums1[0][0] = 1;
	nums1[0][1] = 2;
	nums1[1][0] = 2;
	nums1[1][1] = 3;
	nums1[2][0] = 4;
	nums1[2][1] = 5;

	nums2[0][0] = 1;
	nums2[0][1] = 4;
	nums2[1][0] = 3;
	nums2[1][1] = 2;
	nums2[2][0] = 4;
	nums2[2][1] = 1;

	int uh =2;
	int ok;
	int* fr;
	int** frick = mergeArrays(nums1, 3, &uh, nums2, 3, &uh, &ok, &fr);

	for (int idx = 0; idx < ok; idx++)
	{
		for (int j = 0; j < *fr; j++)
		{
			printf("%d ", frick[idx][j]);
		}
		printf("\n");
	}

	free(frick);


	free(fr);
	for (int idx = 0; idx < 3; idx++)
	{
		free(nums1[idx]);
		free(nums2[idx]);
	}
	free(nums1);
	free(nums2);
	return 0;
}
