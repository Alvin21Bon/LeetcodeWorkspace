#include "leetcode.h"

enum Direction {
	TOP_LEFT,
	TOP_RIGHT,
	BOT_LEFT,
	BOT_RIGHT
};

struct Grid {
	int** cells;
	int numRows;
	int* numCols;
};

struct Coords {
	int row;
	int col;
};

int toggleValidNum(int prevNum)
{
	if (prevNum == 1) return 2;
	else if (prevNum == 2) return 0;
	return 2;
}

bool isCoordsValid(struct Grid grid, struct Coords coords)
{
	if (coords.col < 0 || coords.row < 0) return false;
	if (coords.row >= grid.numRows || coords.col >= grid.numCols[coords.row]) return false;
	return true;
}

struct Coords getDeltaCoords(enum Direction direction)
{
	struct Coords delta;
	switch (direction)
	{
		case TOP_LEFT:
			delta.col = -1;
			delta.row = -1;
			break;
		case TOP_RIGHT:
			delta.col = 1;
			delta.row = -1;
			break;
		case BOT_LEFT:
			delta.col = -1;
			delta.row = 1;
			break;
		case BOT_RIGHT:
			delta.col = 1;
			delta.row = 1;
			break;
	}

	return delta;
}

struct Coords reverseDeltaCoords(struct Coords deltaCoords)
{
	deltaCoords.col *= -1;
	deltaCoords.row *= -1;
	return deltaCoords;
}

struct Coords addCoords(struct Coords a, struct Coords b)
{
	struct Coords added;
	added.col = a.col + b.col;
	added.row = a.row + b.row;

	return added;
}

bool isCoordsEqual(struct Coords a, struct Coords b)
{
	return (a.col == b.col) && (a.row == b.row);
}

int scanDiagonal(struct Grid grid, struct Coords startingCoords, enum Direction direction, bool hasUsedTurn)
{
	struct Coords deltaCoords = getDeltaCoords(direction);
	struct Coords newCoords = addCoords(startingCoords, deltaCoords);

	int validNum = toggleValidNum(grid.cells[startingCoords.row][startingCoords.col]);
	int diagonalLength = 1;
	while (isCoordsValid(grid, newCoords) && grid.cells[newCoords.row][newCoords.col] == validNum)
	{
		diagonalLength++;
		newCoords = addCoords(newCoords, deltaCoords);
		validNum = toggleValidNum(validNum);
	}

	if (hasUsedTurn || diagonalLength == 1) return diagonalLength;

	// start exhausting all possible turns in the first branch
	int longestLength = diagonalLength;
	int firstBranchLength = diagonalLength;
	int turnLength;

	deltaCoords = reverseDeltaCoords(deltaCoords);
	newCoords = addCoords(newCoords, deltaCoords); // undo last delta movement of previous while loop
	while (!isCoordsEqual(newCoords, startingCoords))
	{
		switch (direction)
		{
			case TOP_LEFT:
				turnLength = scanDiagonal(grid, newCoords, TOP_RIGHT, true) + firstBranchLength - 1;
				longestLength = turnLength > longestLength ? turnLength : longestLength;
				break;
			case BOT_RIGHT:
				turnLength = scanDiagonal(grid, newCoords, BOT_LEFT, true) + firstBranchLength - 1;
				longestLength = turnLength > longestLength ? turnLength : longestLength;
				break;
			case TOP_RIGHT:
				turnLength = scanDiagonal(grid, newCoords, BOT_RIGHT, true) + firstBranchLength - 1;
				longestLength = turnLength > longestLength ? turnLength : longestLength;
				break;
			case BOT_LEFT:
				turnLength = scanDiagonal(grid, newCoords, TOP_LEFT, true) + firstBranchLength - 1;
				longestLength = turnLength > longestLength ? turnLength : longestLength;
				break;
		}

		newCoords = addCoords(newCoords, deltaCoords);
		firstBranchLength--;
	}

	return longestLength;
}

int getLongestVDiagonal(struct Grid grid, struct Coords coordsOf1)
{
	int currentLongest = 1;
	int longestVDiagonal = currentLongest;

	// TOP LEFT
	currentLongest = scanDiagonal(grid, coordsOf1, TOP_LEFT, false);
	longestVDiagonal = currentLongest > longestVDiagonal ? currentLongest : longestVDiagonal;

	// TOP RIGHT
	currentLongest = scanDiagonal(grid, coordsOf1, TOP_RIGHT, false);
	longestVDiagonal = currentLongest > longestVDiagonal ? currentLongest : longestVDiagonal;
	
	//BOT LEFT
	currentLongest = scanDiagonal(grid, coordsOf1, BOT_LEFT, false);
	longestVDiagonal = currentLongest > longestVDiagonal ? currentLongest : longestVDiagonal;

	//BOT RIGHT
	currentLongest = scanDiagonal(grid, coordsOf1, BOT_RIGHT, false);
	longestVDiagonal = currentLongest > longestVDiagonal ? currentLongest : longestVDiagonal;

	return longestVDiagonal;
}

int lenOfVDiagonal(int** grid, int gridSize, int* gridColSize) 
{
	int longestVDiagonal = 0;
	struct Grid compactGrid = {grid, gridSize, gridColSize};

	for (int rowNum = 0; rowNum < compactGrid.numRows; rowNum++)
	{
		for (int colNum = 0; colNum < compactGrid.numCols[rowNum]; colNum++)
		{
			if (compactGrid.cells[rowNum][colNum] == 1)
			{
				struct Coords coords = {rowNum, colNum};
				int vDiagonal = getLongestVDiagonal(compactGrid, coords);
				longestVDiagonal = vDiagonal > longestVDiagonal ? vDiagonal : longestVDiagonal;
			}
		}
	}

	return longestVDiagonal;
}

int main()
{
	int array[5][5] = {{2,2,1,2,2},{2,0,2,2,0},{2,0,1,1,0},{1,0,2,2,2},{2,0,0,2,2}};
	int colNums[5] = {5, 5, 5, 5, 5};

	int** mallocedArray = malloc(5 * sizeof(int*));
	for (int i = 0; i < 5; i++)
	{
		mallocedArray[i] = malloc(5 * sizeof(int));
		memcpy(mallocedArray[i], array[i], 5 * sizeof(int));
		for (int j = 0; j < 5; j++)
		{
			printf("%i", mallocedArray[i][j]);
		}
		printf("\n");
	}

	int* mallocedCols = malloc(5 * sizeof(int));
	memcpy(mallocedCols, colNums, 5 * sizeof(int));

	printf("%i\n", lenOfVDiagonal(mallocedArray, 5, mallocedCols));
}

