#include "leetcode.h"

struct Coords {
	int row;
	int column;
};

struct Coords coords(int row, int column)
{
	struct Coords coords;
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
		HASH_ADD_INT(*unitializedHashTable, numberKey, possibleNumbersHashCreate(num));
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
	else return false;
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

void sudokuCoordinatesHashDeletePossibleNumAndHandleGuarantees(struct SudokuCoordinatesHash** hashTable, struct SudokuCoordinatesHash* hashItem, int possibleNumToDelete, char** sudokuBoard);
void sudokuCoordinatesHashTableHandleGuarantees(struct SudokuCoordinatesHash** hashTable, struct SudokuCoordinatesHash* hashItemToCheckForGuarantee, char** sudokuBoard)
{
	bool isGuarantee = hashItemToCheckForGuarantee->numPossibleNumbers == 1;
	if (isGuarantee)
	{
		HASH_DEL(*hashTable, hashItemToCheckForGuarantee);

		int guaranteedNumber = hashItemToCheckForGuarantee->possibleNumbersHashTable->numberKey;
		sudokuCoordinatesHashDeletePossibleNum(hashItemToCheckForGuarantee, guaranteedNumber);

		struct Coords hashItemCoords = hashItemToCheckForGuarantee->coords;
		free(hashItemToCheckForGuarantee);
		sudokuBoard[hashItemCoords.row][hashItemCoords.column] = guaranteedNumber + '0';

		const int NUM_SUDOKU_ROWS = 9, NUM_SUDOKU_COLUMNS = 9;
		struct SudokuCoordinatesHash* hashItem;
		// traverse the row
		for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
		{
			hashItem = sudokuCoordinatesHashTableFind(hashTable, coords(rowIdx, hashItemCoords.column));
			if (hashItem)
			{
				sudokuCoordinatesHashDeletePossibleNumAndHandleGuarantees(hashTable, hashItem, guaranteedNumber, sudokuBoard);
			}
		}

		// traverse the column
		for (int columnIdx = 0; columnIdx < NUM_SUDOKU_COLUMNS; columnIdx++)
		{
			hashItem = sudokuCoordinatesHashTableFind(hashTable, coords(hashItemCoords.row, columnIdx));
			if (hashItem)
			{
				sudokuCoordinatesHashDeletePossibleNumAndHandleGuarantees(hashTable, hashItem, guaranteedNumber, sudokuBoard);
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
					sudokuCoordinatesHashDeletePossibleNumAndHandleGuarantees(hashTable, hashItem, guaranteedNumber, sudokuBoard);
				}
			}
		}
	}
}

void sudokuCoordinatesHashDeletePossibleNumAndHandleGuarantees(struct SudokuCoordinatesHash** hashTable, struct SudokuCoordinatesHash* hashItem, int possibleNumToDelete, char** sudokuBoard)
{
	sudokuCoordinatesHashDeletePossibleNum(hashItem, possibleNumToDelete);
	sudokuCoordinatesHashTableHandleGuarantees(hashTable, hashItem, sudokuBoard);
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


void solveSudoku(char** board, int boardSize, int* boardColSize) 
{
	const int NUM_SUDOKU_ROWS = 9, NUM_SUDOKU_COLUMNS = 9;
	struct SudokuCoordinatesHash *hashItem, *hashTable = NULL;

	for (int rowIdx = 0; rowIdx < NUM_SUDOKU_ROWS; rowIdx++)
	{
		for (int columnIdx = 0; columnIdx < NUM_SUDOKU_COLUMNS; columnIdx++)
		{
			hashItem = sudokuCoordinatesHashCreate(coords(rowIdx, columnIdx));
			sudokuCoordinatesHashFillInPossibleNumbers(hashItem, board);
			sudokuCoordinatesHashTableAdd(&hashTable, hashItem);
			sudokuCoordinatesHashTableHandleGuarantees(&hashTable, hashItem, board);
		}
	}
}
