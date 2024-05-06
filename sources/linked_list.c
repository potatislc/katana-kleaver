#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

struct ListNode *head = NULL;

// TODO: create_list_node function

void push_list_node(struct ListNode** headRef, void* newData)
{
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->data = newData;
    newNode->next = *headRef;
    *headRef = newNode;
}

void delete_list_node(struct ListNode** headRef, void* key)
{
    struct ListNode *temp = *headRef, *prev;

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