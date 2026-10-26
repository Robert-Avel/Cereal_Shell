#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ArgList.h"
#include "ArgToken2.h"


enum Status {
  SUCESS = 0,
  FAIL,
  INVALID_INPUT,
  ALLOCATION_FAIL,
  OUT_OF_RANGE,
  TURN_AROUND
};

struct _ArgListNode* _ArgListNode_new(const struct NArgToken* nat) {
    struct _ArgListNode* alb = malloc(sizeof(struct _ArgListNode));
    if(alb == NULL) {return NULL;}

    alb->next_node = NULL;
    alb->at = NArgToken_newConstruct(nat->key_type, &nat->key, nat->arg_type, &nat->arg);
    return alb;
}


struct _ArgListNode* _ArgListNode_end(struct _ArgListNode* aln) {
    if(aln == NULL) {return NULL;}
    struct _ArgListNode* next_node = aln->next_node;
    NArgToken_end(aln->at);
    free(aln);
    return next_node;
}


struct _ArgListNode* _ArgList_getNode(struct _ArgListNode* ald, size_t index) {
    if(ald == NULL) {return NULL;}

    if(index == 0) {
        return ald;
    } else {
        return _ArgList_getNode(ald->next_node, index -1);
    }
}


int _ArgListNode_endRecursive(struct _ArgListNode* head, size_t index) {
    if(head == NULL) {return FAIL;}

    if(index == 0) {
        return SUCESS;
    } else {
        return _ArgListNode_endRecursive(_ArgListNode_end(head), index -1);
    }
}


struct ArgList* ArgList_new() {
    struct ArgList* ab = (struct ArgList*) malloc(sizeof(struct ArgList));
    if(ab == NULL) {return NULL;}

    ab->head = NULL;
    ab->size = 0;
    return ab;
}

int ArgList_end(struct ArgList* ab) {
    if(ab == NULL) {return INVALID_INPUT;}

    _ArgListNode_endRecursive(ab->head, ab->size);
    free(ab);
    return SUCESS;
}


const struct NArgToken* ArgList_getArg(struct ArgList* al, size_t index) {
    if(al == NULL) {return NULL;}
    if(index >= al->size) {return NULL;}

    const struct _ArgListNode* node = _ArgList_getNode(al->head, index);
    if(node == NULL) {return NULL;}

    return node->at;
}



int ArgList_append(struct ArgList* ab, const struct NArgToken* at) {
    if(ab == NULL || at == NULL) {return INVALID_INPUT;}

    struct _ArgListNode* n_node = _ArgListNode_new(at);
    if(n_node == NULL) {return ALLOCATION_FAIL;}

    if(ab->head == NULL) {
        ab->head = n_node;
    } else {
        struct _ArgListNode* last_node = _ArgList_getNode(ab->head, ab->size - 1);
        if(last_node == NULL) {
            free(n_node);
            return FAIL;
        }
        last_node->next_node = n_node;
    }
    ab->size += 1;

    return SUCESS;
}

int ArgList_appendManual(struct ArgList* ab, enum KeyType kt, const void* key, enum DataType at, const void* arg) {
    struct NArgToken* nat = NArgToken_newConstruct(kt, key, at, arg);
    int temp = ArgList_append(ab, nat);
    NArgToken_end(nat);
    return temp;
}



int ArgList_remove(struct ArgList* ab, size_t index) {
    if(ab == NULL) {return INVALID_INPUT;}
    if(index >= ab->size) {return OUT_OF_RANGE;}

    size_t prev_index = {0};
    if(index == 0) {
        prev_index = 0;
    } else prev_index = index - 1;
    struct _ArgListNode* prvw_node = _ArgList_getNode(ab->head, prev_index);
    if(prvw_node == NULL) {return FAIL;}

    struct _ArgListNode* next_node = _ArgListNode_end(prvw_node->next_node);
    prvw_node = next_node;

    ab->size -= 1;
    return SUCESS;
}

void ArgListIterator_init(struct ArgListIterator* ali, const struct ArgList* al) {
    ali->_head = al->head;
    ali->iterator = al->head;
}


int ArgListIterator_jump(struct ArgListIterator* ali) {
    if(ali == NULL) {return INVALID_INPUT;}
    else if(ali->iterator == NULL) {return FAIL;}

    if(ali->iterator->next_node == NULL) {
        ali->iterator = ali->_head;
        return TURN_AROUND;
    }
    else {
        ali->iterator = ali->iterator->next_node;
        return SUCESS;
    }
}


struct NArgToken* ArgList_getArgPositional(struct ArgList* al, position pos) {
    if(al == NULL ) {return NULL;}

    struct ArgListIterator it;
    ArgListIterator_init(&it, al);
    do {
        if(it.iterator->at->key_type != POSITIONAL) {continue;}
        if(it.iterator->at->key.pos != pos) {continue;}
        return it.iterator->at;
    } while(ArgListIterator_jump(&it) != ITERATOR_TURN_AROUND);
    return NULL;
}

struct NArgToken* ArgList_getArgNamed(struct ArgList* al, const char* key) {
    if(al == NULL ) {return NULL;}

    struct ArgListIterator it;
    ArgListIterator_init(&it, al);
    do {
        if(it.iterator->at->key_type != NAMED) {continue;}
        if(strcmp(it.iterator->at->key.named, key) != 0) {continue;}
        return it.iterator->at;
    } while(ArgListIterator_jump(&it) != ITERATOR_TURN_AROUND);
    return NULL;
}
