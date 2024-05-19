#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

ListNode* ListNodeCreate(void* data)
{
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void ListNodePush(ListNode** headRef, void* newData)
{
    ListNode* newNode = ListNodeCreate(newData);
    newNode->next = *headRef;
    *headRef = newNode;
}

void ListNodeRemove(ListNode** headRef, void* key)
{
    ListNode *temp = *headRef, *prev;

    if (temp != NULL && temp->data == key)
    {
        *headRef = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->data != key)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;
    free(temp);
}