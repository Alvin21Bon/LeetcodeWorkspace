#include "leetcode.h"

bool isPalindrome(int x) 
{
	if (x < 0) return false;

	unsigned long new = 0;
	for (int y = x; y != 0; y /= 10)
	{
		new *= 10;
		new += y % 10;
	}

	return (int)new == x;
}
