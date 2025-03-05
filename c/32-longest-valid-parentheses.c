#include "leetcode.h"

int longestValidParentheses(char* s) 
{
	const int LENGTH_OF_STRING = strlen(s);
	if (LENGTH_OF_STRING == 0) return 0;
	int openingStack[LENGTH_OF_STRING + 1];
	int openingStackLength = 0;

	int largestLength = 0;
	int sectionStart = 0;

	for (int idx = 0; idx < LENGTH_OF_STRING; idx++)
	{
		char parenthesis = s[idx];

		if (parenthesis == '(')
		{
			openingStack[openingStackLength] = idx;
			openingStackLength++;
		}
		else
		{
			if (openingStackLength == 0)
			{
				int currentLength = idx - sectionStart;
				largestLength = currentLength > largestLength ? currentLength : largestLength;
				sectionStart = idx + 1;
			}
			else
			{
				openingStackLength--;
			}
		}
	}

	if (openingStackLength != 0)
	{
		openingStack[openingStackLength] = LENGTH_OF_STRING;
		openingStackLength++;
		for (int idx = 0; idx < openingStackLength; idx++)
		{
			int currentLength;
			if (idx == 0) currentLength = openingStack[idx] - sectionStart;
			else currentLength = openingStack[idx] - openingStack[idx - 1] - 1;

			largestLength = currentLength > largestLength ? currentLength : largestLength;
		}
	}
	else
	{
		int currentLength = LENGTH_OF_STRING - sectionStart;
		largestLength = currentLength > largestLength ? currentLength : largestLength;
	}

	return largestLength;
}
