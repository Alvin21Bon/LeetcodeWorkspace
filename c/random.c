#include "leetcode.h"

#define TABLE_SIZE 50021

int hash(const int* num)
{
	unsigned long h = 5381;
	const unsigned char *p = (unsigned char*)num;
	for (size_t i = 0; i < sizeof(int); i++)
	{
		h = h * 33 + p[i];
	}

	h %= TABLE_SIZE;
	return h;
}

struct HashEntry {
	int num;
	int index;
	struct HashEntry* next;
};

struct HashTable {
	struct HashEntry* buckets[TABLE_SIZE];
};

struct HashTable createHash()
{
	struct HashTable table;
	memset(table.buckets, 0, sizeof(table.buckets));
	return table;
}

struct HashEntry* find(struct HashTable* table, int num)
{
	int hashedNum = hash(&num);
	struct HashEntry* entry = table->buckets[hashedNum];

	if (entry)
	{
		struct HashEntry* cur = entry;
		while (cur)
		{
			if (cur->num == num) return cur;
			cur = cur->next;
		}
	}

	return NULL;
}

void insert(struct HashTable* table, int num, int index)
{
	int hashedNum = hash(&num);
	struct HashEntry* entry = table->buckets[hashedNum];

	struct HashEntry* newEntry = malloc(sizeof(struct HashEntry));
	newEntry->index = index;
	newEntry->num = num;

	if (entry)
	{
		newEntry->next = entry;
		table->buckets[hashedNum] = newEntry;
		return;
	}

	newEntry->next = NULL;
	table->buckets[hashedNum] = newEntry;
}

int* twoSum(int* nums, int numsSize, int target, int* returnSize) 
{
	struct HashTable table = createHash();
	int* ret = malloc(2 * sizeof(int));
	*returnSize = 2;

	for (int i = 0; i < numsSize; i++)
	{
		int num = nums[i];
		int neededNum = target - num;
		struct HashEntry* neededEntry = find(&table, neededNum);
		if (neededEntry)
		{
			ret[0] = neededEntry->index;
			ret[1] = i;
			return ret;
		}

		insert(&table, num, i);
	}

	return NULL;
}

