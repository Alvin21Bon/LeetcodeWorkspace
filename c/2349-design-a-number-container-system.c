#include "leetcode.h"

typedef struct {
	int* array;
	size_t length;
} NumberContainers;


NumberContainers* numberContainersCreate() 
{
	NumberContainers* numberContainer = malloc(sizeof(NumberContainers));
	numberContainer->length = 1024;
	numberContainer->array = malloc(numberContainer->length * sizeof(int));

	return numberContainer;
}

void numberContainersChange(NumberContainers* obj, int index, int number) 
{
	if ((size_t)index >= obj->length)
	{
		obj->length = index + 1024;
		obj->array = realloc(obj->array, obj->length * sizeof(int));
	}

	obj->array[index] = number;
}

int numberContainersFind(NumberContainers* obj, int number) 
{
	for (size_t i = 0; i < obj->length; i++)
	{
		if (obj->array[i] == number) return i;
	}

	return -1;
}

void numberContainersFree(NumberContainers* obj) {
	free(obj->array);
	free(obj);
}

/**
 * Your NumberContainers struct will be instantiated and called as such:
 * NumberContainers* obj = numberContainersCreate();
 * numberContainersChange(obj, index, number);
 
 * int param_2 = numberContainersFind(obj, number);
 
 * numberContainersFree(obj);
*/
