#include "leetcode.h"

int cmp(const void* ptr1, const void* ptr2)
{
	int int1 = *(int*)ptr1;
	int int2 = *(int*)ptr2;

	if (int1 > int2) return -1;
	if (int1 == int2) return 0;
	return 1;
}

long long maximumImportance(int n, int** roads, int roadsSize, int* roadsColSize) 
{
	int cityNumRoadsHash[n];
	memset(cityNumRoadsHash, 0, sizeof(cityNumRoadsHash));
	for(int i = 0; i < roadsSize; i++)
	{
		int* road = roads[i];
		cityNumRoadsHash[road[0]]++;
		cityNumRoadsHash[road[1]]++;
	}
	qsort(cityNumRoadsHash, n, sizeof(int), cmp);

	long long totalImportance = 0;
	for (long long importance = n, i = 0; i < n; importance--, i++)
	{
		totalImportance += importance * cityNumRoadsHash[i];
	}

	return totalImportance;
}

