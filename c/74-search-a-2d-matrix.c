#include "leetcode.h"

bool searchMatrix(int** matrix, int matrixSize, int* matrixColSize, int target) 
{
	int targetRow = matrixSize - 1;
	for (int i = 0; i < matrixSize; i++)
	{
		if (matrix[i][0] > target) 
		{
			targetRow = i - 1;
			break;
		}
	}

	if (targetRow < 0) return false;

	for (int i = 0; i < matrixColSize[targetRow]; i++)
	{
		if (matrix[targetRow][i] == target) return true;
	}

	return false;
}

