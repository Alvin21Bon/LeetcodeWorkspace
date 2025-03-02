#include "leetcode.h"

char* longestPalindrome(char* s) 
{
	size_t largestLength = 0;
	char* substringLeftPointer;
	char* substringRightPointer;

	size_t inputStringLen = strlen(s);
	for (float poi = 0.5; poi < inputStringLen - 1; poi += 0.5)
	{
		char* poiLeftPointer;
		char* poiRightPointer;

		bool isPoiAWholeNumber = floor(poi) == ceil(poi);
		if (isPoiAWholeNumber)
		{
			poiLeftPointer = &s[(int)poi - 1];
			poiRightPointer = &s[(int)poi + 1];
		}
		else
		{
			poiLeftPointer = &s[(int)floor(poi)];
			poiRightPointer = &s[(int)ceil(poi)];
		}

		if (&s[inputStringLen] - poiRightPointer + 1 <= (long)largestLength / 2) break;

		size_t currentLength = 0;
		while ((poiLeftPointer >= s && poiRightPointer < &s[inputStringLen]) && (*poiLeftPointer == *poiRightPointer))
		{
			currentLength = poiRightPointer - poiLeftPointer + 1;
			poiLeftPointer--;
			poiRightPointer++;
		}

		if (currentLength > largestLength)
		{
			largestLength = currentLength;
			substringLeftPointer = poiLeftPointer + 1;
			substringRightPointer = poiRightPointer - 1;
		}
	}

	if (largestLength == 0)
	{
		s[1] = '\0';
		return s;
	}

	substringRightPointer[1] = '\0';
	return substringLeftPointer;
}

