#include "leetcode.h"

struct Coords {
	int row;
	int column;
};

struct Coords coords(int row, int column)
{
	struct Coords coords;
	memset(&coords, 0, sizeof(coords)); // since this is used for key values

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

struct CoordinatesHash {
	struct Coords coords;
	int numPossibleNumbers;
	struct PossibleNumbersHash* possibleNumbersHashTable;
	UT_hash_handle hh;
};

struct CoordinatesHash* coordinatesHashCreate(struct Coords coords)
{
	struct CoordinatesHash* hashItem = calloc(1, sizeof(*hashItem));
	hashItem->coords = coords;
	hashItem->numPossibleNumbers = 9;
	possibleNumbersHashTableInit(&hashItem->possibleNumbersHashTable);

	return hashItem;
}

void coordinatesHashTableDestroy(struct CoordinatesHash** hashTable)
{
	struct CoordinatesHash *hashItem, *tmp;
	HASH_ITER(hh, *hashTable, hashItem, tmp)
	{
		HASH_DEL(*hashTable, hashItem);
		possibleNumbersHashTableDestroy(&hashItem->possibleNumbersHashTable);
		free(hashItem);
	}
}

bool coordinatesHashTableIsEmpty(struct CoordinatesHash** hashTable)
{
	return HASH_COUNT(*hashTable) == 0;
}

void coordinatesHashDeletePossibleNum(struct CoordinatesHash* coordinateHash, int possibleNumToDelete)
{
	if (possibleNumbersHashTableDelete(&coordinateHash->possibleNumbersHashTable, possibleNumToDelete))
	{
		coordinateHash->numPossibleNumbers--;
	}
}

void coordinatesHashTableAdd(struct CoordinatesHash** hashTable, struct CoordinatesHash* hashItem)
{
	HASH_ADD(hh, *hashTable, coords, sizeof(struct Coords), hashItem);
}

struct CoordinatesHash* coordinatesHashTableFind(struct CoordinatesHash** hashTable, struct Coords coords)
{
	struct CoordinatesHash lookup, *hashItem;
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

bool sudokuBoardIsInvalid(struct CoordinatesHash** hashTable)
{
	struct CoordinatesHash *hashItem, *tmp;
	HASH_ITER(hh, *hashTable, hashItem, tmp)
	{
		if (hashItem->numPossibleNumbers <= 0) return true;
	}

	return false;
}

void sudokuBoardFillInNumber(struct CoordinatesHash** hashTable, char** sudokuBoard, int numberToFillIn, struct Coords coordsToFill)
{
	sudokuBoard[coordsToFill.row][coordsToFill.column] = numberToFillIn + '0';

	const int NUM_SUDOKU_ROWS = 9, NUM_SUDOKU_COLUMNS = 9;
	struct CoordinatesHash* hashItem;
	// update column
	for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
	{
		hashItem = coordinatesHashTableFind(hashTable, coords(rowIdx, coordsToFill.column));
		if (hashItem)
		{
			coordinatesHashDeletePossibleNum(hashItem, numberToFillIn);
		}
	}

	// update row
	for (int columnIdx = 0; columnIdx < NUM_SUDOKU_COLUMNS; columnIdx++)
	{
		hashItem = coordinatesHashTableFind(hashTable, coords(coordsToFill.row, columnIdx));
		if (hashItem)
		{
			coordinatesHashDeletePossibleNum(hashItem, numberToFillIn);
		}
	}

	// update cell
	struct Coords cellCoords;
	cellCoords.row = coordsToFill.row / 3;
	cellCoords.column = coordsToFill.column / 3;
	for (int rowIdx = cellCoords.row * 3; rowIdx < (cellCoords.row + 1) * 3; rowIdx++)
	{
		for (int columnIdx = cellCoords.column * 3; columnIdx < (cellCoords.column + 1) * 3; columnIdx++)
		{
			hashItem = coordinatesHashTableFind(hashTable, coords(rowIdx, columnIdx));
			if (hashItem)
			{
				coordinatesHashDeletePossibleNum(hashItem, numberToFillIn);
			}
		}
	}
}

void coordinatesHashGenPossibleNumbers(struct CoordinatesHash* coordinateHash, char** sudokuBoard)
{
	const int NUM_SUDOKU_ROWS = 9, NUM_SUDOKU_COLUMNS = 9;

	// traverse the column
	for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
	{
		char square = sudokuBoard[rowIdx][coordinateHash->coords.column]; 
		if (square != '.')
		{
			coordinatesHashDeletePossibleNum(coordinateHash, square - '0');
		}
	}

	// traverse the row 
	for (int columnIdx = 0; columnIdx < NUM_SUDOKU_COLUMNS; columnIdx++)
	{
		char square = sudokuBoard[coordinateHash->coords.row][columnIdx];
		if (square != '.')
		{
			coordinatesHashDeletePossibleNum(coordinateHash, square - '0');
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
				coordinatesHashDeletePossibleNum(coordinateHash, square - '0');
			}
		}
	}
}

void coordinatesHashHandleIfGuaranteed(struct CoordinatesHash** hashTable, struct CoordinatesHash* hashItemToCheckForGuarantee, char** sudokuBoard)
{
	bool isGuarantee = hashItemToCheckForGuarantee->numPossibleNumbers == 1;
	if (isGuarantee)
	{
		// get info before destroying hash item
		int guaranteedNumber = hashItemToCheckForGuarantee->possibleNumbersHashTable->numberKey;
		struct Coords numberCoords = hashItemToCheckForGuarantee->coords;

		// destroy hash item
		HASH_DEL(*hashTable, hashItemToCheckForGuarantee);
		coordinatesHashDeletePossibleNum(hashItemToCheckForGuarantee, guaranteedNumber);
		free(hashItemToCheckForGuarantee);

		// fill in guaranteed number
		sudokuBoardFillInNumber(hashTable, sudokuBoard, guaranteedNumber, numberCoords);

		// handle any cascading guarantees
		const int NUM_SUDOKU_ROWS = 9, NUM_SUDOKU_COLUMNS = 9;
		struct CoordinatesHash* hashItem;

		// check column squares
		for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
		{
			hashItem = coordinatesHashTableFind(hashTable, coords(rowIdx, numberCoords.column));
			if (hashItem)
			{
				coordinatesHashHandleIfGuaranteed(hashTable, hashItem, sudokuBoard);
			}
		}

		// check row squares
		for (int columnIdx = 0; columnIdx < NUM_SUDOKU_COLUMNS; columnIdx++)
		{
			hashItem = coordinatesHashTableFind(hashTable, coords(numberCoords.row, columnIdx));
			if (hashItem)
			{
				coordinatesHashHandleIfGuaranteed(hashTable, hashItem, sudokuBoard);
			}
		}

		// check cell cquares
		struct Coords cellCoords;
		cellCoords.row = numberCoords.row / 3;
		cellCoords.column = numberCoords.column / 3;
		for (int rowIdx = cellCoords.row * 3; rowIdx < (cellCoords.row + 1) * 3; rowIdx++)
		{
			for (int columnIdx = cellCoords.column * 3; columnIdx < (cellCoords.column + 1) * 3; columnIdx++)
			{
				hashItem = coordinatesHashTableFind(hashTable, coords(rowIdx, columnIdx));
				if (hashItem)
				{
					coordinatesHashHandleIfGuaranteed(hashTable, hashItem, sudokuBoard);
				}
			}
		}
	}
}

void sudokuBoardGenMandatoryState(struct CoordinatesHash** hashTable, char** sudokuBoard)
{
	const int NUM_SUDOKU_ROWS = 9, NUM_SUDOKU_COLUMNS = 9;
	*hashTable = NULL;

	struct CoordinatesHash *hashItem;
	for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
	{
		for (int columnIdx = 0; columnIdx < NUM_SUDOKU_COLUMNS; columnIdx++)
		{
			char square = sudokuBoard[rowIdx][columnIdx];
			if (square == '.')
			{
				hashItem = coordinatesHashCreate(coords(rowIdx, columnIdx));
				coordinatesHashGenPossibleNumbers(hashItem, sudokuBoard);
				// optimization possible here, check if possible nums 0

				coordinatesHashTableAdd(hashTable, hashItem);
				coordinatesHashHandleIfGuaranteed(hashTable, hashItem, sudokuBoard);

			}
		}
	}
}

// ##############################################################

bool solveSudokuHelper(char** sudokuBoard)
{
	struct CoordinatesHash* hashTable;
	sudokuBoardGenMandatoryState(&hashTable, sudokuBoard);

	if (sudokuBoardIsInvalid(&hashTable)) 
	{
		coordinatesHashTableDestroy(&hashTable);
		return false;
	}
	if (coordinatesHashTableIsEmpty(&hashTable))
	{
		coordinatesHashTableDestroy(&hashTable);
		return true;
	}

	// since our board is not solved yet, hook onto a square and exhaust all possible nums
	struct CoordinatesHash *hashItemWithLeastPossibleNums = NULL, *coordinatesHashItem, *coordinatesTmp;
	HASH_ITER(hh, hashTable, coordinatesHashItem, coordinatesTmp)
	{
		if (hashItemWithLeastPossibleNums == NULL || coordinatesHashItem->numPossibleNumbers < hashItemWithLeastPossibleNums->numPossibleNumbers)
		{
			hashItemWithLeastPossibleNums = coordinatesHashItem;
		}
	}

	struct PossibleNumbersHash *numbersHashItem, *numbersTmp;
	HASH_ITER(hh, hashItemWithLeastPossibleNums->possibleNumbersHashTable, numbersHashItem, numbersTmp)
	{
		char** copyBoard = sudokuBoardCreateCopy(sudokuBoard);
		copyBoard[hashItemWithLeastPossibleNums->coords.row][hashItemWithLeastPossibleNums->coords.column] = numbersHashItem->numberKey + '0';

		bool isSudokuSolved = solveSudokuHelper(copyBoard);
		if (isSudokuSolved)
		{
			// copy the copied board into original board
			const int NUM_SUDOKU_ROWS = 9, NUM_SUDOKU_COLUMNS = 9;
			for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
			{
				memcpy(sudokuBoard[rowIdx], copyBoard[rowIdx], NUM_SUDOKU_COLUMNS * sizeof(char));
			}

			sudokuBoardFreeCopy(copyBoard);
			coordinatesHashTableDestroy(&hashTable);
			return true;
		}

		sudokuBoardFreeCopy(copyBoard);
	}

	// all options have been exhausted, meaning board in its given state is unsolvable
	coordinatesHashTableDestroy(&hashTable);
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

	printBoard(board);

	int bruh = 9;
	solveSudoku(board, 9, &bruh);
	printf("\n");

	printBoard(board);
}
