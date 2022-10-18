#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include "list.h"

typedef size_t (*HashFunction)(const char*);
typedef void (*FreeFunction)(void*);

typedef struct Bucket {
    const char *key;
    void *value;
    FreeFunction hFree;
} Bucket;

typedef struct HashMap {
    List **buckets;
    size_t size;
    HashFunction hash;
    FreeFunction hFree;
} HashMap;

HashMap *HashMap_empty(FreeFunction freeFunc);
void HashMap_destroy(HashMap *hashMap);

void HashMap_put(HashMap *map, const char *key, void *value);
void *HashMap_get(HashMap *map, const char *key);

#endif