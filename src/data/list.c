#include <stdlib.h>
#include <stdbool.h>
#include "data/list.h"

void list_default_free(void *data) {
    if(NULL != data) {
        free(data);
        data = NULL;
    }
} 

Node *Node_empty() {
    Node *node = (Node *) malloc(sizeof(Node));
    node->prev = NULL;
    node->next = NULL;
    node->data = NULL;
    return node;
}

List *List_empty(LCallbacks *callbacks) {
    Node *head = Node_empty();
    Node *tail = Node_empty();
    head->next = tail;
    tail->prev = head;

    List *list = (List *)malloc(sizeof(List));
    list->head = head;
    list->tail = tail;
    list->callbacks.lfree = list_default_free;

    if (callbacks != NULL && callbacks->lfree != NULL) {
        list->callbacks.lfree = callbacks->lfree;
    }

    return list;
}

bool List_isEmpty(List *list) {
    return list->head->next == list->tail;
}

void List_destroy(List *list) {
    if(list == NULL) {
        return;
    }

    Node *it = list->head;

    while (it != NULL) {
        Node *tmp = it->next;
        if(it->data != NULL) {
            list->callbacks.lfree(it->data); 
        }
        
        free(it);
        it = tmp;
    }

    list->head = NULL;
    list->tail = NULL;

    free(list);
}

void List_append(List *list, void *data) {
    Node *newnode = Node_empty();
    newnode->data = data;

    // Insert between the last item and the tail
    newnode->prev = list->tail->prev;
    newnode->next = list->tail;

    // Update the previous last item
    list->tail->prev->next = newnode;

    // Update the tail to point to the new node
    list->tail->prev = newnode;
}

void List_insert(List *list, void *data) {
    Node *newnode = Node_empty();
    newnode->data = data;

    // Insert between the first item and the tail
    newnode->prev = list->head;
    newnode->next = list->head->next;

    // Update the previous first item
    list->head->next->prev = newnode;

    // Update the head to point to the new node
    list->head->next = newnode;
}

void List_insert_before(List *list, Node *node, void *data) {
    Node *newnode = Node_empty();
    newnode->data = data;

    // Insert before the node
    newnode->prev = node->prev;
    newnode->next = node;

    // Update the previous first item
    node->prev->next = newnode;

    // Update the head to point to the new node
    node->prev = newnode;
}

void List_forEach(List *list, void (*f)(void *)) {
    if(list == NULL) {
        return;
    }

    Node *it = list->head->next;

    while (it != NULL) {
        (*f)(it->data);
        it = it->next;
    }
}

unsigned int List_size(List *list) {
    if (NULL == list) {
        return 0;
    }

    Node *it = list->head->next;

    unsigned int counter = 0;
    while(it != list->tail) {
        ++counter;
        it = it->next;
    }

    return counter;
}


void *List_at(List *list, unsigned int pos) {
    if (NULL == list) {
        return NULL;
    }

    Node *it = list->head->next;

    unsigned int counter = 0;
    while(it != list->tail) {
        if(counter == pos) {
            return it->data;
        }

        ++counter;
        it = it->next;
    }

    return NULL;
}

Node *List_findFirst(List *list, const void *object, int (*cmp)(const void *, const void*)) {
    if(list == NULL || list->head->next == list->tail) {
        return NULL;
    }

    Node *it = list->head->next;
    while(it != list->tail) {
        int is_eq = (*cmp)(object, it->data);
        if(is_eq == 0) {
            return it;
        }
        it = it->next;
    }
    
    return NULL;
}

Node *List_findFirstBy(List *list, bool (*cmp)(const void *)) {
    if(list == NULL || list->head->next == list->tail) {
        return NULL;
    }

    Node *it = list->head->next;
    while(it != list->tail) {
        if((*cmp)(it->data)) {
            return it;
        }
        it = it->next;
    }
    
    return NULL;
}

Node *List_first(List *list) {
    if(list->head->next != list->tail) {
        return list->head->next;
    } else {
        return NULL;
    }
}

Node *List_last(List *list) {
    if(list->tail->prev != list->head) {
        return list->tail->prev;
    } else {
        return NULL;
    }
}