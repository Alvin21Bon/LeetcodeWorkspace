#include "leetcode.h"

char* findDifferentBinaryString(char** nums, int numsSize) 
{
	char* uniqueBinaryString = calloc(numsSize + 1, sizeof(char));
	uniqueBinaryString[numsSize] = '\0';

	for (int idx = 0; idx < numsSize; idx++)
	{
		char* binaryString = nums[idx];
		switch (binaryString[idx])
		{
			case '0':
				uniqueBinaryString[idx] = '1';
				break;
			case '1':
				uniqueBinaryString[idx] = '0';
				break;
		}
	}

	return uniqueBinaryString;
}
