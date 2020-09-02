#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "std_types.h"

typedef struct
{
    unsigned int count;
    void**       argv;
} CallbackArgsT;

typedef int (*FindByCallbackT)(int val, CallbackArgsT args);

typedef struct NodeTag 
{
    long val;
    struct NodeTag * next;
} NodeT;

typedef struct LinkedListTag
{
    NodeT* head;
} LinkedListT;

LinkedListT* LinkedList_New(void);

void LinkedList_Delete(LinkedListT * list);

NodeT* LinkedList_Append(LinkedListT* list, int val);

int LinkedList_Remove(LinkedListT* list, NodeT* node);

NodeT* LinkedList_FindBy(LinkedListT* list, FindByCallbackT fn, CallbackArgsT args);

void LinkedList_Print(LinkedListT* list);

#endif