#include "leetcode.h"

int numEquivDominoPairs(int** dominoes, int dominoesSize, int* dominoesColSize) 
{
	int pairHash[10][10];
	memset(pairHash, 0, sizeof(pairHash));

	for (int i = 0; i < dominoesSize; i++)
	{
		int* domino = dominoes[i];
		int lowerNum, higherNum;
		if (domino[0] < domino[1])
		{
			lowerNum = domino[0];
			higherNum = domino[1];
		}
		else
		{
			lowerNum = domino[1];
			higherNum = domino[0];
		}

		pairHash[lowerNum][higherNum]++;
	}

	int numPairs = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int num = pairHash[i][j];
			numPairs += (num * (num - 1)) / 2;
		}
	}

	return numPairs;
}

