#include "leetcode.h"

int possibleStringCount(char* word) 
{
	int numRepetitions = 0;
	for (int i = 1; word[i] != '\0'; i++)
	{
		if (word[i] == word[i - 1]) numRepetitions++;
	}

	return numRepetitions + 1;
}
