#include "leetcode.h"

struct ListNode {
	int val;
	struct ListNode *next;
};

// #################################

struct ListNode* reverseKGroup(struct ListNode* head, int k) 
{
	struct ListNode* reverseList[k];
	int kIncrement = 0;
	struct ListNode* outputLinkedList = NULL;
	struct ListNode* currentRear = NULL;

	for (struct ListNode* currentNode = head; currentNode != NULL; )
	{
		reverseList[kIncrement] = currentNode;
		kIncrement++;
		currentNode = currentNode->next;

		if (kIncrement == k)
		{
			if (currentRear != NULL) currentRear->next = reverseList[kIncrement - 1];
			else outputLinkedList = reverseList[kIncrement - 1];

			for (int idx = kIncrement - 1; idx >= 0; idx--)
			{
				if (idx == 0) reverseList[idx]->next = NULL;
				else reverseList[idx]->next = reverseList[idx - 1];
			}

			kIncrement = 0;

			currentRear = reverseList[0];
		}
	}

	if (kIncrement != 0)
		currentRear->next = reverseList[0];

	return outputLinkedList;
}
