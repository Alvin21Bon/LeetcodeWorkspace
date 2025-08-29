#include "leetcode.h"

int cmpAsc(const void* ptr1, const void* ptr2)
{
	int int1 = *(int*)ptr1;
	int int2 = *(int*)ptr2;

	if (int1 < int2) return -1;
	else if (int1 > int2) return 1;
	else return 0;
}

int cmpDesc(const void* ptr1, const void* ptr2)
{
	int int1 = *(int*)ptr1;
	int int2 = *(int*)ptr2;

	if (int1 > int2) return -1;
	else if (int1 < int2) return 1;
	else return 0;
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** sortMatrix(int** grid, int gridSize, int* gridColSize, int* returnSize, int** returnColumnSizes) 
{
	int** returnArray = malloc(gridSize * sizeof(int*));
	for (int i = 0; i < gridSize; i++)
	{
		returnArray[i] = malloc(gridSize * sizeof(int));
	}

	// get this out of the way
	*returnSize = gridSize;
	*returnColumnSizes = malloc(gridSize * sizeof(int));
	for (int i = 0; i < gridSize; i++)
	{
		(*returnColumnSizes)[i] = gridSize;
	}

	int numbersToSort[gridSize];
	int numNumbers = 0;
	// BOTTOM LEFT TRIANGLE
	for (int row = gridSize - 1; row >= 0; row--)
	{
		for (int currentRowPos = row, currentColPos = 0; currentRowPos < gridSize; currentRowPos++, currentColPos++)
		{
			numbersToSort[numNumbers++] = grid[currentRowPos][currentColPos];
		}

		qsort(numbersToSort, numNumbers, sizeof(int), cmpDesc);
		for (int currentRowPos = row, currentColPos = 0, idx = 0; currentRowPos < gridSize; currentRowPos++, currentColPos++, idx++)
		{
			returnArray[currentRowPos][currentColPos] = numbersToSort[idx];
		}

		numNumbers = 0;
	}

	for (int col = 1; col < gridSize; col++)
	{
		for (int currentColPos = col, currentRowPos = 0; currentColPos < gridSize; currentRowPos++, currentColPos++)
		{
			numbersToSort[numNumbers++] = grid[currentRowPos][currentColPos];
		}

		qsort(numbersToSort, numNumbers, sizeof(int), cmpAsc);
		for (int currentColPos = col, currentRowPos = 0, idx = 0; currentColPos < gridSize; currentRowPos++, currentColPos++, idx++)
		{
			returnArray[currentRowPos][currentColPos] = numbersToSort[idx];
		}

		numNumbers = 0;
	}

	return returnArray;
}

