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

struct HashTable createHashTable()
{
	struct HashTable hashTable;
	memset(hashTable.buckets, 0, sizeof(hashTable.buckets));

	return hashTable;
}

struct HashEntry* find(struct HashTable* table, int num)
{
	int hashedNum = hash(&num);
	struct HashEntry* hashEntry = table->buckets[hashedNum];

	if (hashEntry)
	{
		struct HashEntry* cur = hashEntry;
		while  (cur)
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
	struct HashEntry* hashEntry = table->buckets[hashedNum];

	if (hashEntry)
	{
		struct HashEntry* cur = hashEntry;
		while (cur->next != NULL)
	 	{
	 		if (cur->num == num) return;
	 		cur = cur->next;
	 	}

		struct HashEntry* entry = malloc(sizeof(struct HashEntry));
		entry->index = index;
		entry->num = num;
		entry->next = NULL;

		cur->next = entry;
		return;
	}

	struct HashEntry* entry = malloc(sizeof(struct HashEntry));
	entry->index = index;
	entry->num = num;
	entry->next = NULL;

	table->buckets[hashedNum] = entry;
}

int* twoSum(int* nums, int numsSize, int target, int* returnSize) 
{
	struct HashTable table = createHashTable();

	int* retArr = malloc(sizeof(int) * 2);
	*returnSize = 2;

	for (int i = 0; i < numsSize; i++)
	{
		int neededNum = target - nums[i];
		struct HashEntry* entry = find(&table, neededNum);
		if (entry)
		{
			retArr[0] = i;
			retArr[1] = entry->index;
			return retArr;
		}

		insert(&table, nums[i], i);
	}

	return NULL;
}

