#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

struct ListNode* create_list_node(void* data)
{
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void push_list_node(struct ListNode** headRef, void* newData)
{
    struct ListNode* newNode = create_list_node(newData);
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