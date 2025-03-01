#include "leetcode.h"

int romanToInt(char* s) 
{
	char current, lookAhead;
	int result = 0;
	for (int idx = 0; s[idx] != '\0'; idx++)
	{
		current = s[idx];
		lookAhead = s[idx + 1];

		switch (current)
		{
			case 'I':
				switch (lookAhead)
				{
					case 'V':
						idx++;
						result += 4;
						break;
					case 'X':
						idx++;
						result += 9;
						break;
					default:
						result += 1;
						break;
				}
				break;
			case 'V':
				result += 5;
				break;
			case 'X':
				switch (lookAhead)
				{
					case 'L':
						idx++;
						result += 40;
						break;
					case 'C':
						idx++;
						result += 90;
						break;
					default:
						result += 10;
						break;
				}
				break;
			case 'L':
				result += 50;
				break;
			case 'C':
				switch (lookAhead)
				{
					case 'D':
						idx++;
						result += 400;
						break;
					case 'M':
						idx++;
						result += 900;
						break;
					default:
						result += 100;
						break;
				}
				break;
			case 'D':
				result += 500;
				break;
			case 'M':
				result += 1000;
				break;
		}
	}

	return result;
}
