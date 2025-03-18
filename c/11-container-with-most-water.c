#include "leetcode.h"

int maxArea(int* height, int heightSize) 
{
	int largestArea = 0;
	int* largerBars = height;
	int largerBarsSize = heightSize;

	while (true)
	{
		if (largerBarsSize < 2) break;
		int firstBarHeight = largerBars[0];
		if (firstBarHeight == 0) 
		{
			largerBars++;
			largerBarsSize--;
			continue;
		}
		int minPosWidthForGreaterArea = (largestArea / firstBarHeight) + 1;
		if (minPosWidthForGreaterArea >= largerBarsSize) 
		{
			largerBars++;
			largerBarsSize--;
			continue;
		}

		int currentLargestArea = 0;
		int newLargerBarsSize = 0;
		bool biggerBarFound = false;
		for (int idx = 1; idx < largerBarsSize; idx++)
		{
			int currentBarHeight = largerBars[idx];
			if (currentBarHeight > firstBarHeight) biggerBarFound = true;
			if (biggerBarFound) largerBars[newLargerBarsSize++] = currentBarHeight;

			int waterWidth = idx;
			int waterHeight = currentBarHeight < firstBarHeight ? currentBarHeight : firstBarHeight;
			int currentArea = waterWidth * waterHeight;
			currentLargestArea = currentArea > currentLargestArea ? currentArea : currentLargestArea;
		}

		largestArea = currentLargestArea > largestArea ? currentLargestArea : largestArea;
		largerBarsSize = newLargerBarsSize;
	}

	return largestArea;
}

int main()
{
	int ok[9] = {1,8,100,2,100,4,8,3,7};
	int* ok1 = malloc(9 * sizeof(int));
	memcpy(ok1, ok, 9 * sizeof(int));

	printf("%d", maxArea(ok1, 9));
	return 0;
}
