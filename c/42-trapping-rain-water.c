#include "leetcode.h"

int trap(int* height, int heightSize) 
{
	int measuredWidth = heightSize;
	int measuredHeight = 0;
	ulong totalArea;
	ulong invalidSquaresLeft = 0; 
	ulong invalidSquaresRight = 0; 
	ulong sumHeights = 0; 
	ulong totalInvalidSquares;

	// left, also get array info
	for (int idx = 0, currentHighestHeight = 0; idx < heightSize; idx++)
	{
		int currentHeight = height[idx];
		if (currentHeight > measuredHeight) measuredHeight = currentHeight;
		sumHeights += currentHeight;

		if (currentHeight > currentHighestHeight)
		{
			invalidSquaresLeft += (currentHeight - currentHighestHeight) * idx;
			currentHighestHeight = currentHeight;
		}
	}

	totalArea = measuredWidth * measuredHeight;

	// right
	for (int idx = heightSize - 1, currentHighestHeight = 0; idx >= 0; idx--)
	{
		int currentHeight = height[idx];
		if (currentHeight > currentHighestHeight)
		{
			invalidSquaresRight += (currentHeight - currentHighestHeight) * (heightSize - idx - 1);
			currentHighestHeight = currentHeight;
		}
	}

	totalInvalidSquares = invalidSquaresLeft + invalidSquaresRight;
	return totalArea - sumHeights - totalInvalidSquares;
}
