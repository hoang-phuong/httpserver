#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int HashTable_MapKeyToIndex(char* key, unsigned int size);
static unsigned int HashTable_HashFunction(char* key);
static unsigned int crc32b(unsigned char *message);
static int callback(int value, CallbackArgsT args);

static unsigned int crc32b(unsigned char *message) {
   int i, j;
   unsigned int byte, crc, mask;

   i = 0;
   crc = 0xFFFFFFFF;
   while (message[i] != 0) {
      byte = message[i];            // Get next byte.
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) {    // Do eight times.
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
      i = i + 1;
   }
   return ~crc;
}

static unsigned int HashTable_HashFunction(char* key)
{
    return crc32b((unsigned char*)key);
}

static unsigned int HashTable_MapKeyToIndex(char* key, unsigned int size)
{
    return HashTable_HashFunction(key) & (size-1);
}

HashTableT* HashTable_New(unsigned int size)
{
    HashTableT* table = malloc(sizeof(HashTableT));
    
    if (!table)
    {
        fprintf(stderr, "Failed to allocate memory");
        return NULL_PTR;
    }

    table->data = malloc(sizeof(LinkedListT) * size);

    if (!table->data)
    {
        fprintf(stderr, "Failed to allocate memory");
        free(table);
        return NULL_PTR;
    }

    unsigned int i;
    
    for (i = 0; i < size; i++)
    {
        table->data[i].head = NULL_PTR;
    }

    table->size = size;

    return table;
}

static int callback(int value, CallbackArgsT args)
{
    HashTableItemT * item = (HashTableItemT *)(long)value;
    if (strcmp(item->key, args.argv[0]))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int HashTable_Add(HashTableT* table, char* key, long value)
{
    if(!HashTable_Get(table, key, NULL_PTR))
    {
        fprintf(stderr, "Key \"%s\" already exists\r\n", key);
        return 2;
    }
    unsigned int index = HashTable_MapKeyToIndex(key, table->size);

    HashTableItemT * item = malloc(sizeof(HashTableItemT));

    if(!item)
    {
        fprintf(stderr, "Failed to allocate memory");
        return 1;
    }

    memcpy(item->key, key, HASH_TABLE_MAX_KEY_LENGTH);
    item->value = value;

    if(LinkedList_Append(&table->data[index], (int)(long)item))
    {
        return 0;
    }
    else
    {
        fprintf(stderr, "Failed to add item to linked list");
        return 1;
    }
}



int HashTable_Get(HashTableT* table, char* key, long* value)
{
    unsigned int index = HashTable_MapKeyToIndex(key, table->size);
    CallbackArgsT args;

    args.count = 1;
    void** argv = malloc(sizeof(void*));
    argv[0] = key;
    args.argv = argv;

    NodeT* node = LinkedList_FindBy(&table->data[index], callback, args);

    if(node)
    {
        HashTableItemT * item = (HashTableItemT *)(long)node->val;
        if(value) *value = item->value;
        return 0;
    }
    else
    {
        return 1;
    }
}