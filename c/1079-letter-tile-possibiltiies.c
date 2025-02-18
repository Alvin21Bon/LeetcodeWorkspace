#include "leetcode.h"

int factorial(int num)
{
	#define FACTORIAL_7 5040
	#define FACTORIAL_6 720
	#define FACTORIAL_5 120
	#define FACTORIAL_4 24
	#define FACTORIAL_3 6
	#define FACTORIAL_2 2
	#define FACTORIAL_1 1
	#define FACTORIAL_0 1

	switch (num)
	{
		case 0:
			return FACTORIAL_0;
			break;
		case 1:
			return FACTORIAL_1; 
			break;
		case 2:
			return FACTORIAL_2; 
			break;
		case 3:
			return FACTORIAL_3; 
			break;
		case 4:
			return FACTORIAL_4; 
			break;
		case 5:
			return FACTORIAL_5; 
			break;
		case 6:
			return FACTORIAL_6; 
			break;
		case 7:
			return FACTORIAL_7; 
			break;
		default:
			return -1;
	}
}

int combinations(int n, int k)
{
	return factorial(n) / (factorial(k) * factorial(n - k));
}

int numFromBucketPicks(int* bucketPickValues, int arrayLen, int targetSubstringLength)
{
	int n = targetSubstringLength;
	int result = 1;
	for (int idx = 0; idx < arrayLen; idx++)
	{
		int k = bucketPickValues[idx];
		result *= combinations(n, k);
		n -= k;
	}

	return result;
}

int numTilePossibilities(char* tiles) 
{
	#define NUM_OF_LETTERS 26
	#define MAX_NUM_OF_BUCKETS 7

	int charToIdx[NUM_OF_LETTERS];
	int bucketPickValues[MAX_NUM_OF_BUCKETS] = {0};
	int numOfBuckets = 0;
	memset(charToIdx, 0xFF, NUM_OF_LETTERS * sizeof(int)); // initialize to all -1
	
	for (char* c = tiles; *c != '\0'; c++)
	{
		int charAsIdx = *c - 'A';
		if (charToIdx[charAsIdx] == -1)
		{
			charToIdx[charAsIdx] = numOfBuckets;
			bucketPickValues[numOfBuckets]++;
			numOfBuckets++;
		}
		else bucketPickValues[charToIdx[charAsIdx]]++;
	}

	int bucketPickTotal = 0;
	int result = 0;
	int bucketPickCombinations[MAX_NUM_OF_BUCKETS] = {0};
	while (memcmp(bucketPickValues, bucketPickCombinations, numOfBuckets * sizeof(int)) != 0)
	{
		for (int idx = numOfBuckets - 1; ; idx--)
		{
			bucketPickCombinations[idx]++;
			if (bucketPickCombinations[idx] <= bucketPickValues[idx]) 
			{
				bucketPickTotal++;
				break;
			}
			else
			{
				bucketPickTotal -= bucketPickValues[idx];
				bucketPickCombinations[idx] = 0;
			}
		}

		result += numFromBucketPicks(bucketPickCombinations, numOfBuckets, bucketPickTotal);
	}

	return result;
}
