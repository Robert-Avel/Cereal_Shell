#ifndef ARGLIST_HPP
#define ARGLIST_HPP


#include "ArgToken2.h"
#include <stddef.h>

#define EXPANTION_TAX 10


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

int ArgList_append(struct ArgList* ab, const struct NArgToken* at);
int ArgList_appendManual(struct ArgList* ab, enum KeyType kt, const char* key, enum DataType at, const char* arg);

int ArgList_remove(struct ArgList* ab, size_t index);


struct ArgListIterator {
    const struct _ArgListNode* _head;
    const struct _ArgListNode* iterator;
};


void ArgListIterator_init(struct ArgListIterator* ali, const struct ArgList* al);
int ArgListIterator_jump(struct ArgListIterator* ali);

#endif
