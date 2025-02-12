#include "leetcode.h"

struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
};

// ##############################################

struct TreeNode* treeNodeCreate(int val, struct TreeNode* left, struct TreeNode* right)
{
	struct TreeNode* treeNode = malloc(sizeof(*treeNode));
	treeNode->val = val;
	treeNode->left = left;
	treeNode->right = right;

	return treeNode;
}

// ##############################################

struct BSTArray {
	struct TreeNode** array;
	size_t length;
	size_t allocatedLength;
};

struct BSTArray bstArrayCreateWithLength(size_t length)
{
	struct BSTArray bstArray;
	bstArray.array = malloc(length * sizeof(*(bstArray.array)));
	bstArray.allocatedLength = length;
	bstArray.length = 0;

	return bstArray;
}

struct BSTArray bstArray()
{
	const size_t DEFAULT_INITIAL_LENGTH = 64;
	return bstArrayCreateWithLength(DEFAULT_INITIAL_LENGTH);
}

void bstArrayAdd(struct BSTArray* bstArray, struct TreeNode* BST)
{
	if (bstArray->length == bstArray->allocatedLength)
	{
		bstArray->allocatedLength *= 2;
		bstArray->array = realloc(bstArray->array, bstArray->allocatedLength * sizeof(*(bstArray->array)));
	}

	bstArray->array[bstArray->length] = BST;
	bstArray->length++;
}

void bstArrayTrim(struct BSTArray* bstArray)
{
	bstArray->allocatedLength = bstArray->length;
	bstArray->array = realloc(bstArray->array, bstArray->allocatedLength * sizeof(*(bstArray->array)));
}

void bstArrayDestroy(struct BSTArray* bstArray)
{
	free(bstArray->array);
	bstArray->array = NULL;
	bstArray->allocatedLength = 0;
	bstArray->length = 0;
}

// ##############################################

struct BSTHash {
	int lowRangeKey;
	int highRangeKey;
	struct BSTArray uniqueBSTArray;
	UT_hash_handle hh;
};

struct BSTHash* bstHashItemCreateWithArrayLength(int lowRangeKey, int highRangeKey, size_t length)
{
	struct BSTHash* hashItem = calloc(1, sizeof(*hashItem));
	hashItem->lowRangeKey = lowRangeKey;
	hashItem->highRangeKey = highRangeKey;
	hashItem->uniqueBSTArray = bstArrayCreateWithLength(length);

	return hashItem;
}

struct BSTHash* bstHashItemCreate(int lowRangeKey, int highRangeKey)
{
	const size_t DEFAULT_INITIAL_ARRAY_LENGTH = 64;
	return bstHashItemCreateWithArrayLength(lowRangeKey, highRangeKey, DEFAULT_INITIAL_ARRAY_LENGTH);
}

void bstHashTableAdd(struct BSTHash** bstHashTable, struct BSTHash* bstHashItem)
{
	size_t hashKeyLen =   offsetof(struct BSTHash, highRangeKey)
			    + sizeof(int)
			    - offsetof(struct BSTHash, lowRangeKey);

	HASH_ADD(hh, *bstHashTable, lowRangeKey, hashKeyLen, bstHashItem);
}

struct BSTHash* bstHashTableFind(struct BSTHash** bstHashTable, int lowRangeKey, int highRangeKey)
{
	size_t hashKeyLen =   offsetof(struct BSTHash, highRangeKey)
			    + sizeof(int)
			    - offsetof(struct BSTHash, lowRangeKey);

	struct BSTHash lookup = {0}, *hashItem;
	lookup.lowRangeKey = lowRangeKey;
	lookup.highRangeKey = highRangeKey;

	HASH_FIND(hh, *bstHashTable, &lookup, hashKeyLen, hashItem);
	return hashItem;
}

void bstHashTableDestroy(struct BSTHash** bstHashTable)
{
	struct BSTHash *currentItem, *tmpItem;
	HASH_ITER(hh, *bstHashTable, currentItem, tmpItem)
	{
		HASH_DEL(*bstHashTable, currentItem);
		bstArrayDestroy(&currentItem->uniqueBSTArray);
		free(currentItem);
	}
}

// ##############################################

struct BSTArray generateTreesHelper(struct BSTHash** bstHashTable, int numLowRange, int numHighRange)
{
	// BASE CASE: calculated this subrange already
	struct BSTHash* hashItem = bstHashTableFind(bstHashTable, numLowRange, numHighRange);
	if (hashItem) return hashItem->uniqueBSTArray;

	// BASE CASE: subrange is just 1 number
	if (numLowRange == numHighRange)
	{
		struct TreeNode* singleBST = treeNodeCreate(numLowRange, NULL, NULL);
		hashItem = bstHashItemCreateWithArrayLength(numLowRange, numHighRange, 1);
		bstArrayAdd(&hashItem->uniqueBSTArray, singleBST);

		bstHashTableAdd(bstHashTable, hashItem);
		return hashItem->uniqueBSTArray;
	}
	
	// BASE CASE: invalid subrange values (this happens when root node increment in parent node is at either ends of the num range)
	if (numLowRange > numHighRange)
	{
		hashItem = bstHashItemCreateWithArrayLength(numLowRange, numHighRange, 1);
		bstArrayAdd(&hashItem->uniqueBSTArray, NULL); // this NULL value is added so that parent nodes left/right will link to NULL during linking
		
		bstHashTableAdd(bstHashTable, hashItem);
		return hashItem->uniqueBSTArray;
	}
	
	// RECURSIVE CASE: subrange is more than 1 number
	hashItem = bstHashItemCreate(numLowRange, numHighRange);
	for (int root = numLowRange; root <= numHighRange; root++)
	{
		struct BSTArray leftSubtreeUniqueBSTArray = generateTreesHelper(bstHashTable, numLowRange, root - 1);
		struct BSTArray rightSubtreeUniqueBSTArray = generateTreesHelper(bstHashTable, root + 1, numHighRange);

		for (size_t leftIdx = 0; leftIdx < leftSubtreeUniqueBSTArray.length; leftIdx++)
		{
			for (size_t rightIdx = 0; rightIdx < rightSubtreeUniqueBSTArray.length; rightIdx++)
			{
				struct TreeNode* newUniqueBST = treeNodeCreate(root, leftSubtreeUniqueBSTArray.array[leftIdx], rightSubtreeUniqueBSTArray.array[rightIdx]);
				bstArrayAdd(&hashItem->uniqueBSTArray, newUniqueBST);
			}
		}
	}

	bstHashTableAdd(bstHashTable, hashItem);
	return hashItem->uniqueBSTArray;
}

struct TreeNode** generateTrees(int n, int* returnSize) 
{
	struct BSTHash *bstHashTable = NULL, *finalResultHashItem;
	struct BSTArray allUniqueBSTArray = generateTreesHelper(&bstHashTable, 1, n);

	finalResultHashItem = bstHashTableFind(&bstHashTable, 1, n);
	HASH_DEL(bstHashTable, finalResultHashItem);
	free(finalResultHashItem);
	bstHashTableDestroy(&bstHashTable);

	bstArrayTrim(&allUniqueBSTArray);
	*returnSize = allUniqueBSTArray.length;
	return allUniqueBSTArray.array;
}

