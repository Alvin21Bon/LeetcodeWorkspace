#include "leetcode.h"

struct ListNode {
	int val;
	struct ListNode* next;
};

// ##########################################################################

struct LinkedList {
	struct ListNode* head;
	struct ListNode* rear;
};

struct LinkedList createLinkedList()
{
	struct LinkedList linkedList;
	linkedList.head = NULL;
	linkedList.rear = NULL;

	return linkedList;
}

void linkedListAdd(struct LinkedList* linkedList, struct ListNode* nodeToAdd)
{
	if (linkedList->head == NULL)
	{
		linkedList->head = nodeToAdd;
		linkedList->rear = nodeToAdd;
		return;
	}

	linkedList->rear->next = nodeToAdd;
	linkedList->rear = nodeToAdd;
}

struct ListNode* mergeLists(struct ListNode* left, struct ListNode* right)
{
	struct LinkedList mergedLinkedList = createLinkedList();

	while (left != NULL || right != NULL)
	{
		if (left == NULL)
		{
			linkedListAdd(&mergedLinkedList, right);
			right = right->next;
		}
		else if (right == NULL)
		{
			linkedListAdd(&mergedLinkedList, left);
			left = left->next;
		}
		else if (left->val <= right->val)
		{
			linkedListAdd(&mergedLinkedList, left);
			left = left->next;
		}
		else
		{
			linkedListAdd(&mergedLinkedList, right);
			right = right->next;
		}
	}

	return mergedLinkedList.head;
}

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) 
{
	if (listsSize == 0) return NULL;

	while (listsSize > 1)
	{
		for (int idx = 0, newIdx = 0; idx < listsSize; idx += 2, newIdx++)
		{
			if (idx == listsSize - 1) lists[newIdx] = lists[idx];
			else lists[newIdx] = mergeLists(lists[idx], lists[idx+1]);
		}

		listsSize = (listsSize / 2) + (listsSize % 2);
	}

	return lists[0];
}
