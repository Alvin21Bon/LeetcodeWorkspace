#include "leetcode.h"

struct Coords {
	int row;
	int column;
};

struct Coords coords(int row, int column)
{
	struct Coords coords = {0};
	coords.row = row;
	coords.column = column;
	return coords;
}

// #########################################################

struct PossibleNumbersHash {
	int numberKey;
	UT_hash_handle hh;
};

struct PossibleNumbersHash* possibleNumbersHashCreate(int numberKey)
{
	struct PossibleNumbersHash* hashItem = malloc(sizeof(*hashItem));
	hashItem->numberKey = numberKey;

	return hashItem;
}

void possibleNumbersHashTableInit(struct PossibleNumbersHash** unitializedHashTable)
{
	*unitializedHashTable = NULL;

	const int MAX_SUDOKU_DIGIT = 9;
	for (int num = 1; num <= MAX_SUDOKU_DIGIT; num++)
	{
		struct PossibleNumbersHash* hashItem = possibleNumbersHashCreate(num);
		HASH_ADD_INT(*unitializedHashTable, numberKey, hashItem);
	}
}

bool possibleNumbersHashTableDelete(struct PossibleNumbersHash** hashTable, int numberKey)
{
	struct PossibleNumbersHash* hashItem;
	HASH_FIND_INT(*hashTable, &numberKey, hashItem);
	if (hashItem)
	{
		HASH_DEL(*hashTable, hashItem);
		free(hashItem);
		return true;
	}

	return false;
}

void possibleNumbersHashTableDestroy(struct PossibleNumbersHash** hashTable)
{
	struct PossibleNumbersHash *hashItem, *tmp;
	HASH_ITER(hh, *hashTable, hashItem, tmp)
	{
		HASH_DEL(*hashTable, hashItem);
		free(hashItem);
	}
}

// #########################################################

struct SudokuCoordinatesHash {
	struct Coords coords;
	int numPossibleNumbers;
	struct PossibleNumbersHash* possibleNumbersHashTable;
	UT_hash_handle hh;
};

struct SudokuCoordinatesHash* sudokuCoordinatesHashCreate(struct Coords coords)
{
	struct SudokuCoordinatesHash* hashItem = calloc(1, sizeof(*hashItem));
	hashItem->coords = coords;
	hashItem->numPossibleNumbers = 9;
	possibleNumbersHashTableInit(&hashItem->possibleNumbersHashTable);

	return hashItem;
}

void sudokuCoordinatesHashTableDestroy(struct SudokuCoordinatesHash** hashTable)
{
	struct SudokuCoordinatesHash *hashItem, *tmp;
	HASH_ITER(hh, *hashTable, hashItem, tmp)
	{
		HASH_DEL(*hashTable, hashItem);
		possibleNumbersHashTableDestroy(&hashItem->possibleNumbersHashTable);
		free(hashItem);
	}
}

void sudokuCoordinatesHashDeletePossibleNum(struct SudokuCoordinatesHash* coordinateHash, int possibleNumToDelete)
{
	if (possibleNumbersHashTableDelete(&coordinateHash->possibleNumbersHashTable, possibleNumToDelete))
	{
		coordinateHash->numPossibleNumbers--;
	}
}

void sudokuCoordinatesHashTableAdd(struct SudokuCoordinatesHash** hashTable, struct SudokuCoordinatesHash* hashItem)
{
	HASH_ADD(hh, *hashTable, coords, sizeof(struct Coords), hashItem);
}

struct SudokuCoordinatesHash* sudokuCoordinatesHashTableFind(struct SudokuCoordinatesHash** hashTable, struct Coords coords)
{
	struct SudokuCoordinatesHash lookup = {0}, *hashItem;
	lookup.coords = coords;
	HASH_FIND(hh, *hashTable, &lookup, sizeof(struct Coords), hashItem);

	return hashItem;
}

// #########################################################
// INTERACTING WITH SUDOKU BOARD

void printBoard(char** board)
{
	const int NUM_SUDOKU_ROWS = 9, NUM_SUDOKU_COLUMNS = 9;

	for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
	{
		for (int columnIdx = 0; columnIdx < NUM_SUDOKU_COLUMNS; columnIdx++)
		{
			printf("%c ", board[rowIdx][columnIdx]);
		}
		printf("\n");
	}
}

void sudokuCoordinatesHashTableHandleGuarantees(struct SudokuCoordinatesHash** hashTable, struct SudokuCoordinatesHash* hashItemToCheckForGuarantee, char** sudokuBoard)
{
	bool isGuarantee = hashItemToCheckForGuarantee->numPossibleNumbers == 1;
	if (isGuarantee)
	{
		int guaranteedNumber = hashItemToCheckForGuarantee->possibleNumbersHashTable->numberKey;
		struct Coords hashItemCoords = hashItemToCheckForGuarantee->coords;

		HASH_DEL(*hashTable, hashItemToCheckForGuarantee);
		sudokuCoordinatesHashDeletePossibleNum(hashItemToCheckForGuarantee, guaranteedNumber);
		free(hashItemToCheckForGuarantee);

		sudokuBoard[hashItemCoords.row][hashItemCoords.column] = guaranteedNumber + '0';

		const int NUM_SUDOKU_ROWS = 9, NUM_SUDOKU_COLUMNS = 9;
		struct SudokuCoordinatesHash* hashItem;

		// UPDATE POSSIBLE NUMBERS
		// traverse the row
		for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
		{
			hashItem = sudokuCoordinatesHashTableFind(hashTable, coords(rowIdx, hashItemCoords.column));
			if (hashItem)
			{
				sudokuCoordinatesHashDeletePossibleNum(hashItem, guaranteedNumber);
			}
		}

		// traverse the column
		for (int columnIdx = 0; columnIdx < NUM_SUDOKU_COLUMNS; columnIdx++)
		{
			hashItem = sudokuCoordinatesHashTableFind(hashTable, coords(hashItemCoords.row, columnIdx));
			if (hashItem)
			{
				sudokuCoordinatesHashDeletePossibleNum(hashItem, guaranteedNumber);
			}
		}

		// traverse the 3x3 cell
		struct Coords cellCoords;
		cellCoords.row = hashItemCoords.row / 3;
		cellCoords.column = hashItemCoords.column / 3;
		for (int rowIdx = cellCoords.row * 3; rowIdx < (cellCoords.row + 1) * 3; rowIdx++)
		{
			for (int columnIdx = cellCoords.column * 3; columnIdx < (cellCoords.column + 1) * 3; columnIdx++)
			{
				hashItem = sudokuCoordinatesHashTableFind(hashTable, coords(rowIdx, columnIdx));
				if (hashItem)
				{
					sudokuCoordinatesHashDeletePossibleNum(hashItem, guaranteedNumber);
				}
			}
		}
	}
}

void sudokuCoordinatesHashFillInPossibleNumbers(struct SudokuCoordinatesHash* coordinateHash, char** sudokuBoard)
{
	const int NUM_SUDOKU_ROWS = 9, NUM_SUDOKU_COLUMNS = 9;

	// traverse the row
	for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
	{
		char square = sudokuBoard[rowIdx][coordinateHash->coords.column]; 
		if (square != '.')
		{
			sudokuCoordinatesHashDeletePossibleNum(coordinateHash, square - '0');
		}
	}

	// traverse the column
	for (int columnIdx = 0; columnIdx < NUM_SUDOKU_COLUMNS; columnIdx++)
	{
		char square = sudokuBoard[coordinateHash->coords.row][columnIdx];
		if (square != '.')
		{
			sudokuCoordinatesHashDeletePossibleNum(coordinateHash, square - '0');
		}
	}

	// traverse the 3x3 cell
	struct Coords cellCoords;
	cellCoords.row = coordinateHash->coords.row / 3;
	cellCoords.column = coordinateHash->coords.column / 3;
	for (int rowIdx = cellCoords.row * 3; rowIdx < (cellCoords.row + 1) * 3; rowIdx++)
	{
		for (int columnIdx = cellCoords.column * 3; columnIdx < (cellCoords.column + 1) * 3; columnIdx++)
		{
			char square = sudokuBoard[rowIdx][columnIdx];
			if (square != '.')
			{
				sudokuCoordinatesHashDeletePossibleNum(coordinateHash, square - '0');
			}
		}
	}
}

char** sudokuBoardCreateCopy(char** originalBoard)
{
	const int NUM_SUDOKU_ROWS = 9, NUM_SUDOKU_COLUMNS = 9;
	char** copy = malloc(NUM_SUDOKU_ROWS * sizeof(char*));

	for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
	{
		copy[rowIdx] = malloc(NUM_SUDOKU_COLUMNS * sizeof(char));
		memcpy(copy[rowIdx], originalBoard[rowIdx], NUM_SUDOKU_COLUMNS * sizeof(char));
	}

	return copy;
}

void sudokuBoardFreeCopy(char** copyBoard)
{
	const int NUM_SUDOKU_ROWS = 9;
	for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
	{
		free(copyBoard[rowIdx]);
	}

	free(copyBoard);
}

// ##############################################################

bool solveSudokuHelper(char** board)
{
	printf("BOARD STATE:\n");
	printBoard(board);
	printf("\n");

	const int NUM_SUDOKU_ROWS = 9, NUM_SUDOKU_COLUMNS = 9;
	struct SudokuCoordinatesHash *hashItem, *tmp, *hashTable = NULL;

	// get MANDATORY board state
	for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
	{
		for (int columnIdx = 0; columnIdx < NUM_SUDOKU_COLUMNS; columnIdx++)
		{
			char square = board[rowIdx][columnIdx];
			if (square == '.')
			{
				hashItem = sudokuCoordinatesHashCreate(coords(rowIdx, columnIdx));
				sudokuCoordinatesHashFillInPossibleNumbers(hashItem, board);
				// if there is ever a square that has no options, then this is simply an invalid board state
				if (hashItem->numPossibleNumbers == 0)
				{
					free(hashItem);
					sudokuCoordinatesHashTableDestroy(&hashTable);
					return false;
				}

				sudokuCoordinatesHashTableAdd(&hashTable, hashItem);
			}
		}
	}

	bool hasReachedGuarantee = true;
	while (hasReachedGuarantee)
	{
		hasReachedGuarantee = false;
		HASH_ITER(hh, hashTable, hashItem, tmp)
		{
			if (hashItem->numPossibleNumbers <= 0)
			{
				sudokuCoordinatesHashTableDestroy(&hashTable);
				return false;
			}
			else if (hashItem->numPossibleNumbers == 1) hasReachedGuarantee = true;

			sudokuCoordinatesHashTableHandleGuarantees(&hashTable, hashItem, board);
		}
	}

	printf("MANDATORY STATE:\n");
	printBoard(board);
	printf("\n");

	if (HASH_COUNT(hashTable) == 0) return true;

	// if hash map not empty, then board is not solved, venture into options
	const int MAX_POSSIBLE_OPTIONS = 9;
	struct SudokuCoordinatesHash *coordinatesHashItem, *coordinatesTmp;
	struct PossibleNumbersHash *numbersHashItem, *numbersTmp;
	for (int targetNumOptions = 2; targetNumOptions <= MAX_POSSIBLE_OPTIONS; targetNumOptions++)
	{
		HASH_ITER(hh, hashTable, coordinatesHashItem, coordinatesTmp)
		{
			if (coordinatesHashItem->numPossibleNumbers == targetNumOptions)
			{
				HASH_ITER(hh, coordinatesHashItem->possibleNumbersHashTable, numbersHashItem, numbersTmp)
				{
					char** copyBoard = sudokuBoardCreateCopy(board);
					copyBoard[coordinatesHashItem->coords.row][coordinatesHashItem->coords.column] = numbersHashItem->numberKey + '0';

					bool isSudokuSolved = solveSudokuHelper(copyBoard);
					if (isSudokuSolved)
					{
						// copy the copied board into original board
						for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
						{
							memcpy(board[rowIdx], copyBoard[rowIdx], NUM_SUDOKU_COLUMNS * sizeof(char));
						}

						sudokuBoardFreeCopy(copyBoard);
						sudokuCoordinatesHashTableDestroy(&hashTable);
						return true;
					}
					else sudokuBoardFreeCopy(copyBoard);
				}
			}
		}
	}

	// all options have been exhausted, meaning board in its given state is unsolvable
	sudokuCoordinatesHashTableDestroy(&hashTable);
	return false;
}

void solveSudoku(char** board, int boardSize, int* boardColSize) 
{
	solveSudokuHelper(board);
}


int main()
{
	char structBoard[9][9] = {{'.','.','9','7','4','8','.','.','.'},{'7','.','.','.','.','.','.','.','.'},{'.','2','.','1','.','9','.','.','.'},{'.','.','7','.','.','.','2','4','.'},{'.','6','4','.','1','.','5','9','.'},{'.','9','8','.','.','.','3','.','.'},{'.','.','.','8','.','3','.','2','.'},{'.','.','.','.','.','.','.','.','6'},{'.','.','.','2','7','5','9','.','.'}};
	char** board = malloc(9 * sizeof(char*));
	for (int idx = 0; idx < 9; idx++)
	{
		board[idx] = malloc(9 * sizeof(char));
		memcpy(board[idx], structBoard[idx], 9 * sizeof(char));
	}

	/* printBoard(board); */

	int bruh = 9;
	solveSudoku(board, 9, &bruh);
	/* printf("\n"); */

	printBoard(board);
}
