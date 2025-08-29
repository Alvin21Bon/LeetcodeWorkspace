#include "leetcode.h"

int cmp(const void* ptr1, const void* ptr2)
{
	int int1 = *(int*)ptr1;
	int int2 = *(int*)ptr2;

	if (int1 < int2) return -1;
	else if (int1 == int2) return 0;
	else return 1;
}

bool reorderedPowerOf2(int n) 
{
	int digits[10];
	char* formattedPowers[50];
	int numPowers = 0;
	memset(formattedPowers, 0, sizeof(formattedPowers));

	int upperBound = n > (INT_MAX / 10) || n * 10 > 1000000000 ? 1000000000 : n * 10;
	for (int powerOf2 = 1; powerOf2 <= upperBound; powerOf2 = powerOf2 << 1)
	{
		int temp = powerOf2;
		int numDigits = 0;
		for ( ; temp; numDigits++)
		{
			digits[numDigits] = temp % 10;
			temp /= 10;
		}

		qsort(digits, numDigits, sizeof(int), cmp);
		char* formattedValue = malloc(sizeof(char) * (numDigits + 1));
		for (int i = 0; i < numDigits; i++)
		{
			formattedValue[i] = digits[i] + '0';
		}
		formattedValue[numDigits] = '\0';

		formattedPowers[numPowers++] = formattedValue;
	}

	int numDigits = 0;
	for ( ; n; numDigits++)
	{
		digits[numDigits] = n % 10;
		n /= 10;
	}

	qsort(digits, numDigits, sizeof(int), cmp);
	char formattedValue[numDigits + 1];
	for (int i = 0; i < numDigits; i++)
	{
		formattedValue[i] = digits[i] + '0';
	}
	formattedValue[numDigits] = '\0';

	for (int i = 0; i < numPowers; i++)
	{
		if (strcmp(formattedPowers[i], formattedValue) == 0) 
		{
			return true;
		}

		free(formattedPowers[i]);
	}

	return false;
}

