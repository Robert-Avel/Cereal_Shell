#include "ArgToken.h"
#include <stddef.h>

#define EXPANTION_TAX 10


struct _ArgListNode {
    struct _ArgListNode* next_node;
    struct ArgToken at;
};

struct ArgListIterator {
    const struct _ArgListNode* _head;
    const struct _ArgListNode* iterator;
};


struct ArgList {
    struct _ArgListNode* head;
    size_t size;
    size_t allocated;
};


struct _ArgListNode* _ArgListNode_new(const struct ArgToken* at);
struct _ArgListNode* _ArgListNode_end(struct _ArgListNode* aln);

struct ArgList* ArgList_new();
int ArgList_end(struct ArgList* ab);

struct _ArgListNode* _ArgList_getNode(struct _ArgListNode* head, size_t index);
const struct ArgToken* ArgList_getArg(struct ArgList* al, size_t index);
const struct PosArg* ArgList_getPosArg(struct ArgList* al, size_t index);
const struct KeyArg* ArgList_getKeyArg(struct ArgList* al, size_t index);

int ArgList_append(struct ArgList* ab, const struct ArgToken* at);
int ArgList_appendPosArg(struct ArgList* ab, const struct PosArg* pa);
int ArgList_appendManualPosArg(struct ArgList* ab, const char* arg, size_t pos);
int ArgList_appendKeyArg(struct ArgList* ab, const struct KeyArg* ka);
int ArgList_appendManualKeyArg(struct ArgList* ab, const char* arg, const char* key);
int ArgList_remove(struct ArgList* ab, size_t index);

void ArgListIterator_init(struct ArgListIterator* ali, const struct ArgList* al);
int ArgListIterator_jump(struct ArgListIterator* ali);
