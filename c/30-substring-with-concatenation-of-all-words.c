#include "leetcode.h"

struct WordHash {
	char* wordKey;
	int numInstances;
	int NUM_INSTANCES;
	UT_hash_handle hh;
};

struct WordHash* wordHashCreate()
{
	struct WordHash* hashItem = malloc(sizeof(*hashItem));
	hashItem->numInstances = 1;
	hashItem->NUM_INSTANCES = 1;

	return hashItem;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* findSubstring(char* s, char** words, int wordsSize, int* returnSize) 
{
	struct WordHash *hashItem, *tmp, *wordHashTable = NULL;
	int LENGTH_OF_WORDS = strlen(words[0]);
	for (int idx = 0; idx < wordsSize; idx++)
	{
		HASH_FIND_STR(wordHashTable, words[idx], hashItem);
		if (hashItem) 
		{
			hashItem->numInstances++;
			hashItem->NUM_INSTANCES++;
		}
		else
		{
			hashItem = wordHashCreate();
			hashItem->wordKey = words[idx];
			HASH_ADD_KEYPTR(hh, wordHashTable, hashItem->wordKey, LENGTH_OF_WORDS, hashItem);
		}
	}

	*returnSize = 0;
	int LENGTH_OF_INPUT_STRING = strlen(s);
	int* returnArray = malloc(LENGTH_OF_INPUT_STRING * sizeof(int));
	int wordUsageIncrementer = 0;
	bool hashTableModified = false, discrepencyFound = false, validIdxFound = false;
	int TOTAL_WORD_CHARACTERS = wordsSize * LENGTH_OF_WORDS;

	for (int idx = 0; idx < LENGTH_OF_INPUT_STRING - TOTAL_WORD_CHARACTERS + 1; idx++)
	{
		for (int slidingPtr = idx; slidingPtr < LENGTH_OF_INPUT_STRING; slidingPtr += LENGTH_OF_WORDS)
		{
			char originalChar = s[slidingPtr + LENGTH_OF_WORDS];
			s[slidingPtr + LENGTH_OF_WORDS] = '\0';
			HASH_FIND_STR(wordHashTable, s + slidingPtr, hashItem);
			if (hashItem)
			{
				if (hashItem->numInstances != 0) 
				{	
					hashItem->numInstances--;
					wordUsageIncrementer++;
					validIdxFound = wordUsageIncrementer == wordsSize;
					hashTableModified = true;
				}
				else discrepencyFound = true;
			}
			else discrepencyFound = true;

			s[slidingPtr + LENGTH_OF_WORDS] = originalChar;
			if (discrepencyFound) break;
			if (validIdxFound)
			{
				returnArray[*returnSize] = idx;
				(*returnSize)++;
				break;
			}
		}

		if (hashTableModified)
		{
			HASH_ITER(hh, wordHashTable, hashItem, tmp)
			{
				hashItem->numInstances = hashItem->NUM_INSTANCES;
			}
		}

		hashTableModified = false;
		discrepencyFound = false;
		validIdxFound = false;
		wordUsageIncrementer = 0;
	}

	if (*returnSize == 0) 
	{
		free(returnArray);
		returnArray = NULL;
	}
	else
	{
		returnArray = realloc(returnArray, *returnSize * sizeof(int));
	}

	HASH_ITER(hh, wordHashTable, hashItem, tmp)
	{
		HASH_DEL(wordHashTable, hashItem);
		free(hashItem);
	}

	return returnArray;
}
