#include <stdlib.h>
#include <string.h>
#include "data/hashmap.h"
#include "data/list.h"

#define HASHMAP_SIZE 100

void HashMap_default_free(void *object) {
    free(object);
    object = NULL;
}

size_t HashMap_default_hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

Bucket *Bucket_create(FreeFunction freeFunc, const char *key, void *value) {
    Bucket *bucket = (Bucket *) malloc(sizeof(Bucket));

    bucket->key = key;
    bucket->value = value;
    bucket->hFree = freeFunc;

    return bucket;
}

void Bucket_destroy(void *object) {
    Bucket *bucket = (Bucket *)object;
    
    if(NULL != bucket->value) {
        (*bucket->hFree)(bucket->value);
    }

    free(bucket);
    object = NULL;
}

int Bucket_cmp(const void *b1, const void *b2) {
    char *key = (char *) b1;
    Bucket *bucket2 = (Bucket *) b2;
    return strcmp(key, bucket2->key);
}

HashMap *HashMap_empty(FreeFunction freeFunc) {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->size = HASHMAP_SIZE;
    map->buckets = calloc(map->size, sizeof(List *));
    
    LCallbacks callbacks = {.lfree=Bucket_destroy};

    for(int i = 0; i < map->size; ++i) {
        map->buckets[i] = List_empty(&callbacks);
    }

    map->hFree = freeFunc != NULL ? freeFunc : HashMap_default_free;
    map->hash = HashMap_default_hash;

    return map;
}

void HashMap_destroy(HashMap *map) {
    if(map == NULL) {
        return;
    }

    for(int i = 0; i < map->size; ++i) {
        List_destroy(map->buckets[i]);
    }

    free(map->buckets);
    free(map);
}

size_t HashMap_hash(HashMap *map, const char *key) {
    size_t hash = (*map->hash)(key);
    return hash % map->size;
}

void HashMap_put(HashMap *map, const char *key, void *value) {
    size_t hash = HashMap_hash(map, key);

    Bucket *bucket = Bucket_create(map->hFree, key, value);
    List_append(map->buckets[hash], bucket);
}

void *HashMap_get(HashMap *map, const char *key) {
    size_t hash = HashMap_hash(map, key);
    List *buckets = map->buckets[hash];

    Node *node = List_findFirst(buckets, key, Bucket_cmp);
    return node ? ((Bucket *)node->data)->value : NULL;
}