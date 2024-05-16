#ifndef BALL_GAME_LINKED_LIST_H
#define BALL_GAME_LINKED_LIST_H

#include <stdlib.h>
#include "ball.h"

typedef struct ListNode
{
    void *next;
    void* data;
} ListNode;

extern ListNode *head;

ListNode* ListNodeCreate(void* data);
void ListNodePush(ListNode** headRef, void* newData); // Pushes that node to the top of the list
void ListNodeRemove(ListNode** headRef, void* key);
void ListNodePrint(ListNode* node);

#endif //BALL_GAME_LINKED_LIST_H
