#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "krutils/kr_alloc.h"
#include "krutils/kr_hashtable.h"

#define RECORD_NUMBER 100

#define KR_LONG_MAX      (2147483647L)


typedef struct _record {
    int iKey;
    long lKey;
    double dKey;
    char caKey[20];
    char caValue[100];
}T_Record;

static inline void PrintRecord(T_Record *ptRecord)
{
    printf("iKey=[%d], lKey=[%ld], dKey=[%lf], caKey=[%s], caValue=[%s]\n", \
           ptRecord->iKey, ptRecord->lKey, ptRecord->dKey, 
           ptRecord->caKey, ptRecord->caValue);
}

static inline void PrintHTSize(T_KRHashTable *ptHashTable)
{
    printf("htsize = [%u]\n", kr_hashtable_size(ptHashTable));
}


void MYHFunc(void *key, void *value, void *user_data)
{
    T_Record *ptRecord = value;
    int iLimit = *(int *)user_data;
    if (ptRecord->iKey > iLimit)
        PrintRecord(ptRecord);
}


int main()
{
    int i = 0;
    int ret = 0;
    T_Record *ptRecords;
    T_Record *ptRecordFind;
    
    ptRecords = kr_calloc(sizeof(T_Record) * RECORD_NUMBER);
    assert(ptRecords != NULL);
    
    for (i=0; i<RECORD_NUMBER; i++) {
        ptRecords[i].iKey = i;
        ptRecords[i].lKey = KR_LONG_MAX-i;
        ptRecords[i].dKey = i;
        snprintf(ptRecords[i].caKey, 20, "key_%05d", i);
        snprintf(ptRecords[i].caValue, 20, "value_%05d", i);
    }
        
    /*direct hash*/
    T_KRHashTable *ptdirectHash = kr_hashtable_new(NULL, NULL);
    assert(ptdirectHash != NULL);
    for (i=0; i<RECORD_NUMBER; i++) {
        kr_hashtable_insert(ptdirectHash, &ptRecords[i], &ptRecords[i]);
    }
    
    ptRecordFind = kr_hashtable_lookup(ptdirectHash, &ptRecordFind);
    assert(ptRecordFind == NULL);
    
    ptRecordFind = kr_hashtable_lookup(ptdirectHash, &ptRecords[50]);
    assert(ptRecordFind != NULL);
    PrintRecord(ptRecordFind);
    
    kr_hashtable_replace(ptdirectHash, &ptRecords[50], &ptRecords[99]);
    ptRecordFind = kr_hashtable_lookup(ptdirectHash, &ptRecords[50]);
    assert(ptRecordFind != NULL);
    PrintRecord(ptRecordFind);
    
    PrintHTSize(ptdirectHash);
    ret = kr_hashtable_remove(ptdirectHash, &ptRecords[50]);
    assert(ret);
    ptRecordFind = kr_hashtable_lookup(ptdirectHash, &ptRecords[99]);
    assert(ptRecordFind != NULL);
    PrintHTSize(ptdirectHash);
    
    kr_hashtable_remove_all(ptdirectHash);
    PrintHTSize(ptdirectHash);
    
    kr_hashtable_destroy(ptdirectHash);
    
    /*int hash*/
    T_KRHashTable *ptIntHash = kr_hashtable_new(kr_int_hash, kr_int_equal);
    assert(ptIntHash != NULL);
    for (i=0; i<RECORD_NUMBER; i++) {
        kr_hashtable_insert(ptIntHash, &ptRecords[i].iKey, &ptRecords[i]);
    }
    int iLimit= 55;
    kr_hashtable_foreach(ptIntHash, MYHFunc, &iLimit);
    kr_hashtable_destroy(ptIntHash);
    
    /*int64 hash*/
    T_KRHashTable *ptLongHash = kr_hashtable_new(kr_long_hash, kr_long_equal);
    assert(ptLongHash != NULL);
    for (i=0; i<RECORD_NUMBER; i++) {
        kr_hashtable_replace(ptLongHash, &ptRecords[i].lKey, &ptRecords[i]);
    }
    kr_hashtable_destroy(ptLongHash);
    
    /*double hash*/
    T_KRHashTable *ptDoubleHash = kr_hashtable_new(kr_double_hash, kr_double_equal);
    assert(ptDoubleHash != NULL);
    for (i=0; i<RECORD_NUMBER; i++) {
        kr_hashtable_insert(ptDoubleHash, &ptRecords[i].dKey, &ptRecords[i]);
    }
    kr_hashtable_destroy(ptDoubleHash);
    
    /*string hash*/
    T_KRHashTable *ptStrHash = kr_hashtable_new(kr_string_hash, kr_string_equal);
    assert(ptStrHash != NULL);
    for (i=0; i<RECORD_NUMBER; i++) {
        kr_hashtable_insert(ptStrHash, ptRecords[i].caKey, &ptRecords[i]);
    }
    kr_hashtable_destroy(ptStrHash);
    
    return 0;
}

