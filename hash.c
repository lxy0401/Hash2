#include "hash.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t HashFuncDefault(KeyType key)
{
    return key%HashMaxSize;
}

HashElem* CreateElem(KeyType key,ValueType value)
{
    HashElem* new_node = (HashElem*)malloc(sizeof(HashElem));
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    return new_node;

}

void DestroyElem(HashElem* cur)
{
    free(cur);
}

void HashInit(HashTable* hashtable,HashFunc hash_func)
{
    if(hashtable == NULL)
    {
        //非法操作
        return ;
    }
    hashtable->size = 0;
    hashtable->func = hash_func;
    size_t i = 0;
    for(;i < HashMaxSize ; ++i)
    {
        hashtable->data[i] = NULL;
    }
    return ;
}

void HashDestroy(HashTable* hashtable)
{
    if(hashtable == NULL)
    {
        return ;
    }
    hashtable->size = 0;
    hashtable->func = NULL;
    //遍历所有链表进行释放内存
    size_t i = 0;
    for(;i < HashMaxSize ; ++i)
    {
        HashElem* cur = hashtable->data[i];
        while(cur != NULL)
        {
            HashElem* next = cur->next;
            DestroyElem(cur);
        }
        cur = cur->next;
    }
    return ;
}

HashElem* HashBackedFind(HashElem* head,KeyType to_find)
{
    HashElem* cur = head;
    for(;cur != NULL ;cur = cur->next)
    {
        if(cur->key == to_find)
        {
            return cur ;
        }
    }
    return NULL;
}

void HashInsert(HashTable* hashtable,KeyType key,ValueType value)
{
    if(hashtable == NULL)
    {
        return ;
    }
    //1.根据key计算出offset
    size_t offset = hashtable->func(key);
    //2.在offset对应的链表中查找看当前的key是否存在,若存在就认为插入失败
    HashElem* ret = HashBackedFind(hashtable->data[offset],key);
    if(ret = NULL)
    {
        //找到了，说明找到了重复key的值，认为插入失败
        return ;
    }
    //3.若不存在就可以插入，使用头插法
    HashElem* new_node = CreateElem(key,value);
    //头插法
    new_node->next = hashtable->data[offset];
    hashtable->data[offset] = new_node;
    //4.++size
    ++hashtable->size;
    return;
}

int HashFind(HashTable* hashtable,KeyType key,ValueType* value)
{
    if(hashtable == NULL || value == NULL)
    {
        return 0;
    }
    //1.根据key计算出offset
    size_t offset = hashtable->func(key);
    //2.找到对应的offset的链表，遍历链表，尝试找到其中的元素
    HashElem* ret = HashBackedFind(hashtable->data[offset],key);
    if(ret == NULL)
    {
        return 0;
    }
    *value = ret->value;
    return 1;
}

int HashBackedFindEx(HashElem* head,KeyType to_find,HashElem** pre_node,HashElem** cur_node)
{
    HashElem* cur = head;
    HashElem* pre = NULL;
    for(;cur != NULL;pre = cur,cur = cur->next)
    {
        if(cur->key == to_find)
        {
            break;
        }
    }
    if(cur == NULL)
    {
        return 0;
    }
    *pre_node = pre;
    *cur_node = cur;
    return 1;
}

void HashRemove(HashTable* hashtable,KeyType key)
{
    if(hashtable == NULL)
    {
        return ;
    }
    if(hashtable->size == 0)
    {
        return ;
    }
    //1.根据key计算出offset
    size_t offset = hashtable->func(key);
    HashElem* pre = NULL;
    HashElem* cur = NULL;
    //2.通过 offset 找到对应的链表，在链表中找到指定的元素并进行删除
    int ret = HashBackedFindEx(hashtable->data[offset],key,&pre,&cur);
    if(cur == 0)
    {
        //没找到，删除失败
        return ;
    }
    if(pre == NULL)
    {
        //要删除的元素为链表的头节点
        hashtable->data[offset] = cur->next;
    }
    else
    {
        //不是头节点
        pre->next = cur->next;
    }
    DestroyElem(cur);
    //--size
    --hashtable->size;
    return ;
}

/*****
 *
 *以下为测试代码
 *
 *
 * *****/
#if 1
#include <stdio.h>
#define TEST_HEADER printf("\n========%s=========\n",__FUNCTION__)
void HashPrint(HashTable* hashtable,const char* msg)
{
    printf("[%s]\n",msg);
    size_t i = 0;
    for(;i < HashMaxSize ;++i)
    {
        if(hashtable->data[i] == NULL)
        {
            continue;
        }
        printf("i = %lu\n",i);
        HashElem* cur = hashtable->data[i];
        for(; cur != NULL ; cur = cur->next)
        {
            printf("[%d:%d]",cur->key,cur->value);
        }
        printf("\n");
    }
}

void TestInsert()
{
    TEST_HEADER;
    HashTable hashtable;
    HashInit(&hashtable,HashFuncDefault);
    HashInsert(&hashtable,1,1);
    HashInsert(&hashtable,1,10);
    HashInsert(&hashtable,2,2);
    HashInsert(&hashtable,1001,11);
    HashInsert(&hashtable,1002,12);
    HashPrint(&hashtable,"插入若干个元素");
}

void TestFind()
{
    TEST_HEADER;
    HashTable hashtable;
    HashInit(&hashtable,HashFuncDefault);
    HashInsert(&hashtable,1,1);
    HashInsert(&hashtable,1,10);
    HashInsert(&hashtable,2,2);
    HashInsert(&hashtable,1001,11);
    HashInsert(&hashtable,1002,12);
   int ret = 0;
   ValueType value;;
   ret = HashFind(&hashtable,1002,&value);
   printf("ret expected 1,actual %d\n",ret);
   printf("value expected 12,actual %d\n",value);
}

void TestRemove()
{
    TEST_HEADER;
    HashTable hashtable;
    HashInit(&hashtable,HashFuncDefault);
    HashInsert(&hashtable,1,1);
    HashInsert(&hashtable,1,10);
    HashInsert(&hashtable,2,2);
    HashInsert(&hashtable,1001,11);
    HashInsert(&hashtable,1002,12);
    HashRemove(&hashtable,2);
   int ret = 0;
   ValueType value;;
   ret = HashFind(&hashtable,1002,&value);
   printf("ret expected 1,actual %d\n",ret);
   printf("value expected 12,actual %d\n",value);
   ret = HashFind(&hashtable,2,&value);
   printf("ret expected 0,actual %d\n",ret);
}
int main()
{
    TestInsert();
    TestFind();
    TestRemove();
    return 0;
}
#endif
