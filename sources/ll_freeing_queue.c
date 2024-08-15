#include "linked_list.h"

ListNode* freeingQueueHead = NULL;

void FreeingQueuePush(ListNode* node)
{
    ListNodePush(&freeingQueueHead, node);
}

void FreeingQueueFreeAll()
{
    ListNode* current = freeingQueueHead;
    ListNode* next;

    while (current != NULL)
    {
        next = current->next;
        if (current->data != NULL)
        {
            ListNode* data = current->data;
            free(data->data);
            free(data);
        }
        free(current);
        current = next;
    }

    freeingQueueHead = NULL;
}
