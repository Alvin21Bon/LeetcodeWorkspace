#include "leetcode.h"

int numOfUnplacedFruits(int* fruits, int fruitsSize, int* baskets, int basketsSize) 
{
	int unplacedFruit = 0;
	for (int i = 0; i < fruitsSize; i++)
	{
		bool fruitsPlaced = false;
		for (int j = 0; j < basketsSize; j++)
		{
			if (baskets[j] >= fruits[i])
			{
				baskets[j] = 0;
				fruitsPlaced = true;
				break;
			}
		}

		if (!fruitsPlaced) unplacedFruit++;
	}

	return unplacedFruit;
}
