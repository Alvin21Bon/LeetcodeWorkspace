#include "leetcode.h"

int BINARY_SEARCH(int* nums, int size, int startIdx, int target)
{
	if (size == 0) return -1;
	
	int midpoint = (size / 2) + startIdx;
	int foundValue = nums[midpoint];
	if (foundValue == target) return midpoint;
	else if (foundValue > target) return BINARY_SEARCH(nums, midpoint - startIdx, startIdx, target);
	else return  BINARY_SEARCH(nums, size/2 - 1, midpoint + 1, target);
}

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) 
{
	float idxOfFirstMedian = nums1Size / 2.0;
	int valueOfFirstMedian = (nums1[(int)floor(idxOfFirstMedian)] + nums1[(int)ceil(idxOfFirstMedian)]) / 2;
	int idxOfFirstMedianValueInSecondArray = BINARY_SEARCH(nums2, nums2Size, 0, valueOfFirstMedian);
	// MUST BE CHANGED TO A FLOAT AHHHH
}

