#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"
#include "std_types.h"

LinkedListT* LinkedList_New(void)
{
    LinkedListT* list = malloc(sizeof(LinkedListT));

    if(list)
    {
        list->head = NULL_PTR;
    }
    else
    {
        fprintf(stderr, "Failed to allocate memory");
    }

    return list;
}

void LinkedList_Delete(LinkedListT * list)
{
    if(list)
    {
        NodeT * ptr = list->head;

        if(ptr)
        {
            do
            {
                fprintf(stdout, "Deallocate node: %p\r\n", ptr);

                free(ptr);

                ptr = ptr->next;

            } while (ptr);
        }

        fprintf(stdout, "Deallocate list: %p\r\n", list);

        free(list);
    }
}

NodeT* LinkedList_Append(LinkedListT* list, int val)
{
    NodeT* pNode = malloc(sizeof(NodeT));

    if (!pNode)
    {
        fprintf(stderr, "Failed to allocate memory");
        return pNode;
    }

    pNode->val  = val;
    pNode->next = NULL_PTR;

    if (NULL_PTR != list->head)
    {
        NodeT* ptr = list->head;

        while(NULL_PTR != ptr->next)
        {
            ptr = ptr->next;
        }

        ptr->next = pNode;
    }
    else
    {
        list->head = pNode;
    }

    return pNode;
}

int LinkedList_Remove(LinkedListT* list, NodeT* node)
{
    NodeT* ptr;

    ptr = list->head;
    
    if ((NULL_PTR == ptr) || (NULL_PTR == node) || (NULL_PTR == list))
    {
        fprintf(stderr, "NULL parameters");
        return 1;
    }

    while((ptr->next != node) && (NULL_PTR != ptr->next))
    {
        ptr = ptr->next;
    }

    if (NULL_PTR == ptr->next)
    {
        fprintf(stderr, "Node doesn't belong to the linked list");
        return 2;
    }

    NodeT* nodeToDelete = ptr->next;

    ptr->next = nodeToDelete->next;

    free(nodeToDelete);

    return 0;
}

NodeT* LinkedList_FindBy(LinkedListT* list, FindByCallbackT fn, CallbackArgsT args)
{
    NodeT* ptr;

    ptr = list->head;

    if ((NULL_PTR == ptr) || (NULL_PTR == fn) || (NULL_PTR == list))
    {
        return NULL_PTR;
    }

    while (NULL_PTR != ptr)
    {
        if(fn(ptr->val, args))
        {
            return ptr;
        }
        else
        {
            ptr = ptr->next;
        }
    }

    return NULL_PTR;
}

void LinkedList_Print(LinkedListT* list)
{
    if(list)
    {
        NodeT * ptr = list->head;
        int i = 0;
        while(ptr)
        {
            fprintf(stdout, "Linked list: %p, node(%d): %p, value: %x, next: %p\r\n",
                    list,
                    i++,
                    ptr,
                    ptr->val,
                    ptr->next);

            ptr = ptr->next;
        }
    }
}