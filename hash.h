#pragma once
#include <stddef.h>
#define HashMaxSize 1000
typedef int KeyType;
typedef int ValueType;
typedef size_t (*HashFunc)(KeyType key);

typedef enum
{
    Empty,//空状态
    Valid,//有效状态
    Delete,//被删改的状态
}Stat;

typedef struct HashElem
{
    KeyType key;
    ValueType value;
    struct HashElem* next;
}HashElem;

typedef struct HashTable
{
    HashElem* data[HashMaxSize];
    size_t size;
    HashFunc func;
}HashTable;
