#include "leetcode.h"

int reverse(int x)
{
	int reversed = 0;
	bool isPositive = x > 0;

	while (x != 0)
	{
		int digit = x % 10;
		x /= 10;

		if (isPositive)
		{
			if (reversed > (INT_MAX - digit) / 10) return 0;
		}
		else
		{
			if (reversed < (INT_MIN - digit) / 10) return 0;
		}

		reversed *= 10;
		reversed += digit;

	}

	return reversed;
}
