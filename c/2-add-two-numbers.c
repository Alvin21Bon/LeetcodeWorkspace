#include "leetcode.h"

struct ListNode {
	int val;
	struct ListNode* next;
};

////////////////////////////////////////////////////////

void ADD_LIST_NODE(struct ListNode** listHead, struct ListNode** listTail, struct ListNode* nodeToAdd)
{
	nodeToAdd->next = NULL; // just in case
	if (*listHead == NULL) // meaning linked list has not been established yet
	{
		*listHead = nodeToAdd;
		*listTail = nodeToAdd;
	}
	else if ((*listHead)->next == NULL) // linked list length of 1
	{
		(*listHead)->next = nodeToAdd;
		*listTail = nodeToAdd;
	}
	else
	{
		(*listTail)->next = nodeToAdd;
		(*listTail) = nodeToAdd;
	}
}

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) 
{
	struct ListNode* sumHead = NULL;
	struct ListNode* sumTail = NULL;
	struct ListNode* sumItem;

	bool shouldAddOne = false;
	int l1Digit, l2Digit;
	int digitSum;

	while (l1 != NULL || l2 != NULL)
	{
		l1Digit = l1 ? l1->val : 0;
		l2Digit = l2 ? l2->val : 0;
		digitSum = l1Digit + l2Digit + shouldAddOne;
		if (digitSum > 9)
		{
			digitSum -= 10;
			shouldAddOne = true;
		}
		else shouldAddOne = false;

		sumItem = malloc(sizeof(struct ListNode));
		sumItem->val = digitSum;
		sumItem->next = NULL;
		ADD_LIST_NODE(&sumHead, &sumTail, sumItem);

		l1 = l1 ? l1->next : NULL;
		l2 = l2 ? l2->next : NULL;
	}

	// catch should add one if both linked lists already reached end
	if (shouldAddOne)
	{
		sumItem = malloc(sizeof(struct ListNode));
		sumItem->val = 1;
		sumItem->next = NULL;
		ADD_LIST_NODE(&sumHead, &sumTail, sumItem);
	}
	return sumHead;
}
