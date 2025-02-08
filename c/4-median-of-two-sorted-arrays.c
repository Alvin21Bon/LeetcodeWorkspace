#include "leetcode.h"

// ####################################
// FAIL
// ####################################

double BINARY_SEARCH(int* nums, int size, double target)
{
	int left = 0;
	int right = size - 1;
	int midpoint = (right + left) / 2;
	do
	{
		// RETURN THE RIGHT MOST INSTANCE OF TARGET 
		if (nums[midpoint] == target) 
		{
			int rightMostInstacneOfTarget;
			for (rightMostInstacneOfTarget = midpoint; rightMostInstacneOfTarget < size; rightMostInstacneOfTarget++)
			{
				if (nums[rightMostInstacneOfTarget] != target)
				{
					return --rightMostInstacneOfTarget;
				}
			}

			rightMostInstacneOfTarget--;
			return rightMostInstacneOfTarget;
		}
		else if (nums[midpoint] < target) left = midpoint + 1;
		else right = midpoint - 1;

		midpoint = (right + left) / 2;
	} while (left <= right);

	// due to the way it is calculated, midpoint will always be a valid index
	double offset = nums[midpoint] < target ? 0.5 : -0.5;
	return midpoint + offset;
}

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) 
{
	if (nums1Size == 0)
	{
		double idxOfSecondMedian = (0.5 * nums2Size) - 0.5;
		return (nums2[(int)floor(idxOfSecondMedian)] + nums2[(int)ceil(idxOfSecondMedian)]) / 2.0;
	}

	double idxOfFirstMedian = (0.5 * nums1Size) - 0.5;
	double valueOfFirstMedian = (nums1[(int)floor(idxOfFirstMedian)] + nums1[(int)ceil(idxOfFirstMedian)]) / 2.0;
	if (nums2Size == 0) return valueOfFirstMedian;
	double idxOfFirstMedianValueInSecondArray = BINARY_SEARCH(nums2, nums2Size, valueOfFirstMedian);

	int elementsAddedToTheLeft = floor(idxOfFirstMedianValueInSecondArray) + 1;
	int elementsAddedToTheRight = nums2Size - elementsAddedToTheLeft;

	double medianShift = (elementsAddedToTheRight - elementsAddedToTheLeft) / 2.0;
	bool isArr1ExactMedian = idxOfFirstMedian == (int)idxOfFirstMedian;
	bool isArr2MedianFound = idxOfFirstMedianValueInSecondArray == (int)idxOfFirstMedianValueInSecondArray;
	if (medianShift == 0) 
	{
		if (isArr1ExactMedian) return valueOfFirstMedian;
		else
		{
			int arr1LeftValue = nums1[(int)floor(idxOfFirstMedian)];
			int arr1RightValue = nums1[(int)ceil(idxOfFirstMedian)];
			int arr2LeftValue, arr2RightValue;
			if (isArr2MedianFound)
			{
				arr2LeftValue = valueOfFirstMedian;
				if ((idxOfFirstMedianValueInSecondArray + 1) < nums2Size) arr2RightValue = nums2[(int)idxOfFirstMedianValueInSecondArray + 1];
				else arr2RightValue = arr1RightValue; // :/
			}
			else
			{
				if (idxOfFirstMedianValueInSecondArray < 0) arr2LeftValue = arr1LeftValue;
				else arr2LeftValue = nums2[(int)(idxOfFirstMedianValueInSecondArray - 0.5)];
				if (idxOfFirstMedianValueInSecondArray > (nums2Size - 1)) arr2RightValue = arr1RightValue;
				else arr2RightValue = nums2[(int)(idxOfFirstMedianValueInSecondArray + 0.5)];
			}

			int leftValue = arr1LeftValue > arr2LeftValue ? arr1LeftValue : arr2LeftValue;
			int rightValue = arr1RightValue < arr2RightValue ? arr1RightValue : arr2RightValue;
			return (leftValue + rightValue) / 2.0;
		}
	}

	int elementsToSort = ceil(fabs(medianShift) + (0.5 * !isArr1ExactMedian));
	int medianShiftDir = (0 < medianShift) - (medianShift < 0);
	int arr1SortPointer, arr2SortPointer;
	
	arr1SortPointer = isArr1ExactMedian ? idxOfFirstMedian + medianShiftDir : idxOfFirstMedian + (0.5 * medianShiftDir);
	if (isArr2MedianFound)
	{
		if (medianShiftDir > 0) arr2SortPointer = idxOfFirstMedianValueInSecondArray + 1;
		else arr2SortPointer = idxOfFirstMedianValueInSecondArray;
	}
	else arr2SortPointer = idxOfFirstMedianValueInSecondArray + (0.5 * medianShiftDir);

	int miniSortArray[elementsToSort];
	for (int i = 0; i < elementsToSort; i++)
	{
		if (medianShiftDir > 0)
		{
			if (arr1SortPointer < nums1Size && (arr2SortPointer >= nums2Size || nums1[arr1SortPointer] < nums2[arr2SortPointer]))
			{
				miniSortArray[i] = nums1[arr1SortPointer];
				arr1SortPointer++;
			}
			else
			{
				miniSortArray[i] = nums2[arr2SortPointer];
				arr2SortPointer++;
			}
		}
		else
		{
			if (arr1SortPointer >= 0 && (arr2SortPointer < 0 || nums1[arr1SortPointer] > nums2[arr2SortPointer]))
			{
				miniSortArray[i] = nums1[arr1SortPointer];
				arr1SortPointer--;
			}
			else
			{
				miniSortArray[i] = nums2[arr2SortPointer];
				arr2SortPointer--;
			}
		}
	}

	if (isArr1ExactMedian)
	{
		if (medianShift == 0.5 || medianShift == -0.5)
		{
			return (nums1[(int)idxOfFirstMedian] + miniSortArray[0]) / 2.0;
		}

		medianShift = fabs(medianShift);
		medianShift--;
		return (miniSortArray[(int)floor(medianShift)] + miniSortArray[(int)ceil(medianShift)]) / 2.0;
	}
	else
	{
		medianShift = fabs(medianShift);
		medianShift -= 0.5;
		return (miniSortArray[(int)floor(medianShift)] + miniSortArray[(int)ceil(medianShift)]) / 2.0;
	}
}

int main()
{
	int nums1[] = {2, 3};
	int nums2[] = {1};
	double ugh = findMedianSortedArrays(nums1, 2, nums2, 1);
	printf("%f\n", ugh);
}
