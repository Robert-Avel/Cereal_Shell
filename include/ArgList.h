#ifndef ARGLIST_HPP
#define ARGLIST_HPP


#include "ArgToken2.h"
#include <stddef.h>

#define ITERATOR_TURN_AROUND 5


struct _ArgListNode {
    struct _ArgListNode* next_node;
    struct NArgToken* at;
};


struct ArgList {
    struct _ArgListNode* head;
    size_t size;
};


struct ArgList* ArgList_new();
int ArgList_end(struct ArgList* ab);

const struct NArgToken* ArgList_getArg(struct ArgList* al, size_t index);
struct NArgToken* ArgList_getArgPositional(struct ArgList* al, position pos);
struct NArgToken* ArgList_getArgNamed(struct ArgList* al, const char* key);


int ArgList_append(struct ArgList* ab, const struct NArgToken* at);
int ArgList_appendManual(struct ArgList* ab, enum KeyType kt, const void* key, enum DataType at, const void* arg);

int ArgList_remove(struct ArgList* ab, size_t index);


struct ArgListIterator {
    const struct _ArgListNode* _head;
    const struct _ArgListNode* iterator;
};


void ArgListIterator_init(struct ArgListIterator* ali, const struct ArgList* al);
int ArgListIterator_jump(struct ArgListIterator* ali);

#endif
