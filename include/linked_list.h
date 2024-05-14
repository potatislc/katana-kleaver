#ifndef MY_RAYLIB_GAME_LINKED_LIST_H
#define MY_RAYLIB_GAME_LINKED_LIST_H

#include <stdlib.h>
#include "ball.h"

typedef struct ListNode
{
    void *next;
    void* data;
} ListNode;

extern ListNode *head;

ListNode* create_list_node(void* data); // Creates a list node
void push_list_node(ListNode** headRef, void* newData); // Pushes that node to the top of the list
void remove_list_node(ListNode** headRef, void* key);
void print_list_node(ListNode* node);

#endif //MY_RAYLIB_GAME_LINKED_LIST_H
