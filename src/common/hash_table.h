#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "linked_list.h"

#define HASH_TABLE_MAX_KEY_LENGTH       64

typedef LinkedListT* HashTableDataT;

typedef struct 
{
    char    key[HASH_TABLE_MAX_KEY_LENGTH];
    long    value;
} HashTableItemT;

typedef struct
{
    unsigned int    size;
    HashTableDataT  data;
} HashTableT;

HashTableT* HashTable_New(unsigned int size);

int HashTable_Add(HashTableT* table, char* key, long value);

int HashTable_Get(HashTableT* table, char* key, long* value);

#endif