#include "leetcode.h"

bool isEven(int num)
{
	return num % 2 == 0;
}

bool canConstruct(char* s, int k) 
{
	int numLetters = 26;
	int charMap[numLetters];
	memset(charMap, 0, sizeof(charMap));

	int stringLen = 0;
	for (char* c = s; *c != '\0'; c++)
	{
		charMap[(*c - 'a')]++;
		stringLen++;
	}

	if (k > stringLen) return false;
	if (k == stringLen) return true;

	int numIsolates = 0;
	for (int i = 0; i < numLetters; i++)
	{
		if (!isEven(charMap[i])) numIsolates++;
	}

	if (numIsolates > k) return false;

	return true;
}

