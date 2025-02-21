#include "leetcode.h"

struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
};

// ##################################################

struct TreeHash {
	int indexKey;
	UT_hash_handle hh;
};

void treeHashTableAdd(struct TreeHash** hashTable, int indexKey)
{
	struct TreeHash* hash = malloc(sizeof(*hash));
	hash->indexKey = indexKey;
	HASH_ADD_INT(*hashTable, indexKey, hash);
}

void treeHashTableDestroy(struct TreeHash** hashTable)
{
	struct TreeHash *currentItem, *tmpItem;
	HASH_ITER(hh, *hashTable, currentItem, tmpItem)
	{
		HASH_DEL(*hashTable, currentItem);
		free(currentItem);
	}
}

// ##################################################

struct TreeQueueItem {
	struct TreeNode* node;
	int index;
	struct TreeQueueItem* next;
};

struct TreeQueue {
	struct TreeQueueItem* front;
	struct TreeQueueItem* rear;
};

struct TreeQueue treeQueueInit()
{
	struct TreeQueue queue;
	queue.front = NULL;
	queue.rear = NULL;
	return queue;
}

void enqueue(struct TreeQueue* queue, struct TreeNode* nodeToQueue, int indexOfNode)
{
	struct TreeQueueItem* queueItem = malloc(sizeof(*queueItem));
	queueItem->node = nodeToQueue;
	queueItem->index = indexOfNode;
	queueItem->next = NULL;
	if (queue->front == NULL)
	{
		queue->front = queueItem;
		queue->rear = queueItem;
		return;
	}

	queue->rear->next = queueItem;
	queue->rear = queueItem;
}

struct TreeQueueItem* dequeue(struct TreeQueue* queue)
{
	struct TreeQueueItem* dequeuedItem = queue->front;
	queue->front = queue->front->next;
	if (queue->front == NULL) queue->rear = NULL;

	return dequeuedItem;
}

bool isQueueEmpty(struct TreeQueue* queue)
{
	return queue->front == NULL;
}

void treeQueueDestroy(struct TreeQueue* queue)
{
	struct TreeQueueItem* queueItem = queue->front;
	while (queueItem != NULL)
	{
		struct TreeQueueItem* currentItem = queueItem;
		queueItem = queueItem->next;
		free(currentItem);
	}
}

// ##################################################

typedef struct {
	struct TreeHash* hashTable;
} FindElements;


FindElements* findElementsCreate(struct TreeNode* root) 
{
	FindElements* findElements = malloc(sizeof(*findElements));
	findElements->hashTable = NULL;

	struct TreeQueue queue = treeQueueInit();
	if (root != NULL) enqueue(&queue, root, 0);

	while (!isQueueEmpty(&queue))
	{
		struct TreeQueueItem* queueItem = dequeue(&queue);

		treeHashTableAdd(&findElements->hashTable, queueItem->index);
		if (queueItem->node->left != NULL) enqueue(&queue, queueItem->node->left, queueItem->index * 2 + 1);
		if (queueItem->node->right != NULL) enqueue(&queue, queueItem->node->right, queueItem->index * 2 + 2);
		
		free(queueItem);
	}

	return findElements;
}

bool findElementsFind(FindElements* obj, int target) 
{
	struct TreeHash* hashItem;
	HASH_FIND_INT(obj->hashTable, &target, hashItem);
	return hashItem != NULL;
}

void findElementsFree(FindElements* obj) 
{
	treeHashTableDestroy(&obj->hashTable);
	free(obj);
}

/**
 * Your FindElements struct will be instantiated and called as such:
 * FindElements* obj = findElementsCreate(root);
 * bool param_1 = findElementsFind(obj, target);
 
 * findElementsFree(obj);
*/
