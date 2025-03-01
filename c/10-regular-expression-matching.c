#include "leetcode.h"

bool isMatch(char* s, char* p) 
{
	while (*s != '\0' || *p != '\0')
	{
		bool hasEncounteredStarWildcard = *p != '\0' ? *(p + 1) == '*' : false;

		if (hasEncounteredStarWildcard)
		{
			// check if 0 consumed is valid
			if (isMatch(s, p + 2)) return true;

			while ((*p == '.' && *s != '\0') || *s == *p)
			{
				s++;
				if (isMatch(s, p + 2)) return true;
			} 

			return false;
		}

		if ((*p == '.' && *s != '\0') || *s == *p)
		{
			s++; p++;
		}
		else return false;
	}

	return true;
}
