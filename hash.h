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

//初始化
void HashInit(HashTable* hashtable,HashFunc hash_func);

//销毁
void HashDestroy(HashTable* hashtable);

//插入
void HashInsert(HashTable* hashtable,KeyType key,ValueType value);

//查找
int HashFind(HashTable* hashtable,KeyType key,ValueType* value);

//删除
void HashRemove(HashTable* hashtable,KeyType key);
