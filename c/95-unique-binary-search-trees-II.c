#include "leetcode.h"

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
struct TreeNode** generateTrees(int n, int* returnSize) {
    
}

/* struct TestHash { */
/* 	int key1; */
/* 	int key2; */
/* 	int value; */
/* 	UT_hash_handle hh; */
/* }; */
/**/
/* int main() */
/* { */
/* 	struct TestHash *testHash = NULL, *testItem, *temp; */
/* 	testItem = calloc(1, sizeof(*testItem)); */
/**/
/* 	testItem->key1 = 1; */
/* 	testItem->key2 = 2; */
/* 	testItem->value = 3; */
/**/
/* 	size_t keyLen =   offsetof(struct TestHash, key2) */
/* 			+ sizeof(int) */
/* 			- offsetof(struct TestHash, key1); */
/**/
/* 	HASH_ADD(hh, testHash, key1, keyLen, testItem); */
/**/
/* 	testItem = calloc(1, sizeof(*testItem)); */
/* 	testItem->key1 = 4; */
/* 	testItem->key2 = 7; */
/* 	testItem->value = 5; */
/**/
/* 	HASH_ADD(hh, testHash, key1, keyLen, testItem); */
/**/
/* 	struct TestHash lookup = {0}; */
/* 	lookup.key1 = 4; */
/* 	lookup.key2 = 7; */
/**/
/* 	testItem = NULL; */
/* 	HASH_FIND(hh, testHash, &lookup, keyLen, testItem); */
/**/
/* 	if (testItem) printf("It worked! %d\n", testItem->value); */
/**/
/* 	return 0; */
/* } */
