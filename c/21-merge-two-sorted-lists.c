#include "leetcode.h"

struct ListNode {
	int val;
	struct ListNode *next;
};

struct LinkedList {
	struct ListNode* head;
	struct ListNode* rear;
};

struct LinkedList linkedListCreate()
{
	struct LinkedList list = {
		.head = NULL,
		.rear = NULL
	};

	return list;
}

void linkedListAdd(struct LinkedList* linkedList, struct ListNode* node)
{
	if (linkedList->head == NULL)
	{
		linkedList->head = node;
		linkedList->rear = node;
		return;
	}

	linkedList->rear->next = node;
	linkedList->rear = node;
}

// ################################################################################

struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) 
{
	struct LinkedList output = linkedListCreate();
	while (list1 != NULL || list2 != NULL)
	{
		if (list1 == NULL) linkedListAdd(&output, list2), list2 = list2->next;
		else if (list2 == NULL) linkedListAdd(&output, list1), list1 = list1->next;
		else if (list1->val <= list2->val) linkedListAdd(&output, list1), list1 = list1->next;
		else linkedListAdd(&output, list2), list2 = list2->next;
	}

	return output.head;
}
