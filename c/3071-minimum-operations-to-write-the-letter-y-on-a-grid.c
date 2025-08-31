#include "leetcode.h"

int minimumOperationsToWriteY(int** grid, int gridSize, int* gridColSize) 
{
	int numsInOutsideRegion[3] = {0, 0, 0};
	for (int r = 0; r < gridSize; r++)
	{
		for (int c = 0; c < gridSize; c++)
		{
			numsInOutsideRegion[grid[r][c]]++;
		}
	}

	int numsInY[3] = {0, 0, 0};
	int rcCenter = gridSize - (gridSize / 2) - 1;
	// left diag
	for (int rc = 0; rc < rcCenter; rc++)
	{
		numsInY[grid[rc][rc]]++;
		numsInOutsideRegion[grid[rc][rc]]--;
	}
	// right diag
	for (int r = 0, c = gridSize - 1; r < rcCenter; r++, c--)
	{
		numsInY[grid[r][c]]++;
		numsInOutsideRegion[grid[r][c]]--;
	}
	// middle line
	for (int r = rcCenter, c = rcCenter; r < gridSize; r++)
	{
		numsInY[grid[r][c]]++;
		numsInOutsideRegion[grid[r][c]]--;
	}

	int minimumOperations = INT_MAX;
	int currentOperations;
	// change outside to 0, y to 1
	currentOperations = 0;
	currentOperations = numsInOutsideRegion[1] + numsInOutsideRegion[2];
	currentOperations += numsInY[0] + numsInY[2];
	minimumOperations = currentOperations < minimumOperations ? currentOperations : minimumOperations;

	// change outside to 0, y to 2
	currentOperations = 0;
	currentOperations = numsInOutsideRegion[1] + numsInOutsideRegion[2];
	currentOperations += numsInY[0] + numsInY[1];
	minimumOperations = currentOperations < minimumOperations ? currentOperations : minimumOperations;
	
	// change outside to 1, y to 0
	currentOperations = 0;
	currentOperations = numsInOutsideRegion[0] + numsInOutsideRegion[2];
	currentOperations += numsInY[1] + numsInY[2];
	minimumOperations = currentOperations < minimumOperations ? currentOperations : minimumOperations;
	
	// change outside to 1, y to 2
	currentOperations = 0;
	currentOperations = numsInOutsideRegion[0] + numsInOutsideRegion[2];
	currentOperations += numsInY[0] + numsInY[1];
	minimumOperations = currentOperations < minimumOperations ? currentOperations : minimumOperations;

	// change outside to 2, y to 0
	currentOperations = 0;
	currentOperations = numsInOutsideRegion[0] + numsInOutsideRegion[1];
	currentOperations += numsInY[1] + numsInY[2];
	minimumOperations = currentOperations < minimumOperations ? currentOperations : minimumOperations;
	
	// change outside to 2, y to 1
	currentOperations = 0;
	currentOperations = numsInOutsideRegion[0] + numsInOutsideRegion[1];
	currentOperations += numsInY[0] + numsInY[2];
	minimumOperations = currentOperations < minimumOperations ? currentOperations : minimumOperations;
	
	return minimumOperations;
}

