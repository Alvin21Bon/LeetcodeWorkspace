#include "leetcode.h"

int lengthOfLongestSubstring(char* s) 
{
	const int NUM_OF_ASCII_CHARS = 128;
	const int DEFAULT = -1;
	int usedCharArray[NUM_OF_ASCII_CHARS];
	memset(usedCharArray, DEFAULT, NUM_OF_ASCII_CHARS * sizeof(int)); // initialize every element to -1

	int traversalStartIndex = 0;
	int longestLength = 0;
	int currentLength = 0;

	char currentChar;
	int currentIdx = 0;
	while ((currentChar = s[currentIdx]) != '\0')
	{
		if (usedCharArray[currentChar] != DEFAULT)
		{
			if (usedCharArray[currentChar] >= traversalStartIndex)
			{
				currentLength = currentIdx - traversalStartIndex;
				longestLength = currentLength > longestLength ? currentLength : longestLength;

				traversalStartIndex = usedCharArray[currentChar] + 1;
			}
		}

		usedCharArray[currentChar] = currentIdx;
		currentIdx++;
	}

	currentLength = currentIdx - traversalStartIndex;
	longestLength = currentLength > longestLength ? currentLength : longestLength;

	return longestLength;
}

