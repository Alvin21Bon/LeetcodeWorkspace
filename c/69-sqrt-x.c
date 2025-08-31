#include "leetcode.h"

int mySqrt(int x) 
{
	int upper = x;
	int lower = 0;

	while (lower < upper)
	{
		long long middle = ((float)upper / 2) + ((float)lower / 2);
		long long middleSquared = middle * middle;
		if (middleSquared == x) return middle;
		if (middleSquared > x) upper = middle - 1;
		else lower = middle + 1;
	}

	if (lower * lower > x) return lower - 1;
	return lower;
}


