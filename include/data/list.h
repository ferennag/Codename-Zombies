#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef void (*list_free_data)(void *);

typedef struct LCallbacks {
    list_free_data lfree;
} LCallbacks;

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    void *data;
} Node;

typedef struct List {
    Node *head;
    Node *tail;
    LCallbacks callbacks;
} List;

List *List_empty(LCallbacks *callbacks);
void List_destroy(List *list);
bool List_isEmpty(List *list);

void List_append(List *list, void *data);
void List_insert(List *list, void *data);
void List_forEach(List *list, void (*f)(void *));
unsigned int List_size(List *list);
void *List_at(List *list, unsigned int pos);
Node *List_findFirst(List *list, const void *object, int (*cmp)(const void *, const void*));
Node *List_findFirstBy(List *list, bool (*cmp)(const void *));
void List_insert_before(List *list, Node *node, void *data);
Node *List_first(List *list);
Node *List_last(List *list);

#endif