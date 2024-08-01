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
    if (*headRef == NULL) return;

    ListNode *temp = *headRef, *prev;

    if (temp != NULL && temp->data == key)
    {
        *headRef = temp->next;
        if (temp->data != NULL) free(temp->data);
        // free(temp);
        return;
    }

    while (temp != NULL && temp->data != key)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;
    if (temp->data != NULL) free(temp->data);
    // free(temp);
}

void ListNodeRemoveActual(ListNode** headRef, void* key)
{
    if (*headRef == NULL) return;

    ListNode *temp = *headRef, *prev;

    if (temp != NULL && temp->data == key)
    {
        *headRef = temp->next;
        if (temp->data != NULL) free(temp->data);
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
    if (temp->data != NULL) free(temp->data);
    free(temp);
}

void ListRemoveAllNodes(ListNode** headRef)
{
    ListNode* current = *headRef;
    ListNode* next;

    while (current != NULL)
    {
        next = current->next;
        if (current->data != NULL) free(current->data);
        free(current);
        current = next;
    }

    *headRef = NULL;
}

int ListLength(ListNode** headRef)
{
   int length = 0;

    ListNode* current = *headRef;
    ListNode* next;

    while (current != NULL)
    {
        next = current->next;
        length++;
        current = next;
    }

   return length;
}